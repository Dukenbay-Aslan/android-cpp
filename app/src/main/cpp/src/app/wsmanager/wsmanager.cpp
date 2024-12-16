#include "wsmanager.h"

/**
 * @brief Store the pair of
 * thread and connection
 * @param thread Worker thread
 * @param connection Connection
 * that the thread is working with
 */
void WsManager::store(std::thread&& thread,
        crow::websocket::connection& connection) {
    connRefsThreads[&connection] = std::move(thread);
    std::cout <<
        "Assigned thread\n";
    shutdownFlags[&connection] = false;
    std::cout <<
        "shutdownFlag = false\n";
    ipConns[connection.get_remote_ip()] = &connection;
    std::cout <<
        "Assigned connection to ip\n";
}

/**
 * @brief Join the thread
 * working with connection.
 * Remove their pair from memory
 * @param connection Closed connection
 */
void WsManager::remove(crow::websocket::connection& connection) {
    auto& thread = connRefsThreads[&connection];
    if (thread.joinable()) {
        thread.join();
    }
}

/**
 * @brief Join all threads.
 * Clear memory
 */
void WsManager::removeAll() {
    for (auto& [connRef, thread] : connRefsThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

/**
 * @brief Get the shutdown flag
 * of the connection
 * @param connection WebSocket connection
 * @return `WsManager::shutdownFlags[&connection]`
 */
const bool& WsManager::shutdownFlag(crow::websocket::connection& connection) {
    return shutdownFlags[&connection];
}