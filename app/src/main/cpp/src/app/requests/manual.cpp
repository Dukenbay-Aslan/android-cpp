#include <mutex>
#include <condition_variable>
#include <unordered_map>

#include "manual.h"

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
 * @brief Constructor
 * @param port Port to run on
 */
Application::Application(unsigned int port)
    : port_(port)
    , state_(EAppState::STOP)
{

}

/**
 * @brief Start receiving requests
 */
void Application::run() {
    if (size > maxSize) {
        state_ = EAppState::WAIT;
    }
    std::unique_lock<std::mutex> lock(mutex);
    conditionVariable.wait(lock, [] {
        log::error << "(Application::Application) " << 
        "Maximum size of running applications has been " <<
        "reached. Application for port " << port_ << " not run. " <<
        "Waiting for any application to be stopped";
        return (size <= maxSize);
    });
    size++;
    state_ = EAppState::RUN;
    lock.unlock();
    conditionVariable.notify_one();
    app_
        .bindaddr(Config::ipAddress())
        .multithreaded()
        .port(port_)
        .run();
}

/**
 * @brief Stop receiving requests
 */
void Application::stop() {
    state_ = EAppState::STOP;
    size--;
    app_.stop();
}

/**
 * @brief Add a route to receive requests
 * @param endpoint Endpoint of a route
 * @param method HTTP method
 * @param function Function to apply on requests
 * @warning `function` needs to take `const crow::request&` and `crow::response`
 */
void Application::addRoute(const std::string& endpoint,
        crow::HTTPMethod method,
        const std::function<void(const crow::request&, crow::response&)>& function) {
    CROW_ROUTE(app_, endpoint)
    .methods(method)
    ([function](const crow::request& request, crow::response& response) {
        function(request, response);
    });
    endpoints_.push_back(endpoint);
}

/**
 * @brief Get the state of an application
 * @return `RUN`, `STOP` or `UNKNOWN_APP_STATE`
 */
const EAppState Application::state() const {
    return state_;
}

/**
 * @brief Get endpoints of an application
 */
const std::vector<std::string>& Application::endpoints() const {
    return endpoints_;
}

/**
 * @brief Get a port that the application runs on
 */
const unsigned int& Application::port() const {
    return port_;
}

} // namespace NManualRequests