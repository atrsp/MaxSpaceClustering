#ifndef DISTANCE_H
#define DISTANCE_H

#include "Point.h"

typedef struct distance *Distance;

Distance* distance_arrayInit(int arraySize);
Distance distance_set(Point pA, Point pB, double distance);
double distance_getValue (Distance d);
char* distance_getPointId (Distance d, int whichPoint);
void distance_destroy (Distance* d, int size);

#endif