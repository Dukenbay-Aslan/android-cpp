#pragma once

#include <filesystem>
#include <tuple>
#include <string>
#include <nlohmann/json.hpp>

#include "../Structs.h"

/**
 * @brief Control configurations
 */
namespace Config {

bool parse(const std::filesystem::path& path);
nlohmann::json json();
std::string ipHost();
unsigned int portSActions();
std::pair<std::filesystem::path,
    std::filesystem::path> certificate();
ELogLevel logLevel();

} // namespace Config