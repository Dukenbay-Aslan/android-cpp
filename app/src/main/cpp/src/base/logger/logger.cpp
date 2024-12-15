#include "../helpers/utils.h"
#include "logger.h"

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
    level_.store(ELogLevel::DEBUG);
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

/**
 * @brief Log a message
 * @tparam T Data type of a message
 * @param message Message to log
 * @return Concatenated logs
 */
template<typename T>
TLogger::logger& TLogger::logger::operator<<(const T& message) {
    if (level_ <= currentLevel_.load()) {
        std::lock_guard<std::mutex> lock(parentMutex);
        std::cout << utils::now() << prefix_ << message;
    }
    return (*this);
}

/**
 * @brief Concatenate a manipulator to log
 * @param manip Manipulator to concatenate
 * @return Concatenated logs
 */
TLogger::logger& TLogger::logger::operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (level_ <= currentLevel_.load()) {
        std::lock_guard<std::mutex> lock(parentMutex);
        std::cout << manip;
    }
    return (*this);
}