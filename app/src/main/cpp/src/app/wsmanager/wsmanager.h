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
  private:
    /**
     * @brief Map pointer to connection
     * to thread working with that connection
     */
    std::unordered_map<
        crow::websocket::connection* ,
        std::thread> connRefsThreads;
};