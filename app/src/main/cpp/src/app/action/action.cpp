#include <utility>
#include <nlohmann/json.hpp>

#include "Action.h"
#include "../../base/mappers/Mappers.h"

namespace NAction {

/**
 * @brief Parser constructor
 * @param request Received request
 */
TAction::TAction(const crow::request&& request)
    : request_(std::move(request))
    , empty_(true)
{
    remoteIp_ = request_.remote_ip;
    httpMethod_ = request_.method;

    json_ = nlohmann::json::parse(request_.body);
    if (json_.contains(NAction::constants::REQUEST_KEY_ACTION_TYPE)) {
        actionTypeStr = json_[NAction::constants::REQUEST_KEY_ACTION_TYPE].get<std::string>();
        actionType_ = NMappers::strToActionType(actionTypeStr);
        empty_ = false;
    } else {
        empty_ = true;
    }
    if (json_.contains(NAction::constants::REQUEST_KEY_ENTITY)) {
        entityStr = json_[NAction::constants::REQUEST_KEY_ENTITY].get<std::string>();
        entity_ = NMappers::strToEntity(entityStr);
        empty_ = false;
    } else {
        empty_ = true;
    }
    if (json_.contains(NAction::constants::REQUEST_KEY_ROLE)) {
        roleStr = json_[NAction::constants::REQUEST_KEY_ROLE].get<std::string>();
        role_ = NMappers::strToRole(roleStr);
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
const nlohmann::json TAction::json() const {
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
 * @param outputStream Stream to concatenate to
 * @param action Action to print
 * @return Stream with the concatenated action information
 */
std::ostream& operator<<(std::ostream& outputStream,
    const TAction& action) {
    os <<
        "IP address of an actor: " << action.remoteIp() << '\n' <<
        "Type of a request of an actor: " << NMappers::actionTypeToStr(action.actionType()) << '\n' <<
        "Subject to take an action on: " << NMappers::entityToStr(action.entity()) << '\n' <<
        "Role of an actor: " << NMappers::roleToStr(action.role()) << '\n';
}

} // namespace NAction