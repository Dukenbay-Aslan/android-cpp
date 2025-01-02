#include <utility>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

#include "manual.h"
#include "../../base/config/Config.h"

namespace NManualRequests {

/**
 * @brief Number of applications
 * run currently
 */
unsigned int size = 0;

/**
 * @brief Number of applications
 * to run simultaneously
 */
unsigned int maxSize = 4;

/**
 * @brief Mutual exclusion for
 * `NManualRequests::conditionVariable`
 */
std::mutex mutex;

/**
 * @brief Condition variable to
 * control the number of running applications
 */
std::condition_variable conditionVariable;

/**
 * @brief Default construct with
 * `TApplication::port_ = 0`,
 * `TApplication::state_ = UNKNOWN_APP_STATE`
 */
TApplication::TApplication()
    : state_(EAppState::UNKNOWN_APP_STATE)
    , port_(0)
{

}

/**
 * @brief Constructor
 * @param port Port to run on
 */
TApplication::TApplication(unsigned int port)
    : state_(EAppState::UNKNOWN_APP_STATE)
    , port_(port)
{

}

/**
 * @brief Set the port to run an application on
 * @param port Port to run on
 * @return `TApplication::state_ == UNKNOWN_APP_STATE`
 */
bool TApplication::operator()(unsigned int port) {
    if (state_ == EAppState::UNKNOWN_APP_STATE) {
        port_ = port;
        return true;
    }
    return false;
}

/**
 * @brief Start receiving requests
 */
void TApplication::run() {
    if (state_ == EAppState::RUN) {
        log.info <<
            "(TApplication::run) " <<
            "Application is already " <<
            "in run state\n";
        return;
    }
    if (size > maxSize) {
        state_ = EAppState::WAIT;
    }
    std::unique_lock<std::mutex> lock(mutex);
    conditionVariable.wait(lock, [this] {
        if (size > maxSize) {
            log.error <<
                "(TApplication::run) " <<
                "Error in running an application. Reason: " <<
                "Maximum size of running applications " <<
                "reached. Application on port " << port_ <<
                " not run. Waiting for any application to stop\n";
        }
        return (size <= maxSize);
    });
    state_ = EAppState::RUN;
    size++;
    lock.unlock();
    conditionVariable.notify_one();
    /* This method clears all default signal handlers
    of a crow::SimpleApp. When the SIGINT (^C) is hit
    at runtime that signal will be sent to
    signalHandler function in Main.cpp. */
    app_.signal_clear();
    /* Set crow's log level to WARNING */
    app_.loglevel(crow::LogLevel::WARNING);
    log.debug <<
        "app_.signal_clear()";
    app_
        .bindaddr(Config::ipHost())
        .multithreaded()
        .port(port_)
        .run();
}

/**
 * @brief Stop receiving requests
 */
void TApplication::stop() {
    if (state_ == EAppState::STOP) {
        log.info <<
            "(TApplication::stop) " <<
            "Application is already " <<
            "in stop state\n";
        return;
    }
    if (state_ != EAppState::RUN) {
        log.error <<
            "(TApplication::stop) " <<
            "Error in stopping " <<
            "an application. Reason: " <<
            "Application is not " <<
            "in run state\n";
        return;
    }
    state_ = EAppState::STOP;
    size--;
    app_.stop();
}

/**
 * @brief Enable SSL for an application
 * @param crt Path to `.crt` file
 * @param key Path to `.key` file
 */
void TApplication::addCertificate(const std::filesystem::path& crt,
        const std::filesystem::path& key) {
    app_.ssl_file(crt, key);
}

/**
 * @brief Enable SSL for an application
 * @param certificate Paths to `.crt` and `.key` files
 */
void TApplication::addCertificate(const std::pair<std::filesystem::path,
        std::filesystem::path>& certificate) {
    app_.ssl_file(certificate.first, certificate.second);
}

/**
 * @brief Get the state of an application
 * @return `RUN`, `STOP` or `UNKNOWN_APP_STATE`
 */
const EAppState& TApplication::state() const {
    return state_;
}

/**
 * @brief Get a port that the application is run on
 * @return `TApplication::port_`
 */
const unsigned int& TApplication::port() const {
    return port_;
}

} // namespace NManualRequests