#ifndef _GROUP_H_
#define _GROUP_H_

typedef struct group *Group;

/**
 * @brief Initializes a new group.
 *
 * @return Group The new group.
 */
Group group_init();

/**
 * @brief Sets the root identifier of a group.
 *
 * @param group The group.
 * @param rootId The root identifier.
 */
void group_setRootId(Group group, char *rootId);

/**
 * @brief Adds a point to a group.
 *
 * @param group The group.
 * @param pointId The identifier of the point to add.
 */
void group_addPoint(Group group, char *pointId);

/**
 * @brief Gets the root identifier of a group.
 *
 * @param group The group.
 * @return char* The root identifier.
 */
char *group_getRootId(Group group);

/**
 * @brief Gets the size of a group.
 *
 * @param group The group.
 * @return int The size of the group.
 */
int group_getSize(Group group);

/**
 * @brief Gets the identifier of a point in a group.
 *
 * @param group The group.
 * @param index The index of the point.
 * @return char* The identifier of the point.
 */
char *group_getPointId(Group group, int index);

/**
 * @brief Destroys a group.
 *
 * @param group The group.
 */
void group_destroy(Group group);

#endif