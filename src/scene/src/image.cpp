// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "image.hpp"

#include <file_manager.hpp>
#include <type_conversions.hpp>
#include <logger.hpp>

#include <OpenImageIO/imageio.h>
#include <OpenImageIO/filesystem.h>
#include <OpenImageIO/imagebuf.h>

namespace kirana::scene
{
const std::string LOG_CHANNEL_IMAGE{"IMAGE"};

inline ImageChannelFormat getFormat(const OIIO::TypeDesc type)
{
    switch (type.basetype)
    {
    case OIIO::TypeDesc::UINT8:
        return ImageChannelFormat::UINT_8;
    case OIIO::TypeDesc::INT8:
        return ImageChannelFormat::INT_8;
    case OIIO::TypeDesc::UINT16:
        return ImageChannelFormat::UINT_16;
    case OIIO::TypeDesc::INT16:
        return ImageChannelFormat::INT_16;
    case OIIO::TypeDesc::UINT32:
        return ImageChannelFormat::UINT_32;
    case OIIO::TypeDesc::INT32:
        return ImageChannelFormat::INT_32;
    case OIIO::TypeDesc::UINT64:
        return ImageChannelFormat::UINT_64;
    case OIIO::TypeDesc::INT64:
        return ImageChannelFormat::INT_64;
    case OIIO::TypeDesc::HALF:
        return ImageChannelFormat::HALF;
    case OIIO::TypeDesc::FLOAT:
        return ImageChannelFormat::FLOAT;
    case OIIO::TypeDesc::DOUBLE:
        return ImageChannelFormat::DOUBLE;
    default:
        return ImageChannelFormat::UNKNOWN;
    }
}

inline OIIO::TypeDesc getTypeDesc(const ImageChannelFormat format)
{
    switch (format)
    {
    case ImageChannelFormat::UINT_8:
        return OIIO::TypeDesc::UINT8;
    case ImageChannelFormat::INT_8:
        return OIIO::TypeDesc::INT8;
    case ImageChannelFormat::UINT_16:
        return OIIO::TypeDesc::UINT16;
    case ImageChannelFormat::INT_16:
        return OIIO::TypeDesc::INT16;
    case ImageChannelFormat::UINT_32:
        return OIIO::TypeDesc::UINT32;
    case ImageChannelFormat::INT_32:
        return OIIO::TypeDesc::INT32;
    case ImageChannelFormat::UINT_64:
        return OIIO::TypeDesc::UINT64;
    case ImageChannelFormat::INT_64:
        return OIIO::TypeDesc::INT64;
    case ImageChannelFormat::HALF:
        return OIIO::TypeDesc::HALF;
    case ImageChannelFormat::FLOAT:
        return OIIO::TypeDesc::FLOAT;
    case ImageChannelFormat::DOUBLE:
        return OIIO::TypeDesc::DOUBLE;
    default:
        return OIIO::TypeDesc::UNKNOWN;
    }
}

inline size_t getChannelFormatSize(const ImageChannelFormat format)
{
    switch (format)
    {
    case ImageChannelFormat::UINT_8:
    case ImageChannelFormat::INT_8:
        return 1;
    case ImageChannelFormat::UINT_16:
    case ImageChannelFormat::INT_16:
    case ImageChannelFormat::HALF:
        return 2;
    case ImageChannelFormat::UINT_32:
    case ImageChannelFormat::INT_32:
    case ImageChannelFormat::FLOAT:
        return 4;
    case ImageChannelFormat::UINT_64:
    case ImageChannelFormat::INT_64:
    case ImageChannelFormat::DOUBLE:
        return 8;
    default:
        return 0;
    }
}

inline size_t getBufferSize(const uint32_t width, const uint32_t height, const uint32_t num_channels,
                            const ImageChannelFormat format)
{
    return width * height * num_channels * getChannelFormatSize(format);
}

inline std::optional<OIIO::Filesystem::IOMemReader> getUriReader(const std::string &path, core::DataURI &out_uri)
{
    using namespace OIIO;
    if (core::DataURI::isValid(path))
    {
        out_uri = core::DataURI::parse(path);
        if (!out_uri.isValid())
        {
            return std::nullopt;
        }
        return Filesystem::IOMemReader(out_uri.data.data(), out_uri.data.size());
    }
    return std::nullopt;
}

bool Image::doLoad()
{
    using namespace OIIO;

    std::unique_ptr<ImageInput> img = nullptr;
    core::DataURI data_uri{};
    auto uri_reader = getUriReader(m_path, data_uri);
    if (uri_reader)
    {
        auto filename = data_uri.media_type;
        std::ranges::replace(filename, '/', '.');
        img = ImageInput::open(filename, nullptr, &uri_reader.value());
    }
    else
    {
        img = ImageInput::open(m_path);
    }

    if (!img)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to open image: " + m_path);
        return false;
    }
    const auto &img_spec = img->spec();
    if (const auto color_space_val = img_spec.find_attribute("oiio:ColorSpace", TypeDesc::STRING); color_space_val)
    {
        const auto color_space = color_space_val->get_string();
        m_is_srgb = color_space == "sRGB";
    }
    m_width = img_spec.width;
    m_height = img_spec.height;
    m_channels = img_spec.nchannels;
    m_format = getFormat(img_spec.format);

