// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "gltf.hpp"

#include <logger.hpp>
#include <file_manager.hpp>
#include <type_conversions.hpp>

namespace kirana::scene
{
template <class T>
void fromJson(const std::string &name, const glz::json_t &src, T &dst)
{
    if (const auto err = glz::read<glz::opts{}>(dst, src); err)
    {
        const std::string err_str = "Failed to parse GLTF: " + name + ": ";
        std::string buffer{};
        if (const auto w_err = glz::write_json(src, buffer); !w_err)
        {
            core::Logger::error(LOG_CHANNEL_GLTF, err_str + glz::format_error(err, buffer));
        }
        else
        {
            core::Logger::error(LOG_CHANNEL_GLTF, err_str + glz::format_error(err));
        }
    }
}

template <class T>
void toJson(const std::string &name, const T &src, glz::json_t &dst)
{
    std::string buffer{};
    if (const auto err = glz::write_json(src, buffer); !err)
    {
        glz::json_t material{};
        if (const auto r_err = glz::read_json(material, buffer); !r_err)
        {
            dst = material;
        }
        else
        {
            const std::string err_str = "Failed to serialize GLTF: " + name + "to json object: ";
            core::Logger::error(LOG_CHANNEL_GLTF, err_str + glz::format_error(r_err, buffer));
        }
    }
    else
    {
        const std::string err_str = "Failed to serialize GLTF: " + name + "to string: ";
        core::Logger::error(LOG_CHANNEL_GLTF, err_str + glz::format_error(err));
    }
}

void GLTFMaterial::setExtensions(const std::optional<glz::json_t> &extensions)
{
    if (!extensions)
    {
        return;
    }

    const auto &ext = extensions.value();
    if (ext.contains(GLTF_EXT_NAME_MAT_UNLIT))
    {
        unlit = true;
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_ANISOTROPY))
    {
        anisotropy = GLTFMaterialAnisotropy{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_ANISOTROPY);
        fromJson<GLTFMaterialAnisotropy>("ANISOTROPY", src, anisotropy.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_CLEARCOAT))
    {
        clearcoat = GLTFMaterialClearcoat{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_CLEARCOAT);
        fromJson<GLTFMaterialClearcoat>("CLEARCOAT", src, clearcoat.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION))
    {
        diffuse_transmission = GLTFMaterialDiffuseTransmission{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION);
        fromJson<GLTFMaterialDiffuseTransmission>("DIFFUSE TRANSMISSION", src, diffuse_transmission.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_DISPERSION))
    {
        dispersion = GLTFMaterialDispersion{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_DISPERSION);
        fromJson<GLTFMaterialDispersion>("DISPERSION", src, dispersion.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH))
    {
        emissive_strength = GLTFMaterialEmissiveStrength{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH);
        fromJson<GLTFMaterialEmissiveStrength>("EMISSIVE STRENGTH", src, emissive_strength.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_IOR))
    {
        ior = GLTFMaterialIOR{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_IOR);
        fromJson<GLTFMaterialIOR>("IOR", src, ior.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_IRIDESCENCE))
    {
        iridescence = GLTFMaterialIridescence{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_IRIDESCENCE);
        fromJson<GLTFMaterialIridescence>("IRIDESCENCE", src, iridescence.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_SHEEN))
    {
        sheen = GLTFMaterialSheen{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_SHEEN);
        fromJson<GLTFMaterialSheen>("SHEEN", src, sheen.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_SPECULAR))
    {
        specular = GLTFMaterialSpecular{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_SPECULAR);
        fromJson<GLTFMaterialSpecular>("SPECULAR", src, specular.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_TRANSMISSION))
    {
        transmission = GLTFMaterialTransmission{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_TRANSMISSION);
        fromJson<GLTFMaterialTransmission>("TRANSMISSION", src, transmission.value());
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_VOLUME))
    {
        volume = GLTFMaterialVolume{};
        const auto &src = ext.at(GLTF_EXT_NAME_MAT_VOLUME);
        fromJson<GLTFMaterialVolume>("VOLUME", src, volume.value());
    }
}

