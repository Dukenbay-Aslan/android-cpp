#pragma once

#include <string>
#include <string_view>

#include "../Structs.h"

/**
 * @brief Helper functions
 * to convert custom structures
 * from/to `std::string`
 */
namespace mappers {

/**
 * @brief Helper functions
 * to convert custom structures
 * from `std::string`
 */
namespace fromString {

EActionType actionType(std::string_view actionType);
EEntity entity(std::string_view entity);
ERole role(std::string_view role);
ELogLevel logLevel(std::string_view level);

} // namespace fromString

/**
 * @brief Helper functions
 * to convert custom structures
 * to `std::string`
 */
namespace toString {

std::string actionType(const EActionType& actionType);
std::string entity(const EEntity& entity);
std::string role(const ERole& role);
std::string logLevel(const ELogLevel& level);

} // namespace toString

} // namespace mappers