#include "Distance.h"
#include <stdlib.h>
#include <stdio.h>

struct distance {
    Point pA;           // pointA
    Point pB;           // pointB
    double distance;    // distance between pA and pB 
};

#define PA 1
#define PB 0

Distance* distance_arrayInit(int arraySize) {
    Distance* distance = (Distance*)calloc(arraySize, sizeof(Distance));

    return distance;
}

Distance distance_set(Point pA, Point pB, double distance) 
{
    Distance pApB = (Distance)calloc(1, sizeof(struct distance));
    pApB->pA = pA;
    pApB->pB = pB;
    pApB->distance = distance;

    return pApB;
}

double distance_getValue (Distance d) 
{
    return d->distance;
}

char* distance_getPointId (Distance d, int whichPoint) 
{   
    if (whichPoint)
        return point_getId(d->pA);

    return point_getId(d->pB);
}

Point distance_getPoint (Distance d, int whichPoint) 
{   
    if (whichPoint == PA)
        return d->pA;

    return d->pB;
}

int _distance_compare(const void *a, const void *b) {
    const Distance distanceA = *(const Distance*)a;
    const Distance distanceB = *(const Distance*)b;

    // Compare distances
    if (distanceA->distance < distanceB->distance) return -1;
    if (distanceA->distance > distanceB->distance) return 1;
    return 0; // Distances are equal
}

void distance_destroy (Distance* d, int size) 
{
    for (int i=0; i<size; i++) 
        free (d[i]);

    free(d);
    
}