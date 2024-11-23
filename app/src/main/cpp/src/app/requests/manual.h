#pragma once

#include <stdexcept>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <crow.h>

/**
 * @brief Receive manual requests from a client
 */
namespace NManualRequests {

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