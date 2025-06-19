// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "logger.hpp"

#include <spdlog/spdlog.h>

namespace kirana::core
{
inline spdlog::level::level_enum getLogLevel(const Logger::Level level)
{
    switch (level)
    {
    case Logger::Level::DEBUG:
        return spdlog::level::debug;
    case Logger::Level::INFO:
        return spdlog::level::info;
    case Logger::Level::WARN:
        return spdlog::level::warn;
    case Logger::Level::ERROR:
        return spdlog::level::err;
    case Logger::Level::NONE:
        return spdlog::level::off;
    default:
        return spdlog::level::trace;
    }
}

void Logger::init(const std::string &app_name, const Level level)
{
    m_channel = app_name;
    m_level = level;

    spdlog::set_level(getLogLevel(m_level));
    spdlog::set_pattern("[%H:%M:%S:%e] [%^%L%$] [thread %t] %v");
}

void Logger::log(const std::string &channel, const std::string &message, const Level level)
{
    spdlog::log(getLogLevel(level), "[{0}] {1}", channel, message);
}

void Logger::debug(const std::string &channel, const std::string &message)
{
    spdlog::debug("[{0}] {1}", channel, message);
}

void Logger::info(const std::string &channel, const std::string &message)
{
    spdlog::info("[{0}] {1}", channel, message);
}

void Logger::warn(const std::string &channel, const std::string &message)
{
    spdlog::warn("[{0}] {1}", channel, message);
}

void Logger::error(const std::string &channel, const std::string &message)
{
    spdlog::error("[{0}] {1}", channel, message);
}
}