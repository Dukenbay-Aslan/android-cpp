#include <string>
#include <vector>

#include "valid.h"
#include "../../base/helpers/utils.h"

namespace valid {

/**
 * @brief Check whether a string is valid IP address
 * @param string String to check
 * @return `string` consists of 4 octets and
 * each octet is [0-255]
 */
bool ip(std::string_view string) {
    auto octets = utils::split(string, ".");
    if (octets.size() != 4) {
        return false;
    }
    for (auto octetView : octets) {
        auto octet = std::stoi(std::string(octetView));
        if (octet < 0 or octet > 255) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Check whether a number is valid port
 * @param number Number to check
 * @return `49152 <= number <= 65535`
 */
bool port(unsigned int number) {
    return (number >= 49152 and number <= 65535);
}

/**
 * @brief Check whether a number is valid port
 * @param number Number to check in string format
 * @return `1024 <= number <= 65535`
 */
bool port(const std::string& numberStr) {
    auto number = std::stoi(numberStr);
    return (number >= 1024 and number <= 65535);
}

}