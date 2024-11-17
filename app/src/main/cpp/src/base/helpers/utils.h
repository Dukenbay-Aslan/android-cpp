#pragma once

#include <string_view>
#include <vector>

/**
 * @brief Helper functions on STL containers
 */
namespace utils {

std::vector<std::string_view> split(std::string_view string,
    std::string_view delimiter);
std::string now();

} // namespace utils