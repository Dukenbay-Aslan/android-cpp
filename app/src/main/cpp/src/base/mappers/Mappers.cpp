#include "Mappers.h"

namespace NMappers {

/**
 * @brief Mapper
 * @param actionType Type of a request of the actor
 * @return Action type in string format
 */
std::string actionTypeToStr(const EActionType& actionType) {
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
 * @param actionType Type of a request of the actor
 * @return Action type struct
 */
EActionType strToActionType(std::string_view actionType) {
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
 * @return Entity in string format
 */
std::string entityToStr(const EEntity& entity) {
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
 * @param entity Subject to take an action on
 * @return Entity struct
 */
EEntity strToEntity(std::string_view entity) {
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
 * @return Role in string format
 */
std::string roleToStr(const ERole& role) {
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

/**
 * @brief Mapper
 * @param role Role of an actor
 * @return Role struct
 */
ERole strToRole(std::string_view role) {
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

} // namespace NMappers