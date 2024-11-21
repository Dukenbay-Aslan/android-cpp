#include <utility>

#include <crow.h>

#include "../action/action.h"
#include "SActions.h"

/**
 * @brief Constructor
 * @param port Port to run on
 */
SActions::SActions(unsigned int port)
{
    application(8080);
    CROW_ROUTE(application, "/actions")
    ([](const crow::request& request,
            crow::response& response) {
        NAction::TAction action(std::move(request));
        if (action.empty()) {
            response.code = 400;
            response.body = "Bad JSON";
            response.end();
            return;
        }
        response.code = 200;
        response.body = "Received an action";
        response.end();
        return;
    });

    CROW_WEBSOCKET_ROUTE(application, "/ws")
        .onopen([&](crow::websocket::connection& connection) {

                })
        .onclose([&](crow::websocket::connection& connection,
                    const std::string& reason,
                    uint16_t code) {

                })
        .onaccept([&](const crow::request& request,
                    void** data) {
                return true/false;
                })
        .onmessage([&](crow::websocket::connection& connection,
                    const std::string& message,
                    bool isBinary) {

                })
        .onerror([&](crow::websocket::connection& connection,
                    const std::string& error) {

                });
}

/**
 * @brief Start receiving actions
 */
void SActions::run() {
    application.run();
}

/**
 * @brief Stop receiving actions
 */
void SActions::shutdown() {
    application.stop();
}