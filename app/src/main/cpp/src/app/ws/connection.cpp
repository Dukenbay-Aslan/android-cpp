#include "connection.h"

namespace Ws {

/**
 * @brief Default constructor
 */
Connection::Connection()
    : ip_()
    , conn_()
    , thread_()
    , shutdownFlag_()
    , queue_()
{

}

/**
 * @brief Constructor
 * @param ip IP address of the issuer
 * @param conn WebSocket connection
 * @param thread Worker thread
 * @param shutdownFlag Shut down trigger
 * @param queue Queue for worker thread
 */
Connection::Connection(const std::string& ip,
        crow::websocket::connection& conn,
        std::thread&& thread,
        bool& shutdownFlag,
        std::shared_ptr<
            TQueue<
            std::unique_ptr<
            NAction::TAction>>> queue)
    : ip_(ip)
    , conn_(&conn)
    , thread_(std::move(thread))
    , shutdownFlag_(&shutdownFlag)
    , queue_(queue)
{

}

/**
 * @brief Assigning operator with moving
 * @param other Connection to assign
 * @return Moved connection
 */
Connection& Connection::operator=(Connection&& other) {
    Ws::log.debug
        << "(Connection::operator=) this->ip_ = std::move(other.ip_);";
    this->ip_ = std::move(other.ip_);
    Ws::log.debug
        << "(Connection::operator=) this->ip_ = std::move(other.ip_); -- done";
    Ws::log.debug
        << "(Connection::operator=) this->conn_ = std::move(other.conn_);";
    this->conn_ = std::move(other.conn_);
    Ws::log.debug
        << "(Connection::operator=) this->conn_ = std::move(other.conn_); -- done";
    Ws::log.debug
        << "(Connection::operator=) this->queue_ = std::move(other.queue_);";
    this->queue_ = std::move(other.queue_);
    Ws::log.debug
        << "(Connection::operator=) this->queue_ = std::move(other.queue_); -- done";
    Ws::log.debug
        << "(Connection::operator=) this->thread_ = std::move(other.thread_);";
    this->thread_ = std::move(other.thread_);
    Ws::log.debug
        << "(Connection::operator=) this->thread_ = std::move(other.thread_); -- done";
    Ws::log.debug
        << "(Connection::operator=) this->shutdownFlag_ = std::move(other.shutdownFlag_);";
    this->shutdownFlag_ = std::move(other.shutdownFlag_);
    Ws::log.debug
        << "(Connection::operator=) this->shutdownFlag_ = std::move(other.shutdownFlag_); -- done";
    return *this;
}

/**
 * @brief If the worker thread
 * is joinable:
 * - Set `shutdownFlag_ = true`
 * - Push `nullptr` to `queue_`
 * - Join the `thread_`
 * - Close the `conn_`
 */
void Connection::close() {
    Ws::log.debug
        << "(Connection::close) if (thread_.joinable()) {";
    if (thread_.joinable()) {
        Ws::log.debug
            << "(Connection::close) *shutdownFlag_ = true;";
        *shutdownFlag_ = true;
        Ws::log.debug
            << "(Connection::close) *shutdownFlag_ = true; -- done";
        Ws::log.debug
            << "(Connection::close) queue_->push(nullptr);";
        queue_->push(nullptr);
        Ws::log.debug
            << "(Connection::close) queue_->push(nullptr); -- done";
        Ws::log.debug
            << "(Connection::close) thread_.join();";
        thread_.join();
        Ws::log.debug
            << "(Connection::close) thread_.join(); -- done";
        // Ws::log.debug
        //     << "(Connection::close) conn_->close();";
        // conn_->close();
        // Ws::log.debug
        //     << "(Connection::close) conn_->close(); -- done";
    }
    Ws::log.debug
        << "(Connection::close) if (thread_.joinable()) { -- done";
}

/**
 * @brief Check if the connection's
 * thread is joinable
 * @return `Connection::thread_::joinable`
 */
bool Connection::joinable() {
    return thread_.joinable();
}

} // namespace Ws