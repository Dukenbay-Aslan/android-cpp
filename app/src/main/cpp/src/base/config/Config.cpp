#include <utility>
#include <fstream>

#include "Config.h"
#include "../helpers/mappers.h"
#include "../helpers/valid.h"
#include "../logger/logger.h"

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
 * @brief JSON key for the paths to
 * self-signed certificate
 * and its key
 */
const std::string KEY_PATHS_CERTIFICATE = "certificate";
/**
 * @brief Path to `.crt` file
 */
const std::filesystem::path CERTIFICATE_CRT_FILE_PATH
    = "../src/certificates/crt.crt";
/**
 * @brief Path to `.key` file
 */
const std::filesystem::path CERTIFICATE_KEY_FILE_PATH
    = "../src/certificates/key.key";
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
 * @brief Paths to
 * self-signed certificate
 * and its key
 */
std::pair<std::filesystem::path,
    std::filesystem::path> certificate_;
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
        ipHost_ = json_[Config::constants::KEY_IP_HOST]
            .get<std::string>();
        if (!valid::ip(ipHost_)) {
            log.error <<
                "Error in parsing configurations. " <<
                "Reason: Invalid IP address for ip_host." <<
                "Must be 4 octets from [0-255]";
            ipHost_ = "";
            return false;
        }
    } else {
        log.error << "(parse) " <<
            "Error in parsing configurations. " <<
            "Reason: Can not find ip_host key";
        return false;
    }
    if (json_.contains(
            Config::constants::KEY_PORT_SACTIONS
        )) {
        portSActions_ = json_[Config::constants::KEY_PORT_SACTIONS]
            .get<unsigned int>();
        if (!valid::port(portSActions_)) {
            log.error <<
                "Error in parsing configurations. " <<
                "Reason: Invalid port for port_sactions. " <<
                "Must be from range [49152-65535]";
            portSActions_ = -1;
            return false;
        }
    } else {
        portSActions_ = 50000;
    }

    if (json_.contains(Config::constants::KEY_LOG_LEVEL)) {
        auto logLevelStr = json_[Config::constants::KEY_LOG_LEVEL]
            .get<std::string>();
        logLevel_ = mappers::fromString::logLevel(logLevelStr);
    } else {
        logLevel_ = ELogLevel::DEBUG;
    }

    if (json_.contains(Config::constants::KEY_PATHS_CERTIFICATE)) {
        certificate_ = json_[Config::constants::KEY_PATHS_CERTIFICATE]
            .get<std::pair<
                std::filesystem::path,
                std::filesystem::path>>();
    } else {
        certificate_ = std::make_pair(
            Config::constants::CERTIFICATE_CRT_FILE_PATH,
            Config::constants::CERTIFICATE_KEY_FILE_PATH
        );
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
 * @brief Get the paths to
 * self-signed certificate
 * and its key
 */
std::pair<std::filesystem::path,
    std::filesystem::path> certificate() {
    return certificate_;
}

/**
 * @brief Get the general logging level
 * @return `Config::logLevel_`
 */
ELogLevel logLevel() {
    return logLevel_;
}

} // namespace Config