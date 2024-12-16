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
    if (ipConns.count(connection.get_remote_ip())) {
        std::cout <<
            "Found existing connection from " <<
            connection.get_remote_ip() <<
            std::endl;
        ipConns[connection.get_remote_ip()]->close();
        std::cout <<
            "Closed connection\n";
        ipConns.erase(connection.get_remote_ip());
        std::cout <<
            "Erased connection IP\n";
    }
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
void WsManager::remove(crow::websocket::connection& connection,
        std::shared_ptr<
            TQueue<
                std::unique_ptr<
                    NAction::TAction>>>& queue) {
    std::cout <<
        "(WsManager::remove) Removing connection...\n";
    std::cout <<
        "(WsManager::remove) Setting shutdownFlag = true\n";
    shutdownFlags[&connection] = true;
    std::cout <<
        "(WsManager::remove) Pushing nullptr to queue\n";
    queue->push(nullptr);
    std::cout <<
        "(WsManager::remove) Taking thread\n";
    auto& thread = connRefsThreads[&connection];
    std::cout <<
        "(WsManager::remove) Checking thread joinable\n";
    if (thread.joinable()) {
        std::cout <<
            "(WsManager::remove) Joining thread\n";
        thread.join();
        std::cout <<
            "(WsManager::remove) Joined thread\n";
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