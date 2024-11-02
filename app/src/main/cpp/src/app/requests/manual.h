#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <crow/crow.h>

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
};

/**
 * @brief Application to receive manual requests
 */
class Application {
  public:
    Application(unsigned int port);

    void run();
    void stop();

    void addRoute(const std::string& endpoint,
        crow::HTTPMethod method,
        const std::function<void(const crow::request&, crow::response&)>& function);

    // Member getters
    const EAppState state() const;
    const std::vector<std::string>& endpoints() const;
    const unsigned int& port() const;

    /**
     * @brief State of an application
     */
    EAppState state_ = EAppState::UNKNOWN_APP_STATE;
  private:
    /**
     * @brief Underlying application
     */
    crow::SimpleApp app_;

    /**
     * @brief Endpoint of an application
     */
    std::vector<std::string> endpoints_ = {};
    unsigned int port_ = 0;
};

} // namespace NManualRequests