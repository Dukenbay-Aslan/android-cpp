#pragma once

#include <thread>
#include <unordered_map>
#include <crow.h>

#include "../action/action.h"
#include "../../base/logger/logger.h"
#include "../../base/Queue.h"

/**
 * @brief Namespace to work with
 * the WebSocket protocol
 */
namespace Ws {

/**
 * @brief Messages logger
 */
inline TLogger log{"Ws"};

/**
 * @brief Enhanced map with
 * storing, erasing and
 * swapping between two values
 * @tparam Key Key type
 * @tparam Value Value type
 */
template<class Key, class Value>
class TwoMap {
  public:
    Value& first(const Key& key);
    Value& second(const Key& key);
    void clear(const Key& key);
    void swap(const Key& key);
  private:
    /**
     * @brief First values
     */
    std::unordered_map<
        Key, Value> first_;
    /**
     * @brief Second values
     */
    std::unordered_map<
        Key, Value> second_;
};

/**
 * @brief Return the first value
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 * @return First value
 */
template<class Key, class Value>
Value& TwoMap<Key, Value>::first(const Key& key) {
    return first_[key];
}

/**
 * @brief Return the second value
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 * @return Second value
 */
template<class Key, class Value>
Value& TwoMap<Key, Value>::second(const Key& key) {
    return second_[key];
}

/**
 * @brief Erase all values
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 */
template<class Key, class Value>
void TwoMap<Key, Value>::clear(const Key& key) {
    first_.erase(key);
    second_.erase(key);
}

/**
 * @brief Swap values and
 * erase the second value
 * @tparam Key Key type
 * @tparam Value Value type
 * @param key Key
 */
template<class Key, class Value>
void TwoMap<Key, Value>::swap(const Key& key) {
    log.debug
        << "(TwoMap::swap) Enter";
    log.debug
        << "(TwoMap::swap) first_[key] = std::move(second_[key]);";
    first_[key] = std::move(second_[key]);
    log.debug
        << "(TwoMap::swap) first_[key] = std::move(second_[key]); -- done";
    log.debug
        << "(TwoMap::swap) second_.erase(key);";
    second_.erase(key);
    log.debug
        << "(TwoMap::swap) second_.erase(key); -- done";
    log.debug
        << "(TwoMap::swap) Exit";
}
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