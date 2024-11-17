#include "mappers.h"

namespace mappers {

namespace fromString {

/**
 * @brief Mapper
 * @param actionType Type of a request of the actor
 * @return Action type struct
 */
EActionType actionType(std::string_view actionType) {
    if (actionType == "CREATE") {
        return EActionType::CREATE;
    }
    if (actionType == "UPDATE") {
        return EActionType::UPDATE;
    }
    if (actionType == "DELETE") {
        return EActionType::DELETE;
    }
    if (actionType == "VIEW") {
        return EActionType::VIEW;
    }
    return EActionType::UNKNOWN_ACTION_TYPE;
}

/**
 * @brief Mapper
 * @param entity Subject to take an action on
 * @return Entity struct
 */
EEntity entity(std::string_view entity) {
    if (entity == "GROUP") {
        return EEntity::GROUP;
    }
    if (entity == "ACCOUNT") {
        return EEntity::ACCOUNT;
    }
    if (entity == "SCHEDULE") {
        return EEntity::SCHEDULE;
    }
    if (entity == "LESSON") {
        return EEntity::LESSON;
    }
    if (entity == "MARK") {
        return EEntity::MARK;
    }
    return EEntity::UNKNOWN_ENTITY;
}

/**
 * @brief Mapper
 * @param role Role of an actor
 * @return Role struct
 */
ERole role(std::string_view role) {
    if (role == "ADMIN") {
        return ERole::ADMIN;
    }
    if (role == "MANAGER") {
        return ERole::MANAGER;
    }
    if (role == "TEACHER") {
        return ERole::TEACHER;
    }
    if (role == "STUDENT") {
        return ERole::STUDENT;
    }
    return ERole::UNKNOWN_ROLE;
}

/**
 * @brief Mapper
 * @param level Logging level
 * @return Log level struct
 */
ELogLevel logLevel(std::string_view level) {
    if (level == "ERROR") {
        return ELogLevel::ERROR;
    }
    if (level == "WARNING") {
        return ELogLevel::WARNING;
    }
    if (level == "INFO") {
        return ELogLevel::INFO;
    }
    if (level == "DEBUG") {
        return ELogLevel::DEBUG;
    }
    return ELogLevel::UNKNOWN_LOG_LEVEL;
}

} // namespace fromString

namespace toString {

/**
 * @brief Mapper
 * @param actionType Type of a request of the actor
 * @return Action type in string format
 */
std::string actionType(const EActionType& actionType) {
    switch (actionType) {
        case EActionType::CREATE:
            return "CREATE";

        case EActionType::UPDATE:
            return "UPDATE";

        case EActionType::DELETE:
            return "DELETE";

        case EActionType::VIEW:
            return "VIEW";

        default:
            return "UNKNOWN_ACTION_TYPE";
    }
}

/**
 * @brief Mapper
 * @param entity Subject to take an action on
 * @return Entity in string format
 */
std::string entity(const EEntity& entity) {
    switch (entity) {
        case EEntity::GROUP:
            return "GROUP";
            
        case EEntity::ACCOUNT:
            return "ACCOUNT";
            
        case EEntity::SCHEDULE:
            return "SCHEDULE";
            
        case EEntity::LESSON:
            return "LESSON";
            
        case EEntity::MARK:
            return "MARK";

        default:
            return "UNKNOWN_ENTITY";
    }
}

/**
 * @brief Mapper
 * @param role Role of an actor
 * @return Role in string format
 */
std::string role(const ERole& role) {
    switch (role) {
        case ERole::ADMIN:
            return "ADMIN";

        case ERole::MANAGER:
            return "MANAGER";

        case ERole::TEACHER:
            return "TEACHER";

        case ERole::STUDENT:
            return "STUDENT";
        
        default:
            return "UNKNOWN_ROLE";
    }
}

std::string logLevel(const ELogLevel& level) {
    switch (level) {
        case ELogLevel::ERROR:
            return "ERROR";

        case ELogLevel::WARNING:
            return "WARNING";

        case ELogLevel::INFO:
            return "INFO";

        case ELogLevel::DEBUG:
            return "DEBUG";

        default:
            return "UNKNOWN_LOG_LEVEL";
    }
}

} // namespace toString

} // namespace mappers