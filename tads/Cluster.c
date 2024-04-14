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
    point_setIdx(point, cluster->n);
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

int _MST_findPreRoot(Cluster cluster, int idx, int pointSet, int root) {
  int preRoot = 0;
  
  if (pointSet == root) preRoot = idx; //"pre root" == itself's index in the cluster->points array

  while(pointSet != root) //this while gets the "pre root" of the point, to create the cut in the tree afterwards
  {
    if (point_getSet(cluster->points[pointSet]) == root) {
      preRoot = pointSet;
      break;
    }
    pointSet = point_getSet(cluster->points[pointSet]);
  }

  return preRoot;
}

void _MST_union(Cluster cluster, int rootA, int rootB) {

  if (cluster->sz[rootA] < cluster->sz[rootB]) {
    point_setSet(cluster->points[rootA], rootB);
    cluster->sz[rootB] += cluster->sz[rootA];
  }
  else {
    point_setSet(cluster->points[rootB], rootA);
    cluster->sz[rootA] += cluster->sz[rootB];
  }
}

void _MST_cut(Cluster cluster, int pointSet) {
  Point p = cluster->points[pointSet]; // point of preRoot to be cutted (pointSet)
  point_setSet(p, pointSet); // sets the point to be cutted Set variable to itself, "cutting" the connection from it's previous root
}

void cluster_kruskal(Cluster cluster) {

  _MST_init(cluster);

  for (int i = 0; i < cluster->distances_size; i++) {
    Point pA = distance_getPoint(cluster->distances[i], PA);
    Point pB = distance_getPoint(cluster->distances[i], PB);

    int setA = point_getSet(pA);
    int setB = point_getSet(pB);

    int rootA = _MST_findRoot(cluster, setA);
    int rootB = _MST_findRoot(cluster, setB);

    if (rootA != rootB) {
      _MST_union(cluster, rootA, rootB);
    }
  }

  printf("\n");
  for (int i = 0; i < cluster->n; i++) {
    int set = point_getSet(cluster->points[i]);
    printf("%s[%s] ", point_getId(cluster->points[i]), point_getId(cluster->points[set]));
  }
  printf("\n");
}

void cluster_identifyGroups(Cluster cluster, int k) {
  int nCuts = 0;
  int root = 0;
  int preRootA = 0, preRootB = 0;
  int nTreeA = 0, nTreeB = 0; // subtree sizes

  for (int i = cluster->distances_size-1; i >= 0; i--) {
    if (nCuts == k-1) break; // for k=3 groups, we need 2 cuts in the tree, for example

    Point pA = distance_getPoint(cluster->distances[i], PA);
    Point pB = distance_getPoint(cluster->distances[i], PB);

    int setA = point_getSet(pA);
    int setB = point_getSet(pB);
    int idxA = point_getIdx(pA);
    int idxB = point_getIdx(pB);

    if (_MST_isConnected(cluster, setA, setB)) { // get the points from the distance array and check if they are connected
      root = _MST_findRoot(cluster, setA);
      preRootA = _MST_findPreRoot(cluster, idxA, setA, root); // get preRoot of pointB
      preRootB = _MST_findPreRoot(cluster, idxB, setB, root); // get preRoot of pointB

      printf("\ncut: pointA[%d]: %s; pointB[%d]: %s\n", idxA, point_getId(pA), idxB, point_getId(pB));
      nTreeA = cluster->sz[preRootA];
      nTreeB = cluster->sz[preRootB];

      if (nTreeA < nTreeB) { // compares if pA tree branch (from sz array) is smaller than pB's
        if (preRootB == root) {
          _MST_cut(cluster, preRootA); // cuts connection from pointA to root
        }
        else _MST_cut(cluster, preRootB);
      }
      else if (nTreeA >= nTreeB) { // compares if pA tree branch (from sz array) is bigger than pB's
        if (preRootA == root) {
          _MST_cut(cluster, preRootB); // cuts connection from pointB to root
        }
        else _MST_cut(cluster, preRootA);
      }

      nCuts+=1;
    }
    else continue; // points are already not connected -> group already identified
  }

  // for (int i = 0; i < cluster->n; i++) {
  //   printf("sz[%d] = %d ", i, cluster->sz[i]);
  // }
  // printf("\n");

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
