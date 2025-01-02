#pragma once

#include "connection.h"
#include "map.h"
#include "ws.h"

namespace Ws {

/**
 * @brief Manage WebSocket APIs
 */
class Manager {
  public:
    void store(const std::string& ip,
        crow::websocket::connection& conn,
        std::thread&& thread,
        bool& shutdownFlag,
        std::shared_ptr<
            TQueue<
            std::unique_ptr<
            NAction::TAction>>> queue);
    void remove(crow::websocket::connection& connection);
    void removeAll();
    const bool& shutdownFlag(crow::websocket::connection& connection);
  private:
    /**
     * @brief WebSocket connections
     */
    Ws::TwoMap<
        crow::websocket::connection* ,
        Ws::Connection> conns;
    /**
     * @brief Map IPs
     * to connections
     */
    std::unordered_map<
        std::string,
        crow::websocket::connection* > ipConns;
    /**
     * @brief Map connections
     * to IPs
     */
    std::unordered_map<
        crow::websocket::connection* ,
        std::string> connIps;
    /**
     * @brief Shutdown flags of connections.
     * Also used to
     * notify `store()` when
     * `remove()` has completed
     */
    std::unordered_map<
        crow::websocket::connection* ,
        bool> shutdownFlags;
    /**
     * @brief Check if the connection
     * is duplicate to go back from
     * `Manager::remove` to `Manager::store`
     */
    std::unordered_map<
        std::string,
        bool> isDuplicate;
};

} // namespace Ws