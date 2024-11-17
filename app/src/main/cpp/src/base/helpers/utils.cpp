#include <chrono>
#include <iomanip>
#include <sstream>

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

/**
 * @brief Get current time
 * in format `YYYY-MM-DD hh:mm:ss.sss`
 * @return `std::chrono::system_clock::now()`
 * converted to `std::string`
 */
std::string now() {
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    
    // Convert to time_t for date and time formatting
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    
    // Convert to milliseconds for fractional seconds
    auto duration_since_epoch = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<
                    std::chrono::milliseconds>
                    (duration_since_epoch) % 1000;
    
    // Format the time using std::put_time (in UTC)
    std::ostringstream oss;
    oss << std::put_time(
        std::gmtime(&time_t_now), 
        "%Y-%m-%d %H:%M:%S"
    );
    
    // Append the fractional seconds (milliseconds)
    oss <<
        '.' <<
        std::setfill('0') <<
        std::setw(3) <<
        millis.count();
    
    return oss.str();
}

} // namespace utils