#pragma once

/**
 * @brief Type of a request of the actor
 */
enum EActionType {
    CREATE = 0,
    UPDATE,
    DELETE,
    VIEW,
    SIZE_ACTION_TYPE,
    UNKNOWN_ACTION_TYPE,
};

/**
 * @brief Subject to take an action on
 */
enum EEntity {
    /**
     * @brief Group of roles
     */
    GROUP = 0,
    /**
     * @brief Account of role
     */
    ACCOUNT,
    /**
     * @brief Structured information about lessons
     */
    SCHEDULE,
    /**
     * @brief Entity with Video, Article, Marks and others
     */
    LESSON,
    /**
     * @brief Percentage of an academic performance of a Student
     */
    MARK,
    /**
     * @brief `for-loop`
     */
    SIZE_ENTITY,
    /**
     * @brief Handle situations
     */
    UNKNOWN_ENTITY,
};

/**
 * @brief Role of an actor
 */
enum ERole {
    /**
     * @brief Permissions:
     * - Create/Update/Delete a group
     * - Create/Update/Delete a account
     * - Create/Update/Delete a schedule
     * - Create/Update/Delete a lesson
     * - Create/Update/Delete a mark
     * - View a lesson
     */
    ADMIN = 0,
    /**
     * @brief Permissions:
     * - Create/Update/Delete a group
     * - Create/Update/Delete a account
     * - Create/Update/Delete a schedule
     */
    MANAGER,
    /**
     * @brief Permissions:
     * - Create/Update/Delete a lesson
     * - Create/Update/Delete a mark
     */
    TEACHER,
    /**
     * @brief Permissions:
     * - View a lesson
     */
    STUDENT,
    /**
     * @brief `for-loop`
     */
    SIZE_ROLE,
    /**
     * @brief Handle situations
     */
    UNKNOWN_ROLE,
};

/**
 * @brief Logging levels
 */
enum ELogLevel {
    ERROR = 0,
    WARNING,
    INFO,
    DEBUG,
    SIZE_LOG_LEVEL,
    UNKNOWN_LOG_LEVEL
};