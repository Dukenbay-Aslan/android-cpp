#include <utility>
#include <functional>
#include <thread>
#include <crow.h>

#include "../action/action.h"
#include "SActions.h"
#include "../wsmanager/wsmanager.h"
#include "../../base/config/Config.h"

/**
 * @brief Constructor
 * @param port Port to run on
 */
SActions::SActions(unsigned int port)
    : log("SActions")
{
    application(port);

    application.addRoute(
        "/action",
        crow::HTTPMethod::POST,
        process
    );

    application.addWsRoute(
        "/ws",
        [this](crow::websocket::connection& connection) {
            manager.store(
                std::thread(
                    &sendActions,
                    std::ref(connection)
                ),
                connection
            );
        },
        [this](crow::websocket::connection& connection,
                const std::string& reason,
                uint16_t code) {
            log.info <<
                "(onclose) Connection closed. " <<
                "Reason: " << reason << std::endl;
            manager.remove(connection);
        }
    );
}

/**
 * @brief Start receiving actions
 */
void SActions::run() {
    application.addCertificate(
        Config::certificate()
    );
    application.run();
}

/**
 * @brief Process requested actions
 * @param request Request to parse to an action
 * @param response Response to fill and send to a client
 */
void SActions::process(const crow::request& request,
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
}

/**
 * @brief Send actions in
 * JSON format to a client
 * @param connection Connection with client
 */
void SActions::sendActions(crow::websocket::connection& connection) {
    while (!shutdownFlag) {
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
    shutdownFlag = true;
    queue->push(nullptr);
    manager.removeAll();
    application.stop();
}