#include "Group.h"

struct group
{
  char *rootId;     // The identifier of the root point
  char **pointsIds; // The identifiers of the points in the group
  int points_u;     // Number of points in the group
  int points_a;     // Number of points allocated
};

Group group_init()
{
  Group group = (Group)calloc(1, sizeof(struct group));
  group->points_a = 2;
  group->pointsIds = (char **)calloc(group->points_a, sizeof(char *));

  return group;
}

void group_setRootId(Group group, char *rootId)
{
  group->rootId = rootId;
}

void group_addPoint(Group group, char *pointId)
{
  if (group->points_u == group->points_a)
  {
    group->points_a *= 2;
    group->pointsIds = (char **)realloc(group->pointsIds, group->points_a * sizeof(char *));
  }

  group->pointsIds[group->points_u++] = pointId;
}

void group_destroy(Group group)
{
  free(group->pointsIds);
  free(group);
}