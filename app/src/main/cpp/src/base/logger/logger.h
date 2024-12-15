#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <iostream>

#include "../Structs.h"

class TLogger {
  public:
    TLogger(const std::string& prefix);
    static void setLevel(ELogLevel level);
    static ELogLevel getLevel();
  private:
    /**
     * @brief General logging level.
     * Defaults to `DEBUG`
     */
    static std::atomic<ELogLevel> level_;
    /**
     * @brief Mutual exclusion
     * for thread-safe logging
     */
    std::mutex mutex;
    /**
     * @brief Underlying structure
     * for logging on specific level
     */
    class logger {
      public:
        logger(const std::string& prefix,
            ELogLevel level,
            std::atomic<ELogLevel>& currentLevel,
            std::mutex& mutex);
        template<typename T>
        logger& operator<<(const T& message);
        logger& operator<<(std::ostream& (*manip)(std::ostream&));
      private:
        /**
         * @brief Prefix of log on this level
         */
        std::string prefix_;
        /**
         * @brief This logger's level
         */
        ELogLevel level_;
        /**
         * @brief Parent's log level
         */
        std::atomic<ELogLevel>& currentLevel_;
        /**
         * @brief Parent's mutex
         */
        std::mutex& parentMutex;
    };
  public:
    logger error;
    logger warning;
    logger info;
    logger debug;
};