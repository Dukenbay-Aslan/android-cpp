#pragma once

#include <string_view>
/*
#include <cpr.h>
#include <crow/crow.h>
#include <nlohmann/json.hpp>
*/

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
    SIZE_APP_STATE,
    UNKNOWN_APP_STATE
};

/**
 * @brief Application to receive manual requests
 */
class Application {
  public:
    Application();
    Application(std::string_view endpoint, unsigned int port);

    void run();
    void stop();

    // Member getters
    const std::string& endpoint();
    const unsigned int& port();

    /**
     * @brief State of an application
     */
    EAppState state = EAppState::STOP;
  private:
    /**
     * @brief Underlying application
     */
    crow::SimpleApp app_;

    /**
     * @brief Endpoint of an application
     */
    std::string endpoint_;
    unsigned int port_;

    bool operator==(const Application& other) const;
};

void addApp(std::string_view endpoint, unsigned int port);
void runApp(Application& app);
void stopApp(Application& app);
void removeApp(Application& app);

void run();
void stop();

} // namespace NManualRequests