#pragma once

#include <string>
#include <crow/crow.h>
#include <iostream>

#include "../../base/Structs.h"

/**
 * @brief Implementation of the requested actions
 */
namespace NAction {

/**
 * @brief Helper constant values
 */
namespace constants {

/**
 * @brief JSON key of an action type
 */
const std::string REQUEST_KEY_ACTION_TYPE = "action_type";
/**
 * @brief JSON key of an entity to take an action on
 */
const std::string REQUEST_KEY_ENTITY = "entity";
/**
 * @brief JSON key of a role of an actor
 */
const std::string REQUEST_KEY_ROLE = "role";

} // namespace constants

/**
 * @brief Requested action
 */
class TAction {
  public:
    TAction(const crow::request&& request);

    // Member getters
    const crow::request& request() const;
    const crow::HTTPMethod httpMethod() const;
    const std::string& remoteIp() const;
    const EActionType& actionType() const;
    const EEntity& entity() const;
    const ERole& role() const;
    const nlohmann::json& json() const;
    const bool& empty() const;

    friend std::ostream& operator<<(std::ostream& outputStream,
        const TAction& action);
  private:
    crow::request request_ = {}; // Received request
    // Whether a request has been successfully parsed
    bool empty_;
    crow::HTTPMethod httpMethod_; // Method of a request
    std::string remoteIp_ = "0.0.0.0"; // IP address of an actor
    // Type of an action
    EActionType actionType_ = EActionType::UNKNOWN_ACTION_TYPE;
    // Subject to take an action on
    EEntity entity_ = EEntity::UNKNOWN_ENTITY;
    // Role of an actor
    ERole role_ = ERole::UNKNOWN_ROLE;
    // JSON body of a requested action
    nlohmann::json json_ = nlohmann::json::object;
};

} // namespace NAction