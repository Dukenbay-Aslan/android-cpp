#pragma once

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
    NManualRequests::TApplication app;
    TQueue queue;
};