    img->close();
    return true;
}

Image::Image(std::string path, const std::vector<uint8_t> &raw_buffer) : m_path{std::move(path)}
{
    using namespace OIIO;

    auto reader = Filesystem::IOMemReader(raw_buffer.data(), raw_buffer.size());
    const auto img = ImageInput::open(m_path, nullptr, &reader);
    if (!img)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to open raw image buffer: " + m_path);
        return;
    }
    const auto &img_spec = img->spec();
    const auto img_out = ImageOutput::create(m_path);
    if (!img_out)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to create file for raw image buffer: " + m_path);
        return;
    }
    img_out->open(m_path, img_spec);
    if (!img_out->copy_image(img.get()))
    {
        core::Logger::error(LOG_CHANNEL_IMAGE,
                            "Failed to write raw image buffer to file: " + m_path + " Error: " + img_out->geterror());
    }
    img_out->close();
    img->close();
}

bool Image::readPixelBuffer(std::vector<uint8_t> &pixel_buffer, ImageChannelFormat format, uint32_t num_channels) const
{
    using namespace OIIO;
    if (!isValid())
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Image has not been initialized correctly: " + m_path);
        return false;
    }

    bool convert = format != ImageChannelFormat::UNKNOWN && format != m_format;
    format = convert ? format : m_format;

    num_channels = std::min(num_channels, 4u);
    const bool convert_channels = num_channels > 0 && num_channels != m_channels;
    num_channels = convert_channels ? num_channels : m_channels;

    convert |= convert_channels;

    // In case the image needs to be converted, we need to pass OIIO's ImageSpec when opening the image.
    const TypeDesc native_format = getTypeDesc(format);
    const ImageSpec img_spec{static_cast<int>(m_width), static_cast<int>(m_height), static_cast<int>(num_channels),
                             native_format};

    std::unique_ptr<ImageInput> img = nullptr;
    core::DataURI data_uri{};
    auto uri_reader = getUriReader(m_path, data_uri);
    if (uri_reader)
    {
        auto filename = data_uri.media_type;
        std::ranges::replace(filename, '/', '.');
        img = ImageInput::open(filename, convert ? &img_spec : nullptr, &uri_reader.value());
    }
    else
    {
        img = ImageInput::open(m_path, convert ? &img_spec : nullptr);
    }

    if (!img)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to open image: " + m_path);
        return false;
    }

    const size_t num_bytes = getBufferSize(m_width, m_height, num_channels, format);
    pixel_buffer.resize(num_bytes);

    stride_t x_stride = img->spec().nchannels < num_channels
                            ? num_channels * static_cast<stride_t>(getChannelFormatSize(format))
                            : AutoStride;
    const bool status =
        img->read_image(0, 0, 0, static_cast<int32_t>(num_channels), native_format, pixel_buffer.data(), x_stride);
    img->close();
    if (!status)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to read image buffer: " + m_path);
        return false;
    }
    return true;
}
} // namespace kirana::scene