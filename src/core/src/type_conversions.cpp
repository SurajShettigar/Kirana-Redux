// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "type_conversions.hpp"

#include <cppcodec/base64_rfc4648.hpp>

namespace kirana::core
{
    std::vector<uint8_t> base64ToBytes(const std::string& value)
    {
        using base64 = cppcodec::base64_rfc4648;
        return base64::decode(value);
    }
}
