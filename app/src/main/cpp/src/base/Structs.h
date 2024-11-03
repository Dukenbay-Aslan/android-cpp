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
     * Group of roles
     */
    GROUP = 0,
    /**
     * Account of role
     */
    ACCOUNT,
    /**
     * Structured information about lessons
     */
    SCHEDULE,
    /**
     * Entity with Video, Article, Marks and others
     */
    LESSON,
    /**
     * Percentage of an academic performance of a Student
     */
    MARK,
    /**
     * `for-loop`
     */
    SIZE_ENTITY,
    /**
     * Handle situations
     */
    UNKNOWN_ENTITY,
};

/**
 * @brief Role of an actor
 */
enum ERole {
    /**
     * Permissions:
     * - Create/Update/Delete a group
     * - Create/Update/Delete a account
     * - Create/Update/Delete a schedule
     * - Create/Update/Delete a lesson
     * - Create/Update/Delete a mark
     * - View a lesson
     */
    ADMIN = 0,
    /**
     * Permissions:
     * - Create/Update/Delete a group
     * - Create/Update/Delete a account
     * - Create/Update/Delete a schedule
     */
    MANAGER,
    /**
     * Permissions:
     * - Create/Update/Delete a lesson
     * - Create/Update/Delete a mark
     */
    TEACHER,
    /**
     * Permissions:
     * - View a lesson
     */
    STUDENT,
    /**
     * `for-loop`
     */
    SIZE_ROLE,
    /**
     * Handle situations
     */
    UNKNOWN_ROLE,
};