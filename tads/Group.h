#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdio.h>

typedef struct group *Group;

/**
 * @brief Initializes a new group.
 *
 * @return Group The new group.
 */
Group group_init();

/**
 * @brief Adds a point to a group.
 *
 * @param group The group.
 * @param pointId The identifier of the point to add.
 */
void group_addPoint(Group group, char *pointId);

/**
 * @brief Gets the size of a group.
 *
 * @param group The group.
 * @return int The size of the group.
 */
int group_getSize(Group group);

/**
 * @brief Sets the size of a group.
 *
 * @param group The group.
 * @param size The new size.
 */
void group_setSize(Group group, int size);

/**
 * @brief Gets the identifier of a group.
 *
 * @param group The group.
 * @return int The identifier of the group.
 */
int group_getId(Group group);

/**
 * @brief Sets the identifier of a group.
 *
 * @param group The group.
 * @param id The new identifier.
 */
void group_setId(Group group, int id);

/**
 * @brief Remove all points and make size equal to 0.
 *
 * @param group The group.
 */
void group_vanish(Group group);

/**
 * @brief Gets the identifier of a point in a group.
 *
 * @param group The group.
 * @param index The index of the point.
 * @return char* The identifier of the point.
 */
char *group_getPointId(Group group, int index);

/**
 * @brief Sorts the points in a group by their ids.
 *
 * @param group The group.
 */
void group_sort(Group group);

/**
 * @brief Compares two groups by their first id.
 *
 * @param a The first group.
 * @param b The second group.
 * @return int The comparison result.
 */
int group_compare(const void *a, const void *b);

/**
 * @brief Prints a group on a file.
 *
 * @param group The group.
 * @param file The file.
 */
void group_printOnFile(Group group, FILE *file);

/**
 * @brief Destroys a group.
 *
 * @param group The group.
 */
void group_destroy(Group group);

#endif