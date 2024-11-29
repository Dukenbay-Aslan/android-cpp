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
    /* This method clears all default signal handlers
    of a crow::SimpleApp. When the SIGINT (^C) is hit
    at runtime that signal will be sent to
    signalHandler function in Main.cpp. */
    app_.signal_clear();
}

/**
 * @brief Set the port to run an application on
 * @param port Port to run on
 * @return `TApplication::state_ == UNKNOWN_APP_STATE`
 */
bool TApplication::operator()(unsigned int port) {
    if (state_ == EAppState::UNKNOWN_APP_STATE) {
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
        log.error <<
            "(TApplication::run) " <<
            "Error in running an application. Reason: " <<
            "Maximum size of running applications " <<
            "reached. Application on port " << port_ <<
            " not run. Waiting for any application to stop\n";
        return (size <= maxSize);
    });
    state_ = EAppState::RUN;
    size++;
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
 * @brief Add a route to receive requests
 * @tparam Function Functional type
 * @param endpoint Endpoint of a route
 * @param method HTTP method
 * @param function Function to apply on requests
 */
template<typename Function>
void TApplication::addRoute(const std::string& endpoint,
        crow::HTTPMethod method,
        Function&& function) {
    app_
        .route_dynamic(endpoint)
        .methods(method)
        (std::forward<Function>(function));
}

/**
 * @brief Add a WebSocket route
 * to establish a connection.
 * @tparam OnOpen Callable type.
 * @tparam OnMessage Callable type.
 * @tparam OnClose Callable type.
 * @tparam OnError Callable type.
 * @tparam OnAccept Callable type.
 * @param endpoint Endpoint of a route.
 * @param onOpen Function to apply
 * when a connection is established.
 * @param onClose Function to apply
 * when a connection is closed.
 * Defaults to logging the closing reason.
 * @param onMessage Function to apply
 * when a client sends a message to server.
 * Defaults to logging the client message.
 * @param onError Function to apply
 * when an error occurs in connection.
 * Defaults to logging the error message.
 * @param onAccept Function to apply
 * when a client sends a request.
 * Defaults to `return true`.
 * @warning Closed connection is passed
 * to `onClose` and `onError` functions.
 */
template<typename OnOpen,
    typename OnClose,
    typename OnMessage,
    typename OnError,
    typename OnAccept>
void TApplication::addWsRoute(const std::string& endpoint,
        OnOpen&& onOpen,
        OnClose&& onClose,
        OnMessage&& onMessage,
        OnError&& onError,
        OnAccept&& onAccept) {
    app_
        .route_dynamic(endpoint)
        .websocket()
        .onopen(std::forward<OnOpen>(onOpen))
        .onclose(std::forward<OnClose>(onClose))
        .onmessage(std::forward<OnMessage>(onMessage))
        .onerror(std::forward<OnError>(onError))
        .onaccept(std::forward<OnAccept>(onAccept));
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