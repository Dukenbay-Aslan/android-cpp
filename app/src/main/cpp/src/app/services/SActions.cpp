#include <utility>
#include <functional>
#include <thread>
#include <crow.h>

#include "../action/action.h"
#include "SActions.h"

/**
 * @brief Constructor
 * @param port Port to run on
 */
SActions::SActions(unsigned int port)
{
    application(port);

    application.addRoute(
        "/action",
        crow::HTTPMethod::POST,
        process
    );

    application.addWsRoute(
        "/ws",
        [](crow::websocket::connection& connection) {
            std::thread(
                &sendActions,
                std::ref(connection)
            ).detach();
        }
    );
}

/**
 * @brief Start receiving actions
 */
void SActions::run() {
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
    application.stop();
}