#pragma once

#include <filesystem>
#include <tuple>
#include <string>
#include <crow.h>

#include "../../base/logger/logger.h"

/**
 * @brief Receive manual requests from a client
 */
namespace NManualRequests {

/**
 * @brief Messages logger
 */
TLogger log("NManualRequests");

/**
 * @brief function to apply
 * when a connection is closed.
 */
const auto defaultOnClose =
    [](crow::websocket::connection&,
        const std::string& reason,
        uint16_t) {
    log.info <<
        "(onclose) Connection closed. " <<
        "Reason: " << reason << std::endl;
};

/**
 * @brief Default function to apply 
 * when a client sends a message to server.
 */
const auto defaultOnMessage =
    [](crow::websocket::connection& connection,
        const std::string& message,
        bool isBinary) {
    if (isBinary) {
        connection.send_binary(message);
    } else {
        connection.send_text(message);
    }
    return;
};

/**
 * @brief function to apply
 * when an error occurs in connection.
 */
const auto defaultOnError =
    [](crow::websocket::connection&,
        const std::string& error) {
    log.error <<
        "(onerror) Connection error: " <<
        error << std::endl;
    return;
};

/**
 * @brief function to apply
 * when a client sends a request.
 */
const auto defaultOnAccept =
    [](const crow::request&,
        void**) {
    return true;
};

/**
 * @brief State of an application
 */
enum EAppState {
    RUN = 0,
    STOP,
    WAIT,
    SIZE_APP_STATE,
    UNKNOWN_APP_STATE,
};

/**
 * @brief Application to receive manual requests
 */
class TApplication {
  public:
    TApplication();
    TApplication(unsigned int port);
    bool operator()(unsigned int port);

    void run();
    void stop();

    template<typename Function>
    void addRoute(const std::string& endpoint,
        crow::HTTPMethod method,
        Function&& function);

    template<typename OnOpen,
        typename OnClose
            = decltype(NManualRequests::defaultOnClose),
        typename OnMessage
            = decltype(NManualRequests::defaultOnMessage),
        typename OnError
            = decltype(NManualRequests::defaultOnError),
        typename OnAccept
            = decltype(NManualRequests::defaultOnAccept)>
    void addWsRoute(const std::string& endpoint,
        OnOpen&& onOpen,
        OnClose&& onClose
            = NManualRequests::defaultOnClose,
        OnMessage&& onMessage
            = NManualRequests::defaultOnMessage,
        OnError&& onError
            = NManualRequests::defaultOnError,
        OnAccept&& onAccept
            = NManualRequests::defaultOnAccept);
    
    void addCertificate(const std::filesystem::path& crt,
        const std::filesystem::path& key);
    void addCertificate(const std::pair<std::filesystem::path,
        std::filesystem::path>& certificate);

    // Member getters
    const EAppState& state() const;
    const unsigned int& port() const;

    /**
     * @brief State of an application
     */
    EAppState state_ = EAppState::UNKNOWN_APP_STATE;
  private:
    /**
     * @brief Port that an application
     * is run on
     */
    unsigned int port_ = 0;
    /**
     * @brief Underlying application
     */
    crow::SimpleApp app_;
};

} // namespace NManualRequests