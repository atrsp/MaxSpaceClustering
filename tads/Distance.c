#include "Distance.h"
#include "Point.h"

struct distance {
    Point pA;           // pointA
    Point pB;           // pointB
    double distance;    // distance between pA and pB
    int size;           // array size 
};

Distance distance_init(int n) {
    int arraySize = ((n*n) - n) / 2; // size of the lower triangle of a square matrix n sized;

    Distance distance = (Distance)calloc(arraySize, sizeof(struct distance));

    distance->size = arraySize;

    return distance;
}

int distance_getSize(Distance distance) {
    return distance->size;
}