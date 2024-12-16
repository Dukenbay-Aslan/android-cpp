#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <crow.h>
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
 * @brief `action_type`. JSON key of an action type.
 * One of `CREATE`, `UPDATE`, `DELETE`, `VIEW`
 */
const std::string REQUEST_KEY_ACTION_TYPE = "action_type";
/**
 * @brief `entity`. JSON key of an entity to take an action on.
 * One of `GROUP`, `ACCOUNT`, `SCHEDULE`, `LESSON`, `MARK`
 */
const std::string REQUEST_KEY_ENTITY = "entity";
/**
 * @brief `role`. JSON key of a role of an actor.
 * One of `ADMIN`, `MANAGER`, `TEACHER`, `STUDENT`
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

    friend std::ostream& operator<<(std::ostream& os,
        const TAction& action);
  private:
    /**
     * @brief Received request
     */
    crow::request request_ = {};
    /**
     * @brief Whether a request has been
     * successfully parsed
     */
    bool empty_;
    /**
     * @brief Method of a request
     */
    crow::HTTPMethod httpMethod_;
    /**
     * @brief IP address of an actor
     */
    std::string remoteIp_ = "0.0.0.0";
    /**
     * @brief Type of an action
     */
    EActionType actionType_ = EActionType::UNKNOWN_ACTION_TYPE;
    /**
     * @brief Subject to take an action on
     */
    EEntity entity_ = EEntity::UNKNOWN_ENTITY;
    /**
     * @brief Role of an actor
     */
    ERole role_ = ERole::UNKNOWN_ROLE;
    /**
     * @brief JSON body of a requested action
     */
    nlohmann::json json_ = nlohmann::json::object();
};

} // namespace NAction