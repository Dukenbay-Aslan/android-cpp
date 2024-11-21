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
 * @brief Application to receive manual requests.
 * Extended `crow::SimpleApp` with tracking endpoints.
 * @throws Runtime error when an endpoint is added twice
 */
class TApplication : public crow::SimpleApp {
  public:
    TApplication();
    TApplication(unsigned int port);
    bool operator()(unsigned int port);

    void run();
    void stop();

    // Member getters
    const EAppState& state() const;
    const std::unordered_set<std::string>& endpoints() const;
    const unsigned int& port() const;

    /**
     * @brief Track added endpoints on call
     * of `CROW_ROUTE`, `CROW_WEBSOCKET_ROUTE`
     * @tparam ...Args
     * @param endpoint Endpoint of a new route 
     * @return Extended `crow::SimpleApp::route()`
     */
    template<typename... Args>
    auto route(const std::string& endpoint,
            Args&&... args) {
        // If endpoint is already registered
        if (endpoints_.find(endpoint) != endpoints_.end()) {
            log.error
                << "Error in adding endpoint to "
                << "an application. Reason: Endpoint "
                << "already exists: "
                << endpoint
                << std::endl;
            throw std::runtime_error(
                "Endpoint already exists: " + endpoint
            );
        }
        endpoints_.insert(endpoint);
        return app_.route(
                    endpoint,
                    std::forward<Args>(args)...
                );

    }

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
     * @brief Endpoints of an application
     */    
    std::unordered_set<std::string> endpoints_ = {};
    /**
     * @brief Underlying application
     */
    crow::SimpleApp app_;
};

} // namespace NManualRequests