#pragma once

#include <thread>
#include <unordered_map>
#include <crow.h>

/**
 * @brief Manage WebSocket APIs
 */
class WsManager {
  public:
    WsManager() = default;
    void store(std::thread&& thread,
        crow::websocket::connection& connection);
    void remove(crow::websocket::connection& connection);
    void removeAll();
    const bool& shutdownFlag(crow::websocket::connection& connection);
  private:
    /**
     * @brief Map pointer to connection
     * to thread working with that connection
     */
    std::unordered_map<
        crow::websocket::connection* ,
        std::thread> connRefsThreads;
    /**
     * @brief IP addresses to connections
     */
    std::unordered_map<
        std::string,
        crow::websocket::connection* > ipConns;
    /**
     * @brief Shutdown flags of connections.
     * Also used to
     * notify `store()` when
     * `remove()` has completed
     */
    std::unordered_map<
        crow::websocket::connection* ,
        bool> shutdownFlags;
};