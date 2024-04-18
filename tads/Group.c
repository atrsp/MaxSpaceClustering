#include "Group.h"

#include <stdlib.h>
#include <string.h>

struct group
{
  char **pointsIds; // The identifiers of the points in the group
  int points_u;     // Number of points in the group
  int points_a;     // Number of points allocated
  int id;           // Group identifier
};

Group group_init()
{
  Group group = (Group)calloc(1, sizeof(struct group));
  group->points_a = 2;
  group->pointsIds = (char **)calloc(group->points_a, sizeof(char *));

  group->points_u = 0;

  return group;
}

void group_addPoint(Group group, char *pointId)
{
  if (group->points_u == group->points_a)
  {
    group->points_a += 1;
    group->pointsIds = (char **)realloc(group->pointsIds, group->points_a * sizeof(char *));
  }

  group->pointsIds[group->points_u] = pointId;
  group->points_u++;
}

int group_getSize(Group group)
{
  return group->points_u;
}

void group_setSize(Group group, int size)
{
  group->points_u = size;
}

int group_getId(Group group)
{
  return group->id;
}

void group_setId(Group group, int id)
{
  group->id = id;
}

void group_vanish(Group group)
{
  for (int i = 0; i < group->points_u; i++)
  {
    group->pointsIds[i] = NULL;
  }

  group->points_u = 0;
}

char *group_getPointId(Group group, int index)
{
  return group->pointsIds[index];
}

int _compare_id(const void *a, const void *b)
{
  return strcmp(*(char **)a, *(char **)b);
}

void group_sort(Group group)
{
  qsort(group->pointsIds, group->points_u, sizeof(char *), _compare_id);
}

int group_compare(const void *a, const void *b)
{
  return strcmp((*(Group *)a)->pointsIds[0], (*(Group *)b)->pointsIds[0]);
}

void group_printOnFile(Group group, FILE *file)
{
  for (int i = 0; i < group->points_u; i++)
  {
    if (i < group->points_u - 1)
    {
      fprintf(file, "%s,", group->pointsIds[i]);
    }
    else
    {
      fprintf(file, "%s\n", group->pointsIds[i]);
    }
  }
}

void group_destroy(Group group)
{
  free(group->pointsIds);
  free(group);
}
