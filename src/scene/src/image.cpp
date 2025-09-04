// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "image.hpp"

#include <file_manager.hpp>
#include <type_conversions.hpp>

#include <OpenImageIO/imageio.h>
#include <OpenImageIO/filesystem.h>

namespace kirana::scene
{

Image Image::loadFromRawBuffer(const std::string &name, const std::vector<uint8_t> &buffer, std::vector<uint8_t> &out_pixels)
{
    using namespace OIIO;
    Image image{name, ""};

    auto reader = Filesystem::IOMemReader(buffer.data(), buffer.size());

    const auto img = ImageInput::open(name, nullptr, &reader);
    if (!img)
    {
        return image;
    }
    const auto &img_spec = img->spec();
    image.m_width = img_spec.width;
    image.m_height = img_spec.height;
    image.m_channels = img_spec.nchannels;

    // TODO: Add reading of pixels with size greater than 8 bits.
    const size_t num_bytes = image.m_width * image.m_height * image.m_channels;
    out_pixels.clear();
    out_pixels.resize(num_bytes);

    img->read_image(0, 0, 0, static_cast<int32_t>(image.m_channels), TypeDesc::UINT8,
                    out_pixels.data());
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

Image::Image(std::string name, std::string path)
    : IResource{}, m_name{std::move(name)}, m_path{std::move(path)}
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
        return;
    }
    const auto &img_spec = img->spec();
    m_width = img_spec.width;
    m_height = img_spec.height;
    m_channels = img_spec.nchannels;
    img->close();
}

bool Image::readPixels(std::vector<uint8_t> &out_buffer) const
{
    using namespace OIIO;
    if (!isValid())
    {
        return false;
    }

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
        return false;
    }
    // TODO: Add reading of pixels with size greater than 8 bits.
    const size_t num_bytes = m_width * m_height * m_channels;
    out_buffer.clear();
    out_buffer.resize(num_bytes);

    const bool status = img->read_image(0, 0, 0, static_cast<int32_t>(m_channels), TypeDesc::UINT8, out_buffer.data());
    img->close();
    return status;
}
}