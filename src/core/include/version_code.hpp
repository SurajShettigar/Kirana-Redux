// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_VERSION_CODE_HPP
#define KIRANA_CORE_VERSION_CODE_HPP

#include <string>

namespace kirana::core
{
struct VersionCode
{
    uint32_t major = 0;
    uint32_t minor = 0;
    uint32_t patch = 0;

    VersionCode() = default;

    explicit VersionCode(const uint32_t major, const uint32_t minor, const uint32_t patch)
        : major{major}, minor{minor}, patch{patch}
    {
    }

    [[nodiscard]] std::string toString() const
    {
        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    }

    explicit operator std::string() const
    {
        return toString();
    }
};
}
#endif  // KIRANA_CORE_VERSION_CODE_HPP