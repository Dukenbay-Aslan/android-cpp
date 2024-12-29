#include <utility>
#include <functional>
#include <thread>
#include <crow.h>

#include "../action/action.h"
#include "SActions.h"
#include "../../base/config/Config.h"

/**
 * @brief Constructor
 * @param port Port to run on
 */
SActions::SActions(unsigned int port)
    : log("SActions")
{
    if (!application(port)) {
        log.error <<
            "Error in attaching a port to an application." <<
            "Reason: Application state != UNKNOWN_APP_STATE";
        return; 
    }
    // Initialize queue of actions
    queue = std::make_shared<
        TQueue<
            std::unique_ptr<
                NAction::TAction>>>();
    // Receive HTTP/HTTPS POST requests
    CROW_ROUTE(application.app_, "/action")
        .methods(crow::HTTPMethod::Post)
        ([this](const crow::request& request,
                crow::response& response) {
            NAction::TAction action(std::move(request));
            if (action.empty()) {
                response.code = crow::status::BAD_REQUEST;
                response.body = "Bad JSON";
                response.end();
                return;
            }
            queue->push(
                std::make_unique
                    <NAction::TAction>(action)
            );
            response.code = crow::status::OK;
            response.body = "Received an action";
            response.end();
            return;
        });

    // Support WebSocket (Secure) connections
    CROW_WEBSOCKET_ROUTE(application.app_, "/ws")
        .onopen([this](crow::websocket::connection& connection) {
            log.info <<
                "(onopen) New WebSocket connection from " <<
                connection.get_remote_ip();
            manager.store(
                connection.get_remote_ip(),
                connection,
                std::thread(
                    &SActions::sendActions,
                    this,
                    std::ref(connection)
                ),
                shutdownFlag,
                queue
            );
        })
        .onclose([this](crow::websocket::connection& connection,
                const std::string& reason,
                uint16_t code) {
            log.info
                << "(onclose) Connection closed. "
                << "Reason: " << reason
                << "Code: " << code;
            manager.remove(
                connection
            );
        })
        .onmessage(NManualRequests::defaultOnMessage)
        .onerror(NManualRequests::defaultOnError)
        .onaccept(NManualRequests::defaultOnAccept);
    log.info <<
        "Service initialized";
}

/**
 * @brief Start receiving actions
 */
void SActions::run() {
    log.info <<
        "Service started";
    application.addCertificate(
        Config::certificate()
    );
    application.run();
}

/**
 * @brief Send actions in
 * JSON format to a client
 * @param connection Connection with client
 */
void SActions::sendActions(crow::websocket::connection& connection) {
    while (!manager.shutdownFlag(connection) and !shutdownFlag) {
        auto action = queue->pop();
        if (action == nullptr) {
            continue;
        }
        connection.send_text(
            action
                ->json().dump(4)
        );
    }
}

/**
 * @brief Stop receiving actions
 */
void SActions::shutdown() {
    log.info <<
        "Shutting down service...";
    shutdownFlag = true;
    log.info <<
        "Set shutdown flag = true";
    manager.removeAll();
    log.info <<
        "Removed all websocket connections";
    application.stop();
    log.info <<
        "application stopped";
    log.info <<
        "Service successfully shut down";
}