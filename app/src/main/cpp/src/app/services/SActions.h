#pragma once

#include <memory>
#include <crow.h>

#include "../action/action.h"
#include "../requests/manual.h"
#include "../../base/IService.h"
#include "../../base/Queue.h"

/**
 * @brief Service to manage client actions:
 * - `/action`: API to receive an action
 * - `/ws`: WebSocket connection to see
 * received actions in real time
 */
class SActions : public IService {
  public:
    SActions(unsigned int port);

    void run() override;
    void shutdown() override;
  private:
    void process(const crow::request& request,
        crow::response& response);
    void sendActions(crow::websocket::connection& connection);
    /**
     * @brief Application to receive actions
     */
    NManualRequests::TApplication application;
    /**
     * @brief Queue of requested actions
     */
    std::shared_ptr<
        TQueue<
        std::unique_ptr<
        NAction::TAction>>> queue;
};