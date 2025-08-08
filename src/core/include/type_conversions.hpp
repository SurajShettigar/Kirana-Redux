// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_TYPE_CONVERSIONS_HPP
#define KIRANA_CORE_TYPE_CONVERSIONS_HPP

#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

namespace kirana::core
{
struct DataURI
{
    std::string media_type{"text/plain"};
    bool is_base64{false};
    std::vector<uint8_t> data{};

    static DataURI parse(const std::string &uri);

    static bool isValid(const std::string &uri)
    {
        return uri.starts_with("data:");
    }

    [[nodiscard]] bool isValid() const
    {
        return !data.empty();
    }
};

template <typename T1, typename T2>
static T2 bit_cast(T1 val)
{
    static_assert(sizeof(T1) == sizeof(T2), "Types must match sizes");
    static_assert(std::is_pod_v<T1>, "Requires POD input");
    static_assert(std::is_pod_v<T2>, "Requires POD output");

    T2 res;
    std::memcpy(std::addressof(res), std::addressof(val), sizeof(T1));
    return res;
}

std::vector<uint8_t> base64ToBytes(const std::string &value);
}

#endif  // KIRANA_CORE_TYPE_CONVERSIONS_HPP