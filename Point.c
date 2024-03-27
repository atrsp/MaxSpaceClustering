#include "Point.h"

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

void point_destroy(Point point)
{
  free(point->id);
  free(point->coords);
  free(point);
}
