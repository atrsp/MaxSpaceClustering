#include "Group.h"

struct group
{
  char *rootId;     // The identifier of the root point
  char **pointsIds; // The identifiers of the points in the group
  int points_u;     // Number of points in the group
  int points_a;     // Number of points allocated
};