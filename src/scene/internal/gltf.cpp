// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "gltf.hpp"

#include <logger.hpp>

namespace kirana::scene
{
void GLTFMaterial::setExtensions(const std::optional<glz::json_t> &extensions)
{
    if (!extensions)
    {
        return;
    }
    const auto ext = extensions.value();
    if (ext.contains(GLTF_EXT_NAME_MAT_UNLIT))
    {
        unlit = true;
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_ANISOTROPY))
    {
        anisotropy = GLTFMaterialAnisotropy{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_ANISOTROPY);
        if (const auto err = glz::read<glz::opts{}>(*anisotropy, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: ANISOTROPY: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_CLEARCOAT))
    {
        clearcoat = GLTFMaterialClearcoat{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_CLEARCOAT);
        if (const auto err = glz::read<glz::opts{}>(*clearcoat, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: CLEARCOAT: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION))
    {
        diffuse_transmission = GLTFMaterialDiffuseTransmission{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION);
        if (const auto err = glz::read<glz::opts{}>(*diffuse_transmission, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: DIFFUSE TRANSMISSION: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_DISPERSION))
    {
        dispersion = GLTFMaterialDispersion{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_DISPERSION);
        if (const auto err = glz::read<glz::opts{}>(*dispersion, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: DISPERSION: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH))
    {
        emissive_strength = GLTFMaterialEmissiveStrength{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH);
        if (const auto err = glz::read<glz::opts{}>(*emissive_strength, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: EMISSIVE STRENGTH: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_IOR))
    {
        ior = GLTFMaterialIOR{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_IOR);
        if (const auto err = glz::read<glz::opts{}>(*ior, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: IOR: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_IRIDESCENCE))
    {
        iridescence = GLTFMaterialIridescence{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_IRIDESCENCE);
        if (const auto err = glz::read<glz::opts{}>(*iridescence, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: IRIDESCENCE: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_SHEEN))
    {
        sheen = GLTFMaterialSheen{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_SHEEN);
        if (const auto err = glz::read<glz::opts{}>(*sheen, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: SHEEN: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_SPECULAR))
    {
        specular = GLTFMaterialSpecular{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_SPECULAR);
        if (const auto err = glz::read<glz::opts{}>(*specular, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: SPECULAR: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_TRANSMISSION))
    {
        transmission = GLTFMaterialTransmission{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_TRANSMISSION);
        if (const auto err = glz::read<glz::opts{}>(*transmission, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: TRANSMISSION: "
                                + glz::format_error(err, buffer));
        }
    }
    if (ext.contains(GLTF_EXT_NAME_MAT_VOLUME))
    {
        volume = GLTFMaterialVolume{};
        const auto src = ext.at(GLTF_EXT_NAME_MAT_VOLUME);
        if (const auto err = glz::read<glz::opts{}>(*volume, src); err)
        {
            std::string buffer{};
            const auto _ = glz::write_json(src, buffer);
            core::Logger::error(LOG_CHANNEL_GLTF,
                                "Failed to parse GLTF material: VOLUME: "
                                + glz::format_error(err, buffer));
        }
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
        std::string buffer{};
        if (const auto err = glz::write_json(anisotropy.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_ANISOTROPY] = material;
        }
    }
    if (clearcoat.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(clearcoat.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_CLEARCOAT] = material;
        }
    }
    if (diffuse_transmission.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(diffuse_transmission.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION] = material;
        }
    }
    if (dispersion.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(dispersion.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_DISPERSION] = material;
        }
    }
    if (emissive_strength.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(emissive_strength.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH] = material;
        }
    }
    if (ior.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(ior.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_IOR] = material;
        }
    }
    if (iridescence.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(iridescence.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_IRIDESCENCE] = material;
        }
    }
    if (sheen.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(sheen.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_SHEEN] = material;
        }
    }
    if (specular.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(specular.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_SPECULAR] = material;
        }
    }
    if (transmission.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(transmission.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_TRANSMISSION] = material;
        }
    }
    if (volume.has_value())
    {
        std::string buffer{};
        if (const auto err = glz::write_json(volume.value(), buffer); !err)
        {
            glz::json_t material{};
            glz::read_json(material, buffer);
            extensions[GLTF_EXT_NAME_MAT_VOLUME] = material;
        }
    }
    if (extensions.empty())
    {
        return nullptr;
    }
    return extensions;
}
}