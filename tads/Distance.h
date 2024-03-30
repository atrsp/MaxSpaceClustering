#ifndef DISTANCE_H
#define DISTANCE_H

typedef struct distance *Distance;

Distance distance_init(int n);

int distance_getSize(Distance distance);

#endif