std::optional<glz::json_t> GLTFMaterial::getExtensions() const
{
    glz::json_t extensions{};
    if (unlit)
    {
        extensions[GLTF_EXT_NAME_MAT_UNLIT] = glz::json_t{};
    }
    if (anisotropy.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialAnisotropy>("ANISOTROPY", anisotropy.value(), material);
        extensions[GLTF_EXT_NAME_MAT_ANISOTROPY] = material;
    }
    if (clearcoat.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialClearcoat>("CLEARCOAT", clearcoat.value(), material);
        extensions[GLTF_EXT_NAME_MAT_CLEARCOAT] = material;
    }
    if (diffuse_transmission.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialDiffuseTransmission>("DIFFUSE TRANSMISSION", diffuse_transmission.value(), material);
        extensions[GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION] = material;
    }
    if (dispersion.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialDispersion>("DISPERSION", dispersion.value(), material);
        extensions[GLTF_EXT_NAME_MAT_DISPERSION] = material;
    }
    if (emissive_strength.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialEmissiveStrength>("EMISSIVE STRENGTH", emissive_strength.value(), material);
        extensions[GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH] = material;
    }
    if (ior.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialIOR>("IOR", ior.value(), material);
        extensions[GLTF_EXT_NAME_MAT_IOR] = material;
    }
    if (iridescence.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialIridescence>("IRIDESCENCE", iridescence.value(), material);
        extensions[GLTF_EXT_NAME_MAT_IRIDESCENCE] = material;
    }
    if (sheen.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialSheen>("SHEEN", sheen.value(), material);
        extensions[GLTF_EXT_NAME_MAT_SHEEN] = material;
    }
    if (specular.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialSpecular>("SPECULAR", specular.value(), material);
        extensions[GLTF_EXT_NAME_MAT_SPECULAR] = material;
    }
    if (transmission.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialTransmission>("TRANSMISSION", transmission.value(), material);
        extensions[GLTF_EXT_NAME_MAT_TRANSMISSION] = material;
    }
    if (volume.has_value())
    {
        glz::json_t material{};
        toJson<GLTFMaterialVolume>("VOLUME", volume.value(), material);
        extensions[GLTF_EXT_NAME_MAT_VOLUME] = material;
    }
    if (extensions.empty())
    {
        return nullptr;
    }
    return extensions;
}


