#include <fstream>

#include "Config.h"
#include "../helpers/mappers.h"
#include "../helpers/valid.h"

namespace Config {

/**
 * @brief Helper constant values
 */
namespace constants {

/**
 * @brief JSON key for an IP address
 * of the host to run all applications on
 */
const std::string KEY_IP_HOST = "ip_host";
/**
 * @brief JSON key for a port
 * to run the service `SActions` on
 */
const std::string KEY_PORT_SACTIONS = "port_sactions";
/**
 * @brief JSON key for a general
 * logging level
 */
const std::string KEY_LOG_LEVEL = "log_level";

} // namespace constants

/**
 * @brief Messages logger
 */
TLogger log("Config");

// Private members
/**
 * @brief Configurations JSON
 */
nlohmann::json json_ = nlohmann::json::object();
/**
 * @brief IP address of the host
 * to run all applications on
 */
std::string ipHost_ = "";
/**
 * @brief Port to run the service
 * `SActions` on
 */
unsigned int portSActions_ = -1;
/**
 * @brief General logging level
 */
ELogLevel logLevel_ = ELogLevel::DEBUG;

/**
 * @brief Parse configurations
 * @param path Path to configurations file
 */
bool parse(const std::filesystem::path& path) {
    // Parsing a file into JSON
    std::ifstream file(path); // Config file
    if (!file.is_open()) {
        log.error << "(parse) " <<
            "Error in parsing configurations. " <<
            "Reason: Can not open file: " <<
            path << '\n';
        return false;
    }
    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    if (file.bad()) {
        log.error << "(parse) " <<
            "Error in parsing configurations. " <<
            "Reason: File is bad: " <<
            path << '\n';
    }
    json_ = nlohmann::json::parse(content);

    if (json_.contains(Config::constants::KEY_IP_HOST)) {
        ipHost_ = json_[Config::constants::KEY_IP_HOST].get<std::string>();
        if (!valid::ip(ipHost_)) {
            ipHost_ = "";
            return false;
        }
    } else {
        log.error << "(parse) " <<
            "Error in parseing configurations. " <<
            "Reason: Can not find ip_host key\n";
        return false;
    }
    if (json_.contains(Config::constants::KEY_PORT_SACTIONS)) {
        portSActions_ = json_[Config::constants::KEY_PORT_SACTIONS].get<unsigned int>();
        if (!valid::port(portSActions_)) {
            portSActions_ = -1;
            return false;
        }
    } else {
        portSActions_ = 8080;
    }

    if (json_.contains(Config::constants::KEY_LOG_LEVEL)) {
        auto logLevelStr = json_[Config::constants::KEY_LOG_LEVEL].get<std::string>();
        logLevel_ = mappers::fromString::logLevel(logLevelStr);
    } else {
        logLevel_ = ELogLevel::DEBUG;
    }
    return true;
}

/**
 * @brief Get the configurations JSON
 * @return `Config::json_`
 */
nlohmann::json json() {
    return json_;
}

/**
 * @brief Get the IP address of the
 * host to run all applications on
 * @return `Config::ipHost_`
 */
std::string ipHost() {
    return ipHost_;
}

/**
 * @brief Get the port to run
 * the service `SActions` on
 * @return `Config::portSActions_`
 */
unsigned int portSActions() {
    return portSActions_;
}

/**
 * @brief Get the general logging level
 * @return `Config::logLevel_`
 */
ELogLevel logLevel() {
    return logLevel_;
}

} // namespace Config