#pragma once

#include <memory>

#include "../action/action.h"
#include "../requests/manual.h"
#include "../../base/IService.h"
#include "../../base/Queue.h"

class SActions : public IService {
  public:
    SActions(unsigned int port);

    void process(const crow::request& request,
        crow::response& response);

    void run() override;
    void shutdown() override;
  private:
    /**
     * @brief HTTP/HTTPS application
     * to receive actions
     */
    NManualRequests::TApplication app;
    /**
     * @brief Queue of requested actions
     */
    std::shared_ptr<
        TQueue<
        std::unique_ptr<
        NAction::TAction>>> queue;
};