GLTFBinaryDocument::GLTFBinaryDocument(const std::span<uint8_t> file_buffer)
{
    m_header = GLTFBinaryHeader{file_buffer};
    if (!m_header.isValid())
    {
        return;
    }

    size_t offset = 12;
    while (offset < m_header.length)
    {
        uint32_t chunk_length = 0;
        std::memcpy(&chunk_length, file_buffer.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        auto chunk_type = GLTFBinaryChunkType::JSON;
        std::memcpy(&chunk_type, file_buffer.data() + offset, sizeof(GLTFBinaryChunkType));
        offset += sizeof(GLTFBinaryChunkType);

        const std::span<uint8_t> chunk_data = file_buffer.subspan(offset, chunk_length);
        m_chunks.emplace_back(GLTFBinaryChunk{chunk_length, chunk_type, chunk_data});
        offset += chunk_length;
    }
}

bool GLTFLoader::loadBuffer(const uint32_t buffer_index)
{
    if (m_document.buffers.size() <= buffer_index)
    {
        return false;
    }
    if (m_buffer_views.contains(buffer_index))
    {
        return true;
    }
    const GLTFBuffer &buffer_info = m_document.buffers.at(buffer_index);
    if (!buffer_info.uri && buffer_index > 0)
    {
        core::Logger::error(LOG_CHANNEL_GLTF, "Buffer with index greater than 0 must have a valid URI");
        return false;
    }

    core::Logger::info(LOG_CHANNEL_GLTF, "Loading buffer: " + std::to_string(buffer_index));
    // If the buffer has an undefined URI, then there must be an embedded binary chunk in the GLB file buffer.
    if (!buffer_info.uri)
    {
        if (const auto bin_buffer = m_binary_doc.getBinaryBuffer(); !bin_buffer.empty())
        {
            m_buffer_views.insert(std::make_pair(0, bin_buffer));
            return true;
        }
        return false;
    }

    // Check if base-64
    if (core::DataURI::isValid(buffer_info.uri.value()))
    {
        auto uri = core::DataURI::parse(buffer_info.uri.value());
        const auto data_size = uri.data.size();
        if (data_size == 0)
        {
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to decode data URI for buffer index: " + std::to_string(buffer_index));
            return false;
        }
        const size_t offset = m_buffer.size();

        m_buffer.insert(m_buffer.end(), std::make_move_iterator(uri.data.begin()),
                        std::make_move_iterator(uri.data.end()));
        std::span buffer_view{m_buffer.data() + offset, data_size};
        m_buffer_views.insert(std::make_pair(buffer_index, buffer_view));
    }
    else
    {
        const core::Filepath path = core::Filepath{m_base_path} / core::Filepath{buffer_info.uri.value()};
        if (!core::fileExists(path))
        {
            core::Logger::error(LOG_CHANNEL_GLTF, "Failed to find buffer file: " + path.string());
            return false;
        }
        std::vector<uint8_t> buffer_data;
        buffer_data.resize(core::getFileSize(path));
        if (core::readFile(path, true, reinterpret_cast<char *>(buffer_data.data())) == 0)
        {
            core::Logger::error(LOG_CHANNEL_GLTF, "Failed to read buffer file: " + path.string());
            return false;
        }
        const size_t offset = m_buffer.size();
        const size_t buffer_size = buffer_data.size();
        m_buffer.insert(m_buffer.end(), std::make_move_iterator(buffer_data.begin()),
                        std::make_move_iterator(buffer_data.end()));

        m_buffer_views.insert(std::make_pair(buffer_index, std::span{m_buffer.data() + offset, buffer_size}));
    }
    return true;
}

bool GLTFLoader::loadImage(const uint32_t image_index, const bool read_pixels)
{
    if (m_document.images.size() <= image_index)
    {
        return false;
    }
    if (read_pixels && m_image_buffers.contains(image_index))
    {
        return true;
    }
    if (m_images.contains(image_index))
    {
        if (read_pixels && !m_image_buffers.contains(image_index))
        {
            core::Logger::info(LOG_CHANNEL_GLTF, "Loading image buffer: " + std::to_string(image_index));
            m_image_buffers.insert(std::make_pair(image_index, std::vector<uint8_t>{}));
            m_images.at(image_index).readPixels(m_image_buffers.at(image_index));
        }
        return true;
    }

    const GLTFImage &image_info = m_document.images.at(image_index);
    std::string image_name = image_info.name.value_or("Image_" + std::to_string(image_index));
    if (!image_info.uri)
    {
        // If image URI is not defined, then the image must be embedded in the GLTF binary buffer.
        if (!image_info.buffer_view)
        {
            core::Logger::error(LOG_CHANNEL_GLTF, "Image should have a valid URI or buffer view");
            return false;
        }
        const auto &buffer_view = m_document.buffer_views.at(image_info.buffer_view.value());
        std::vector<uint8_t> buffer_data{};
        buffer_data.resize(buffer_view.byte_length);
        if (!loadBufferViewData(buffer_view, buffer_data.data()))
        {
            core::Logger::error(LOG_CHANNEL_GLTF, "Failed to load buffer for image: " + std::to_string(image_index));
            return false;
        }

        if (const auto mime_type = image_info.mime_type.value_or(GLTFImageMimeType::PNG);
            mime_type == GLTFImageMimeType::PNG)
        {
            image_name += ".png";
        }
        else if (mime_type == GLTFImageMimeType::JPEG)
        {
            image_name += ".jpg";
        }
        // Load the buffer and read the pixels irrespective of whether `read_pixels` is true.
        core::Logger::info(LOG_CHANNEL_GLTF, "Loading image buffer: " + std::to_string(image_index));
        std::vector<uint8_t> pixel_buffer{};
        const auto image = core::Image::loadFromRawBuffer(image_name, buffer_data, pixel_buffer);
        if (!image.isValid())
        {
            core::Logger::error(LOG_CHANNEL_GLTF, "Failed to load image from buffer: " + std::to_string(image_index));
            return false;
        }
        m_images.insert(std::make_pair(image_index, image));
        m_image_buffers.insert(std::make_pair(image_index, std::move(pixel_buffer)));
    }
    else
    {
        std::string image_uri = image_info.uri.value();
        // If the given image is not embedded as data URI, make the path absolute.
        if (!core::DataURI::isValid(image_uri))
        {
            image_uri = (core::Filepath{m_base_path} / core::Filepath{image_info.uri.value()}).string();
        }
        m_images.insert(std::make_pair(image_index, core::Image{image_name, image_uri}));

        if (read_pixels)
        {
            core::Logger::info(LOG_CHANNEL_GLTF, "Loading image buffer: " + std::to_string(image_index));
            m_image_buffers.insert(std::make_pair(image_index, std::vector<uint8_t>{}));
            m_images.at(image_index).readPixels(m_image_buffers.at(image_index));
        }
    }
    return true;
}

GLTFLoader::GLTFLoader(std::string path, const bool load_buffers, const bool load_images)
    : m_path{std::move(path)}, m_base_path{core::Filepath{m_path}.parent_path().string()}
{
    using namespace core;

    const Filepath filepath{m_path};
    std::vector<uint8_t> file_data;
    file_data.resize(getFileSize(filepath));
    if (readFile(filepath, true, reinterpret_cast<char *>(file_data.data())) == 0)
    {
        Logger::error(LOG_CHANNEL_GLTF, "Failed to read GLTF file");
        return;
    }

    if (GLTFBinaryHeader{file_data}.isValid())
    {
        // If the given file has a GLB header, interpret the file buffer as GLB chunks.
        m_buffer = std::move(file_data);
        m_binary_doc = GLTFBinaryDocument{m_buffer};
        if (const auto json_buffer = m_binary_doc.getJSONBuffer(); !json_buffer.empty())
        {
            if (const auto err = glz::read<glz::opts{.error_on_unknown_keys = false}>(m_document, json_buffer); err)
            {
                Logger::error(LOG_CHANNEL_GLTF,
                              "Failed to parse JSON chunk from the GLB file: " + glz::format_error(err));
            }
        }
        else
        {
            Logger::error(LOG_CHANNEL_GLTF, "JSON Chunk is missing from the GLB file");
        }
        loadBuffer(0);
    }
    else
    {
        // Consider the file buffer as GLTF JSON.
        if (const auto err = glz::read<glz::opts{.error_on_unknown_keys = false}>(m_document, file_data); err)
        {
            Logger::error(LOG_CHANNEL_GLTF, "Failed to parse JSON from the GLTF file: " + glz::format_error(err));
        }
    }
    if (load_buffers)
    {
        for (uint32_t i = 0; i < m_document.buffers.size(); ++i)
        {
            loadBuffer(i);
        }
    }

    for (uint32_t i = 0; i < m_document.images.size(); ++i)
    {
        loadImage(i, load_images);
    }
}

std::span<const uint8_t> GLTFLoader::getBuffer(const uint32_t buffer_index)
{
    if (m_document.buffers.empty())
    {
        core::Logger::error(LOG_CHANNEL_GLTF, "No buffers found in the GLTF document");
        return {};
    }

    // If the buffer is preloaded, return it as is.
    if (m_buffer_views.contains(buffer_index))
    {
        return m_buffer_views.at(buffer_index);
    }

    // If the buffer exists in the document, load it and then return it.
    if (loadBuffer(buffer_index))
    {
        return m_buffer_views.at(buffer_index);
    }

    // No such buffer exists.
    return {};
}

std::span<const uint8_t> GLTFLoader::getImageBuffer(const uint32_t image_index)
{
    if (m_document.images.empty())
    {
        core::Logger::error(LOG_CHANNEL_GLTF, "No images found in the GLTF document");
        return {};
    }

    // If the image buffer is preloaded, return it as is.
    if (m_image_buffers.contains(image_index))
    {
        return m_image_buffers.at(image_index);
    }

    // If the image buffer exists in the document, load it and then return it.
    if (loadImage(image_index, true))
    {
        return m_image_buffers.at(image_index);
    }

    // No such image exists.
    return {};
}

bool GLTFLoader::loadBufferViewData(const GLTFBufferView &view, uint8_t *out_data)
{
    if (!view.isValid())
    {
        core::Logger::error(LOG_CHANNEL_GLTF, "Invalid buffer view");
        return false;
    }
    const auto &buffer_data = getBuffer(view.buffer);
    if (buffer_data.empty())
    {
        core::Logger::error(LOG_CHANNEL_GLTF,
                            "Buffer with index: " + std::to_string(view.buffer) + " does not exist or is empty.");
        return false;
    }
    std::memcpy(out_data, buffer_data.data() + view.byte_offset, view.byte_length);
    return true;
}


bool GLTFLoader::loadAccessorData(const GLTFAccessor &accessor, uint8_t *out_data)
{
    if (!accessor.isValid())
    {
        core::Logger::error(LOG_CHANNEL_GLTF, "Invalid accessor");
        return false;
    }
    if (accessor.isSparse())
    {
        // TODO: Add support for GLTF sparse accessor.
        core::Logger::warn(LOG_CHANNEL_GLTF, "Sparse accessor is not yet supported.");
        return false;
    }
    if (accessor.buffer_view.value() >= m_document.buffer_views.size())
    {
        core::Logger::error(LOG_CHANNEL_GLTF,
                            "Buffer view with index: " + std::to_string(accessor.buffer_view.value()) +
                            " does not exist.");
        return false;
    }
    const auto &view = m_document.buffer_views.at(accessor.buffer_view.value());
    const auto &buffer_data = getBuffer(view.buffer);
    if (buffer_data.empty())
    {
        core::Logger::error(LOG_CHANNEL_GLTF,
                            "Buffer with index: " + std::to_string(view.buffer) + " does not exist or is empty.");
    }

    const uint64_t base_offset = accessor.byte_offset + view.byte_offset;
    const size_t element_size = accessor.getPerElementSize();
    if (const uint64_t byte_stride = view.byte_stride.value_or(0); byte_stride == 0)
    {
        const size_t copy_size = accessor.count * element_size;
        std::memcpy(out_data, buffer_data.data() + base_offset, copy_size);
    }
    else
    {
        uint64_t src_offset = base_offset;
        uint64_t dst_offset = 0;
        for (uint32_t i = 0; i < accessor.count; ++i)
        {
            std::memcpy(out_data + dst_offset, buffer_data.data() + src_offset, element_size);
            src_offset += byte_stride;
            dst_offset += element_size;
        }
    }
    return true;
}
}