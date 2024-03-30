#include "Distance.h"
#include <stdlib.h>

struct distance {
    Point pA;           // pointA
    Point pB;           // pointB
    double distance;    // distance between pA and pB 
};

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

void distance_destroy (Distance* d, int size) 
{
    for (int i=0; i<size; i++) 
        free (d[i]);

    free(d);
    
}