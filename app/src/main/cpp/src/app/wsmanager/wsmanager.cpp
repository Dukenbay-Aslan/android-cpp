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