// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "image.hpp"

#include <file_manager.hpp>
#include <type_conversions.hpp>
#include <logger.hpp>

#include <OpenImageIO/imageio.h>
#include <OpenImageIO/filesystem.h>

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

Image Image::loadFromRawBuffer(const std::string &path, const std::vector<uint8_t> &buffer)
{
    using namespace OIIO;
    Image image{};
    image.m_path = path;
    image.m_is_raw_buffer = true;

    auto reader = Filesystem::IOMemReader(buffer.data(), buffer.size());

    const auto img = ImageInput::open(image.m_path, nullptr, &reader);
    if (!img)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to open image: " + image.m_path);
        return image;
    }
    const auto &img_spec = img->spec();
    image.m_width = img_spec.width;
    image.m_height = img_spec.height;
    image.m_channels = img_spec.nchannels;
    image.m_format = getFormat(img_spec.format);

    const size_t num_bytes = getBufferSize(image.m_width, image.m_height, image.m_channels, image.m_format);
    image.m_pixels.clear();
    image.m_pixels.resize(num_bytes);

    img->read_image(0, 0, 0, img_spec.nchannels, img_spec.format, image.m_pixels.data());
    img->close();
    return image;
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

Image::Image(std::string path, const bool read_pixels) : IResource{}, m_path{std::move(path)}
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
        return;
    }
    const auto &img_spec = img->spec();
    m_width = img_spec.width;
    m_height = img_spec.height;
    m_channels = img_spec.nchannels;
    m_format = getFormat(img_spec.format);

    if (read_pixels)
    {
        const size_t num_bytes = getBufferSize(m_width, m_height, m_channels, m_format);
        m_pixels.resize(num_bytes);
        if (!img->read_image(0, 0, 0, img_spec.nchannels, img_spec.format, m_pixels.data()))
        {
            core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to read image buffer: " + m_path);
        }
    }
    img->close();
}

const std::vector<uint8_t> &Image::readPixelBuffer(const ImageChannelFormat convert_format)
{
    using namespace OIIO;
    if (!isValid())
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Image has not been initialized correctly: " + m_path);
        return m_pixels;
    }
    // TODO: Add option to convert images which were constructed through raw image buffer.
    if (m_is_raw_buffer)
    {
        return m_pixels;
    }
    bool convert = convert_format != ImageChannelFormat::UNKNOWN && m_format != convert_format;
    if (!m_pixels.empty() && !convert)
    {
        return m_pixels;
    }
    m_format = convert ? convert_format : m_format;

    // In case the image needs to be converted, we need to pass OIIO's ImageSpec when opening the image.
    const TypeDesc native_format = getTypeDesc(m_format);
    const ImageSpec img_spec{static_cast<int>(m_width), static_cast<int>(m_height),
                             static_cast<int>(m_channels), native_format};

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
        return m_pixels;
    }

    const size_t num_bytes = getBufferSize(m_width, m_height, m_channels, m_format);
    m_pixels.clear();
    m_pixels.resize(num_bytes);

    const bool status = img->read_image(0, 0, 0, static_cast<int32_t>(m_channels), native_format, m_pixels.data());
    img->close();
    if (!status)
    {
        core::Logger::error(LOG_CHANNEL_IMAGE, "Failed to read image buffer: " + m_path);
    }
    return m_pixels;
}
}