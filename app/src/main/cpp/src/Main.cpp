#include <signal.h>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <utility>
#include <thread>
#include <filesystem>
#include <vector>
#include <iostream>

#include "app/services/SActions.h"
#include "base/config/Config.h"
#include "base/logger/logger.h"
#include "base/IService.h"

/**
 * @brief 24 hours
 */
const auto oneDay = std::chrono::hours(24);
/**
 * @brief Flag to signal
 * all services to shut down
 */
std::atomic<bool> shutdownFlag = false;
/**
 * @brief Mutual exclusion for
 * `shutdownCv`
 */
std::mutex shutdownMutex;
/**
 * @brief Condition variable to
 * control shut down of services
 */
std::condition_variable shutdownCv;

/**
 * @brief Catch signals and
 * notify services to shut down
 * @param sig Signal received
 */
void signalHandler(int sig) {
    shutdownFlag = true;
}

int main(int argc, char *argv[]) {
    /**
     * @brief Messages logger
     */
    TLogger log("Main");
    if (argc != 2) {
        log.error << "Give a path to configuration file";
        return -1;
    }
    std::string configFilePath = argv[1];
    log.info <<
        "Parsing " <<
        configFilePath <<
        std::endl;
    if (!Config::parse(std::filesystem::path(configFilePath))) {
        log.error
            << "Error in parsing configurations. "
            << std::endl;
        return -1;
    }
    log.info
        << "Successfully parsed configurations: "
        << Config::json().dump(4)
        << std::endl;
    
    // Catching signals
    signal(
        SIGINT,
        signalHandler
    );
    signal(
        SIGTERM,
        signalHandler
    );
    signal(
        SIGABRT,
        signalHandler
    );

    std::vector<std::shared_ptr<IService>> services;
    auto actionsService = std::make_shared<SActions>(
        Config::portSActions()
    );

    /**
     * @brief `run` functions of services
     */
    std::vector<std::thread> threads;
    for (auto& service : services) {
        threads.push_back(std::thread(&IService::run, service));
    }

    /**
     * Blocking services here until
     * SIGINT received and
     * shutdownFlag set true.
     * Checking this condition
     * every 24 hours starting
     * from program start
     */
    {
        /**
         * @brief Lock mutex for condition variable
         */
        std::unique_lock<std::mutex> lock(shutdownMutex);
        shutdownCv.wait_for(
            lock,
            oneDay,
            [&]() {
                return shutdownFlag.load();
            }
        );
        lock.unlock();
        shutdownCv.notify_all();
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    log.info
        << "Joined all threads"
        << std::endl;

    return 0;
}