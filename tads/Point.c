#include "Point.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

struct point
{
  char *id;       // Identifier of the point
  double *coords; // Coordinates of the point
  int set;        // index of immediate parent
};

Point point_init(int m, int idInitialLength)
{
  Point point = (Point)calloc(1, sizeof(struct point));
  point->id = (char *)calloc(idInitialLength, sizeof(char));
  point->coords = (double *)calloc(m, sizeof(double));
  point->set = 0;

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

void point_setSet(Point p, int set)
{
  p->set = set;
}

int point_getSet(Point p)
{
  return p->set;
}

void point_setCoord(Point point, int i, double coord)
{
  point->coords[i] = coord;
}

double point_euclidianDistance(Point pA, Point pB, int dimension)
{
  double distance = 0;
  double sum = 0;

  for (int i = 0; i < dimension; i++)
  {
    double diff = pA->coords[i] - pB->coords[i];
    sum += diff * diff;
  }

  distance = sqrt(sum);
  return distance;
}

void point_destroy(Point point)
{
  free(point->id);
  free(point->coords);
  free(point);
}
