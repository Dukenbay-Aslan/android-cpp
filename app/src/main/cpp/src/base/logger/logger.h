#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <iostream>

#include "../helpers/utils.h"
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
        
        /**
         * @brief Log a message
         * @tparam T Data type of a message
         * @param message Message to log
         * @return Concatenated logs
         */
        template<typename T>
        logger& operator<<(const T& message) {
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
        logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
            if (level_ <= currentLevel_.load()) {
                std::lock_guard<std::mutex> lock(parentMutex);
                std::cout << manip;
            }
            return (*this);
        }
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