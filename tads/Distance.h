#ifndef DISTANCE_H
#define DISTANCE_H

#include "Point.h"

typedef struct distance *Distance;

/**
 * Initializes an array of distances.
 *
 * @param arraySize The size of the array to be initialized.
 * @return An array of distances initialized to zero.
 */
Distance* distance_arrayInit(int arraySize);

/**
 * Sets the values of a Distance object.
 *
 * @param pA The first point.
 * @param pB The second point.
 * @param distance The distance between the two points.
 * @return The initialized Distance object.
 */
Distance distance_set(Point pA, Point pB, double distance);

/**
 * Retrieves the distance value from a Distance object.
 *
 * @param d The Distance object.
 * @return The distance value.
 */
double distance_getValue (Distance d);

/**
 * Retrieves the ID of one of the points in a Distance object.
 *
 * @param d The Distance object.
 * @param whichPoint Specifies which point to get the ID from (0 for pointA, 1 for pointB).
 * @return The ID of the specified point.
 */
char* distance_getPointId (Distance d, int whichPoint);

/**
 * 
*/
Point distance_getPoint (Distance d, int whichPoint);

/**
 * Comparison function for sorting distances (crescent order).
 *
 * @param a Pointer to the first Distance object.
 * @param b Pointer to the second Distance object.
 * @return Negative value if the first distance is smaller, positive value if the first distance is larger, 0 if equal.
 */
int _distance_compare(const void* a, const void* b);

/**
 * Deallocates memory for an array of Distance objects.
 *
 * @param d The array of Distance objects.
 * @param size The size of the array.
 */
void distance_destroy (Distance* d, int size);

#endif