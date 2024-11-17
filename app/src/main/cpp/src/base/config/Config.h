#pragma once

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

/**
 * @brief Control configurations
 */
namespace Config {

bool parse(const std::filesystem::path& path);
nlohmann::json json();
std::string ipHost();
unsigned int portSActions();

} // namespace Config