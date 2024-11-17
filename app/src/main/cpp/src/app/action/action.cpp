#include <utility>
#include <nlohmann/json.hpp>

#include "action.h"
#include "../../base/helpers/mappers.h"

namespace NAction {

/**
 * @brief Parser constructor
 * @param request Received request
 */
TAction::TAction(const crow::request&& request)
    : request_(std::move(request))
    , empty_(true)
{
    remoteIp_ = request_.remote_ip_address;
    httpMethod_ = request_.method;

    json_ = nlohmann::json::parse(request_.body);
    if (json_.contains(NAction::constants::REQUEST_KEY_ACTION_TYPE)) {
        auto actionTypeStr =
            json_[NAction::constants::REQUEST_KEY_ACTION_TYPE]
            .get<std::string>();
        actionType_ = mappers::fromString::actionType(actionTypeStr);
        empty_ = false;
    } else {
        empty_ = true;
    }
    if (json_.contains(NAction::constants::REQUEST_KEY_ENTITY)) {
        auto entityStr =
            json_[NAction::constants::REQUEST_KEY_ENTITY]
            .get<std::string>();
        entity_ = mappers::fromString::entity(entityStr);
        empty_ = false;
    } else {
        empty_ = true;
    }
    if (json_.contains(NAction::constants::REQUEST_KEY_ROLE)) {
        auto roleStr =
            json_[NAction::constants::REQUEST_KEY_ROLE]
            .get<std::string>();
        role_ = mappers::fromString::role(roleStr);
        empty_ = false;
    } else {
        empty_ = true;
    }
}

/**
 * @brief Get the received request
 * @return `TAction::request_`
 */
const crow::request& TAction::request() const {
    return request_;
}

/**
 * @brief Get the method of a request
 * @return `TAction::httpMethod_`
 */
const crow::HTTPMethod TAction::httpMethod() const {
    return httpMethod_;
}

/**
 * @brief Get the IP address of an actor
 * @return `TAction::remoteIp_`
 */
const std::string& TAction::remoteIp() const {
    return remoteIp_;
}

/**
 * @brief Get the type of an action
 * @return `TAction::actionType_`
 */
const EActionType& TAction::actionType() const {
    return actionType_;
}

/**
 * @brief Get the subject to take an action on
 * @return `TAction::entity_`
 */
const EEntity& TAction::entity() const {
    return entity_;
}

/**
 * @brief Get the role of an actor
 * @return `TAction::role_`
 */
const ERole& TAction::role() const {
    return role_;
}

/**
 * @brief Get the JSON body of a requested action
 * @return `TAction::json_`
 */
const nlohmann::json& TAction::json() const {
    return json_;
}

/**
 * @brief Whether a request has been successfully parsed
 * @return `TAction::empty_`
 */
const bool& TAction::empty() const {
    return empty_;
}

/**
 * @brief Print an action information to `std::ostream`
 * @param os Stream to concatenate to
 * @param action Action to print
 * @return Stream with the concatenated action information
 */
std::ostream& operator<<(std::ostream& os,
    const TAction& action) {
    os <<
    "IP address of an actor: " <<
        action.remoteIp() << '\n' <<
    "Type of a request of an actor: " <<
        mappers::toString::actionType(action.actionType()) << '\n' <<
    "Subject to take an action on: " <<
        mappers::toString::entity(action.entity()) << '\n' <<
    "Role of an actor: " <<
        mappers::toString::role(action.role()) << '\n';
}

} // namespace NAction