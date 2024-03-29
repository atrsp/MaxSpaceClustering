#include "Point.h"

#include <stdlib.h>
#include <string.h>

struct point
{
  char *id;       // Identifier of the point
  double *coords; // Coordinates of the point
};

Point point_init(int m, int idInitialLength)
{
  Point point = (Point)calloc(1, sizeof(struct point));
  point->id = (char *)calloc(idInitialLength, sizeof(char));
  point->coords = (double *)calloc(m, sizeof(double));
  return point;
}

char *point_getId(Point point)
{
  return point->id;
}

double point_getCoord(Point point, int i)
{
  return point->coords[i];
}

void point_setId(Point point, char *id)
{
  strcpy(point->id, id);
}

void point_setCoord(Point point, int i, double coord)
{
  point->coords[i] = coord;
}

void point_destroy(Point point)
{
  free(point->id);
  free(point->coords);
  free(point);
}
