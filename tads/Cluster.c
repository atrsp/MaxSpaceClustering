#include "Cluster.h"
#include "Point.h"
#include "Distance.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PA 1
#define PB 0

struct cluster
{
  int k;                // Number of groups to be formed
  int m;                // Dimension of the space
  int n;                // Number of points
  Point *points;        // Array of points
  int points_alloc;     // Number of points allocated
  Distance *distances;  // Array of distances
  int distances_size;   // Size of distances array

  int* sz;              // For each item, gives the number of objects in the tree rooted in that item
};

Cluster cluster_init()
{
  Cluster cluster = (Cluster)calloc(1, sizeof(struct cluster));
  cluster->points_alloc = 2;
  cluster->points = (Point *)calloc(cluster->points_alloc, sizeof(Point));

  return cluster;
}

void cluster_read(Cluster cluster, char *filepath)
{
  FILE *file = fopen(filepath, "r");
  char *line = NULL;
  size_t len = 0;

  if (file == NULL)
  {
    printf("Error: File not found.\n");
    exit(1);
  }

  // Read the first line to get space dimension
  getline(&line, &len, file);
  char *token = strtok(line, ","); // ignores the point id

  while (strtok(NULL, ",") != NULL)
  {
    cluster->m++;
  }

  line = NULL;
  len = 0;

  // back to the beginning of the file
  rewind(file);

  while (getline(&line, &len, file) != -1)
  {
    if (cluster->n == cluster->points_alloc)
    {
      cluster->points_alloc *= 2;
      cluster->points = (Point *)realloc(cluster->points, cluster->points_alloc * sizeof(Point));
    }

    char *token = strtok(line, ","); // get the point id
    Point point = point_init(cluster->m, strlen(token) + 1);
    // printf("ID: %s\n", token);
    point_setId(point, token);

    for (int i = 0; i < cluster->m; i++)
    {
      token = strtok(NULL, ",");
      // printf("Coord: %s\n", token);
      point_setCoord(point, i, atof(token));
    }

    cluster->points[cluster->n] = point;
    cluster->n++;
  }

  free(line);
  free(token);
  fclose(file);
}

void _cluster_printPoints(Cluster cluster)
{
  // printf("\nquantity of points (n): %d;\n", cluster->n);
  // printf("space size (m): %d;\n", cluster->m);
  // printf("distances array size: %d.\n\n", cluster->distances_size);

  for (int i = 0; i < cluster->n; i++)
  {
    printf("Point %d\n", i);
    printf("ID: %s\n", point_getId(cluster->points[i]));
    for (int j = 0; j < cluster->m; j++)
    {
      printf("Coord %d: %f\n", j, point_getCoord(cluster->points[i], j));
    }
  }
}

void _cluster_printDistances(Cluster cluster)
{ 
  
  for (int i = 0; i < cluster->distances_size; i++)
    printf ("%d. DIST (pontos %s e %s): %lf\n", i, distance_getPointId(cluster->distances[i], 1), distance_getPointId(cluster->distances[i], 0), distance_getValue(cluster->distances[i]));
}


void cluster_calcDistances(Cluster cluster) 
{
  cluster->distances_size = (pow(cluster->n, 2) - cluster->n) / 2;   // Size of the lower triangle of a square matrix n sized;
  cluster->distances = distance_arrayInit(cluster->distances_size);

  int distancesIndex = 0;
  for (int j = 0; j < cluster->n; j++) 
  {
    for (int k = j+1; k < cluster->n; k++) 
    {
      double distance = point_euclidianDistance(cluster->points[j], cluster->points[k], cluster->m);
      cluster->distances[distancesIndex] = distance_set(cluster->points[j], cluster->points[k], distance);
      distancesIndex++;
    }
  }
  
}

void cluster_sortDistances(Cluster cluster) {
  qsort(cluster->distances, cluster->distances_size, sizeof(Distance), _distance_compare);
}

void _MST_init(Cluster cluster) {
  cluster->sz = (int*)calloc(cluster->n, sizeof(int)); //init

  for (int i = 0; i < cluster->n; i++) { //init
    point_setSet(cluster->points[i], i);
    cluster->sz[i] = 1;
  }
}

bool _MST_isConnected(Cluster cluster, int setA, int setB) {
    return _MST_findRoot(cluster, setA) == _MST_findRoot(cluster, setB);
}

int _MST_findRoot(Cluster cluster, int pointSet) {
  while(pointSet != point_getSet(cluster->points[pointSet]))
  {
    pointSet = point_getSet(cluster->points[pointSet]);
  }
  
  return pointSet;
}

void _MST_union(Cluster cluster, int setA, int setB) {
  int rootA = _MST_findRoot(cluster, setA);
  int rootB = _MST_findRoot(cluster, setB);

  if (cluster->sz[rootA] < cluster->sz[rootB]) {
    point_setSet(cluster->points[rootA], rootB);
    cluster->sz[rootB] += cluster->sz[rootA];
  }
  else {
    point_setSet(cluster->points[rootB], rootA);
    cluster->sz[rootA] += cluster->sz[rootB];
  }
}

void cluster_kruskal(Cluster cluster) {

  _MST_init(cluster);

  for (int i = 0; i < cluster->distances_size; i++) {
    Point pA = distance_getPoint(cluster->distances[i], PA);
    Point pB = distance_getPoint(cluster->distances[i], PB);

    int setA = point_getSet(pA);
    int setB = point_getSet(pB);

    if (setA != setB) {
      _MST_union(cluster, setA, setB);
    }
  }

  printf("\n");
  for (int i = 0; i < cluster->n; i++) {
    int set = point_getSet(cluster->points[i]);
    printf("%s[%s] ", point_getId(cluster->points[i]), point_getId(cluster->points[set]));
  }
  printf("\n");
}

void cluster_destroy(Cluster cluster)
{
  distance_destroy(cluster->distances, cluster->distances_size);
  
  for (int i = 0; i < cluster->n; i++)
    point_destroy(cluster->points[i]);
  
  free(cluster->points);
  free(cluster->sz);
  free(cluster);
}
