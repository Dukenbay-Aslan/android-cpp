#include "manager.h"

namespace Ws {

/**
 * @brief Store the pair of
 * thread and connection
 * @param thread Worker thread
 * @param connection Connection
 * that the thread is working with
 */
void Manager::store(const std::string& ip,
        crow::websocket::connection& conn,
        std::thread&& thread,
        bool& shutdownFlag,
        std::shared_ptr<
            TQueue<
            std::unique_ptr<
            NAction::TAction>>> queue) {
    Ws::log.debug
        << "(Manager::store) if (ipConns.count(ip)) {";
    if (ipConns.count(ip)) {
        Ws::log.debug
            << "(Manager::store) isDuplicate[ip] = true;";
        isDuplicate[ip] = true;
        Ws::log.debug
            << "(Manager::store) isDuplicate[ip] = true; -- done";
        Ws::log.debug
            << "(Manager::store) ipConns[ip]->close();";
        ipConns[ip]->close();
        Ws::log.debug
            << "(Manager::store) ipConns[ip]->close(); -- done";
        Ws::log.debug
            << "(Manager::store) conns.second(&conn) = Ws::Connection(";
        conns.second(&conn) = Ws::Connection(
            ip,
            conn,
            std::move(thread),
            shutdownFlag,
            queue
        );
        Ws::log.debug
            << "(Manager::store) conns.second(&conn) = Ws::Connection( -- done";
        Ws::log.debug
            << "(Manager::store) if (ipConns.count(ip)) { -- done";
        return;
    }
    Ws::log.debug
        << "(Manager::store) if (ipConns.count(ip)) { -- done";
    Ws::log.debug
        << "(Manager::store) ipConns[ip] = &conn;";
    ipConns[ip] = &conn;
    Ws::log.debug
        << "(Manager::store) ipConns[ip] = &conn; -- done";
    Ws::log.debug
        << "(Manager::store) connIps[&conn] = ip;";
    connIps[&conn] = ip;
    Ws::log.debug
        << "(Manager::store) connIps[&conn] = ip; -- done";
    Ws::log.debug
        << "(Manager::store) conns.first(&conn) = Ws::Connection(";
    conns.first(&conn) = Ws::Connection(
        ip,
        conn,
        std::move(thread),
        shutdownFlag,
        queue
    );
    Ws::log.debug
        << "(Manager::store) conns.first(&conn) = Ws::Connection( -- done";
}

/**
 * @brief Join the thread
 * working with connection.
 * Remove their pair from memory
 * @param connection Closed connection
 */
void Manager::remove(crow::websocket::connection& connection) {
    Ws::log.debug
        << "(Manager::remove) Enter";
    if (!connIps.count(&connection)) {
        Ws::log.error
            << "Error in removing connection."
            << "Reason: Can not find";
        return;
    }
    Ws::log.debug
        << "(Manager::remove) auto ip = connIps[&connection];";
    auto ip = connIps[&connection];
    Ws::log.debug
        << "(Manager::remove) auto ip = connIps[&connection]; -- done";
    Ws::log.debug
        << "(Manager::remove) if (isDuplicate[ip]) {";
    if (isDuplicate[ip]) {
        Ws::log.debug
            << "(Manager::remove) isDuplicate[ip] = false;";
        isDuplicate[ip] = false;
        Ws::log.debug
            << "(Manager::remove) isDuplicate[ip] = false; -- done";
        Ws::log.debug
            << "(Manager::remove) conns.second(&connection).close();";
        conns.first(&connection).close();
        Ws::log.debug
            << "(Manager::remove) conns.second(&connection).close(); -- done";
        Ws::log.debug
            << "(Manager::remove) conns.swap(&connection);";
        conns.swap(&connection);
        Ws::log.debug
            << "(Manager::remove) conns.swap(&connection); -- done";
        return;
    }
    Ws::log.debug
        << "(Manager::remove) if (isDuplicate[ip]) { -- done";
    Ws::log.debug
        << "(Manager::remove) conns.first(&connection).close();";
    conns.first(&connection).close();
    Ws::log.debug
        << "(Manager::remove) conns.first(&connection).close(); -- done";
    Ws::log.debug
        << "(Manager::remove) conns.clear(&connection);";
    conns.clear(&connection);
    Ws::log.debug
        << "(Manager::remove) conns.clear(&connection); -- done";
    Ws::log.debug
        << "(Manager::remove) ipConns.erase(connIps[&connection]);";
    ipConns.erase(connIps[&connection]);
    Ws::log.debug
        << "(Manager::remove) ipConns.erase(connIps[&connection]); -- done";
    Ws::log.debug
        << "(Manager::remove) connIps.erase(&connection);";
    connIps.erase(&connection);
    Ws::log.debug
        << "(Manager::remove) connIps.erase(&connection); -- done";
}

/**
 * @brief Join all threads.
 * Clear memory
 */
void Manager::removeAll() {
    for (auto it = ipConns.begin();
         it != ipConns.end();
         /* iterator is changed
            in erase */) {
        it->second->close();
        conns.clear(it->second);
        connIps.erase(it->second);
        it = ipConns.erase(it);
    }
}

/**
 * @brief Get the shutdown flag
 * of the connection
 * @param connection WebSocket connection
 * @return `Manager::shutdownFlags[&connection]`
 */
const bool& Manager::shutdownFlag(crow::websocket::connection& connection) {
    return shutdownFlags[&connection];
}

} // namespace Ws