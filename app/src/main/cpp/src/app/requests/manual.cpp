#include <mutex>
#include <condition_variable>
#include <unordered_map>

#include "manual.h"
#include "../../base/config/Config.h"
#include "../../base/logger/logger.h"

namespace NManualRequests {

/**
 * @brief Messages logger
 */
TLogger log("NManualRequests");

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
    : port_(0)
    , state_(EAppState::UNKNOWN_APP_STATE)
{

}

/**
 * @brief Constructor
 * @param port Port to run on
 */
TApplication::TApplication(unsigned int port)
    : port_(port)
    , state_(EAppState::UNKNOWN_APP_STATE)
{
    /* This method clears all default signal handlers
    of a crow::SimpleApp. When the SIGINT (^C) is hit
    at runtime that signal will be sent to
    signalHandler function in Main.cpp. */
    app_.signal_clear();
}

/**
 * @brief Set the port to run an application on
 * @param port Port to run on
 * @return `TApplication::state_ != UNKNOWN_APP_STATE`
 */
bool TApplication::operator()(unsigned int port) {
    if (state_ != EAppState::UNKNOWN_APP_STATE) {
        /* This method clears all default signal handlers
        of a crow::SimpleApp. When the SIGINT (^C) is hit
        at runtime that signal will be sent to
        signalHandler function in Main.cpp. */
        app_.signal_clear();
        port_ = port;
        return true;
    }
    return false;
}

/**
 * @brief Start receiving requests
 */
void TApplication::run() {
    if (size > maxSize) {
        state_ = EAppState::WAIT;
    }
    std::unique_lock<std::mutex> lock(mutex);
    conditionVariable.wait(lock, [this] {
        log.error << "(TApplication::run) " <<
            "Maximum size of running applications has been " <<
            "reached. Application for port " << port_ <<
            " not run. Waiting for any application to be stopped";
        return (size <= maxSize);
    });
    size++;
    state_ = EAppState::RUN;
    app_.signal_clear();
    lock.unlock();
    conditionVariable.notify_one();
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
    state_ = EAppState::STOP;
    size--;
    app_.stop();
}

/**
 * @brief Add a route to receive requests
 * @param endpoint Endpoint of a route
 * @param method HTTP method
 * @param function Function to apply on requests
 * @warning `function` needs to take `const crow::request&`,
 * `crow::response&` and return `void`
 */
void TApplication::addRoute(const std::string& endpoint,
        crow::HTTPMethod method,
        const std::function<
            void(const crow::request&,
                crow::response&)>& function) {
    app_.route_dynamic(endpoint)
        .methods(method)
        ([function](const crow::request& request,
                crow::response& response) {
            function(request, response);
        });
    endpoints_.push_back(endpoint);
}

/**
 * @brief Get the state of an application
 * @return `RUN`, `STOP` or `UNKNOWN_APP_STATE`
 */
const EAppState TApplication::state() const {
    return state_;
}

/**
 * @brief Get endpoints of an application
 */
const std::vector<std::string>& TApplication::endpoints() const {
    return endpoints_;
}

/**
 * @brief Get a port that the application runs on
 */
const unsigned int& TApplication::port() const {
    return port_;
}

} // namespace NManualRequests