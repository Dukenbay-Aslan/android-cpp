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
    app(8080);
    app.addRoute(
        "/actions",
        crow::HTTPMethod::POST,
        [](const crow::request& request,
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
}

/**
 * @brief Start receiving actions
 */
void SActions::run() {
    app.run();
}

/**
 * @brief Stop receiving actions
 */
void SActions::shutdown() {
    app.stop();
}