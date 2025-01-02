#pragma once

#include "ws.h"

namespace Ws {

/**
 * @brief WebSocket connection
 */
class Connection {
  public:
    Connection();
    Connection(const std::string& ip,
        crow::websocket::connection& conn,
        std::thread&& thread,
        bool& shutdownFlag,
        std::shared_ptr<
            TQueue<
            std::unique_ptr<
            NAction::TAction>>> queue);
    Connection& operator=(Connection&& other);
    void close();
    bool joinable();
  private:
    /**
     * @brief IP address of the
     * WebSocket connection issuer
     */
    std::string ip_;
    /**
     * @brief Pointer to WebSocket
     * connection that the worker
     * thread works with
     */
    crow::websocket::connection* conn_;
    /**
     * @brief Worker thread
     */
    std::thread thread_;
    /**
     * @brief Pointer to trigger to:
     * - Close the connection
     * - Push `nullptr` to queue
     * - Join the worker thread
     */
    bool* shutdownFlag_;
    /**
     * @brief Queue that the
     * worker thread works with
     */
    std::shared_ptr<
        TQueue<
        std::unique_ptr<
        NAction::TAction>>> queue_;
};

} // namespace Ws