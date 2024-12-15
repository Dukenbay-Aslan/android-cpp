#include "logger.h"

std::atomic<ELogLevel> TLogger::level_{ELogLevel::DEBUG};

/**
 * @brief Constructor. Also construct
 * underlying `logger`s for all levels
 * @param serviceName Prefix a service
 * name for any message
 */
TLogger::TLogger(const std::string& serviceName)
    : error("[ERROR] " + serviceName + ": ",
        ELogLevel::ERROR,
        level_,
        mutex)
    , warning("[WARNING] " + serviceName + ": ",
        ELogLevel::WARNING,
        level_,
        mutex)
    , info("[INFO] " + serviceName + ": ",
        ELogLevel::INFO,
        level_,
        mutex)
    , debug("[DEBUG] " + serviceName + ": ",
        ELogLevel::DEBUG,
        level_,
        mutex)
{

}

/**
 * @brief Set general logging level
 * @param level Level to set
 */
void TLogger::setLevel(ELogLevel level) {
    level_.store(level);
}

/**
 * @brief Get the current general logging level
 * @return `static TLogger::level_`
 */
ELogLevel TLogger::getLevel() {
    return level_.load();
}

/**
 * @brief Constructor
 * @param prefix Prefix of log on this level
 * @param level This logger's level
 * @param currentLevel Parent's log level
 * @param mutex Parent's mutex
 */
TLogger::logger::logger(const std::string& prefix,
        ELogLevel level,
        std::atomic<ELogLevel>& currentLevel,
        std::mutex& mutex)
    : prefix_(prefix)
    , level_(level)
    , currentLevel_(currentLevel)
    , parentMutex(mutex)
{

}