// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_LOGGER_HPP
#define KIRANA_CORE_LOGGER_HPP

#include <string>

#include "no_copy.hpp"

namespace kirana::core
{
/**
 * A general purpose Logging wrapper.
 */
class Logger : NoCopy
{
public:
    enum class Level
    {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        NONE = 4,
        COUNT = 5
    };

    static Logger &get()
    {
        static Logger logger;
        return logger;
    }

    /**
     * Initializes logger and it's dependencies.
     * @param app_name The name of the app will be used as the default channel name
     * to tag log messages.
     * @param level Max log level messages to be processed. DEBUG is the lowest
     * level and will process all messages, NONE is the highest level and will not
     * process any messages (including errors).
     */
    void init(const std::string &app_name, Level level);

    /**
     * Process a general log message based on the given level and channel.
     * @param channel Named tag given to the log message.
     * @param message Log content.
     * @param level Log level indicating its severity. The log message will not
     * be processed if its level is below the one specified during initialization.
     */
    static void log(const std::string &channel, const std::string &message, Level level = Level::DEBUG);

    /**
     * Process a general log message based on the given level. The default channel
     * name is used to tag the message.
     * @param message Log content.
     * @param level Log level indicating its severity. The log message will not
     * be processed if its level is below the one specified during initialization.
     */
    void log(const std::string &message, const Level level = Level::DEBUG) const
    {
        log(m_channel, message, level);
    }

    /**
     * Process a debug log message.
     * @param channel Named tag given to the log message.
     * @param message Debug log content.
     */
    static void debug(const std::string &channel, const std::string &message);
    /**
     * Process a debug log message.
     * @param message Debug log content.
     */
    void debug(const std::string &message) const
    {
        debug(m_channel, message);
    }

    /**
     * Process an information log message.
     * @param channel Named tag given to the log message.
     * @param message Informational log content.
     */
    static void info(const std::string &channel, const std::string &message);
    /**
     * Process an information log message.
     * @param message Informational log content.
     */
    void info(const std::string &message) const
    {
        info(m_channel, message);
    }

    /**
     * Process a warning log message.
     * @param channel Named tag given to the log message.
     * @param message Warning log content.
     */
    static void warn(const std::string &channel, const std::string &message);
    /**
     * Process a warning log message.
     * @param message Warning log content.
     */
    void warn(const std::string &message) const
    {
        warn(m_channel, message);
    }

    /**
     * Process an error log message.
     * @param channel Named tag given to the log message.
     * @param message Error log content.
     */
    static void error(const std::string &channel, const std::string &message);
    /**
     * Process an error log message.
     * @param message Error log content.
     */
    void error(const std::string &message) const
    {
        error(m_channel, message);
    }

private:
    Level m_level = Level::DEBUG;
    std::string m_channel = "APP";

    Logger() = default;
    ~Logger() = default;
};
}
#endif  // KIRANA_CORE_LOGGER_HPP