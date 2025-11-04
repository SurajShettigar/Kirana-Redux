// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "type_conversions.hpp"

#include <cppcodec/base64_rfc4648.hpp>

#include <sstream>
#include <regex>

inline std::vector<std::string> split(const std::string &value, const char delimiter)
{
    std::stringstream ss{value};
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delimiter))
    {
        elems.push_back(item);
    }
    return elems;
}

namespace kirana::core
{

DataURI DataURI::parse(const std::string &uri)
{
    static const std::regex uri_regex{R"(^data:([^;,]*)(;base64)?,(.*)$)", std::regex::icase};
    DataURI result{};

    std::smatch matches;
    if (!std::regex_match(uri, matches, uri_regex))
    {
        return result;
    }
    if (const auto media_type = matches[1].str(); !media_type.empty())
    {
        result.media_type = media_type;
    }
    result.is_base64 = !matches[2].str().empty();
    const auto data = matches[3].str();
    result.data = result.is_base64 ? base64ToBytes(data) : std::vector<uint8_t>{data.begin(), data.end()};
    return result;
}

std::vector<uint8_t> base64ToBytes(const std::string &value)
{
    using base64 = cppcodec::base64_rfc4648;
    return base64::decode(value);
}
}