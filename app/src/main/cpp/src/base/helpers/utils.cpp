#include "utils.h"

namespace utils {

/**
 * @brief Split a string by delimiter
 * and collect substrings
 * @param string String to split
 * @param delimiter Delimiter to split by
 * @return List of substrings
 */
std::vector<std::string_view> split(std::string_view string,
        std::string_view delimiter) {
    std::vector<std::string_view> result;

    if (string.empty() || delimiter.empty()) {
        return result; // Handle edge cases
    }

    size_t start = 0;
    size_t end = 0;

    while ((end = string.find(delimiter, start)) != std::string_view::npos) {
        if (end != start) { // Avoid empty substrings
            result.emplace_back(string.substr(start, end - start));
        }
        start = end + delimiter.length();
    }

    // Add the final segment (if any)
    if (start < string.size()) {
        result.emplace_back(string.substr(start));
    }

    return result;
}

} // namespace utils