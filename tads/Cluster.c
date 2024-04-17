#include "Cluster.h"
#include "Point.h"
#include "Distance.h"
#include "Group.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PA 1
#define PB 0

struct cluster
{
  int k;               // Number of groups to be formed
  int m;               // Dimension of the space
  int n;               // Number of points
  Point *points;       // Array of points
  int points_alloc;    // Number of points allocated
  Distance *distances; // Array of distances
  int distances_size;  // Size of distances array

  int *sz; // For each item, gives the number of objects in the tree rooted in that item

  Group *groups;
  int groups_a;
  int groups_u;

  Distance *edges; // Array of edges
};

Cluster cluster_init(int k)
{
  Cluster cluster = (Cluster)calloc(1, sizeof(struct cluster));
  cluster->points_alloc = 2;
  cluster->points = (Point *)calloc(cluster->points_alloc, sizeof(Point));
  cluster->k = k;

  cluster->groups = (Group *)calloc(cluster->k, sizeof(Group));
  cluster->groups_a = k;
  cluster->groups_u = 0;

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
      cluster->points_alloc += 1;
      cluster->points = (Point *)realloc(cluster->points, cluster->points_alloc * sizeof(Point));
    }

    char *token = strtok(line, ","); // get the point id
    Point point = point_init(cluster->m, strlen(token) + 1);
    point_setId(point, token);

    for (int i = 0; i < cluster->m; i++)
    {
      token = strtok(NULL, ",");
      point_setCoord(point, i, atof(token));
    }

    cluster->points[cluster->n] = point;
    point_setIdx(point, cluster->n);
    cluster->n++;
  }

  free(line);
  free(token);
  fclose(file);

  // Alloc edges array
  cluster->edges = (Distance *)calloc(cluster->n - 1, sizeof(Distance));
}

void _cluster_printPoints(Cluster cluster)
{
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
    printf("%d. DIST (pontos %s e %s): %lf\n", i, distance_getPointId(cluster->distances[i], 1), distance_getPointId(cluster->distances[i], 0), distance_getValue(cluster->distances[i]));
}

void cluster_calcDistances(Cluster cluster)
{
  cluster->distances_size = (pow(cluster->n, 2) - cluster->n) / 2; // Size of the lower triangle of a square matrix n sized;
  cluster->distances = distance_arrayInit(cluster->distances_size);

  int distancesIndex = 0;
  for (int j = 0; j < cluster->n - 1; j++)
  {
    for (int k = j + 1; k < cluster->n; k++)
    {
      double distance = point_euclidianDistance(cluster->points[j], cluster->points[k], cluster->m);
      cluster->distances[distancesIndex] = distance_set(cluster->points[j], cluster->points[k], distance);
      distancesIndex++;
    }
  }
}

void cluster_sortDistances(Cluster cluster)
{
  qsort(cluster->distances, cluster->distances_size, sizeof(Distance), _distance_compare);
}

void _MST_init(Cluster cluster)
{
  cluster->sz = (int *)calloc(cluster->n, sizeof(int)); // init

  for (int i = 0; i < cluster->n; i++)
  { // init
    point_setSet(cluster->points[i], i);
    cluster->sz[i] = 1;
  }
}

bool _MST_isConnected(Cluster cluster, int setA, int setB)
{
  return _MST_findRoot(cluster, setA) == _MST_findRoot(cluster, setB);
}

int _MST_findRoot(Cluster cluster, int pointSet)
{
  while (pointSet != point_getSet(cluster->points[pointSet]))
  {
    pointSet = point_getSet(cluster->points[pointSet]);
  }

  return pointSet;
}

int _MST_findPreRoot(Cluster cluster, int idx, int pointSet, int root)
{
  int preRoot = 0;

  if (pointSet == root)
    preRoot = idx; //"pre root" == itself's index in the cluster->points array

  while (pointSet != root) // this while gets the "pre root" of the point, to create the cut in the tree afterwards
  {
    if (point_getSet(cluster->points[pointSet]) == root)
    {
      preRoot = pointSet;
      break;
    }
    pointSet = point_getSet(cluster->points[pointSet]);
  }

  return preRoot;
}

void _MST_union(Cluster cluster, int rootA, int rootB)
{

  if (cluster->sz[rootA] < cluster->sz[rootB])
  {
    point_setSet(cluster->points[rootA], rootB);
    cluster->sz[rootB] += cluster->sz[rootA];
  }
  else
  {
    point_setSet(cluster->points[rootB], rootA);
    cluster->sz[rootA] += cluster->sz[rootB];
  }
}

void _MST_cut(Cluster cluster, int pointSet)
{
  Point p = cluster->points[pointSet]; // point of preRoot to be cutted (pointSet)
  point_setSet(p, pointSet);           // sets the point to be cutted Set variable to itself, "cutting" the connection from it's previous root
}

void cluster_kruskal(Cluster cluster)
{

  _MST_init(cluster);

  int edgesIndex = 0;

  for (int i = 0; i < cluster->distances_size; i++)
  {
    Point pA = distance_getPoint(cluster->distances[i], PA);
    Point pB = distance_getPoint(cluster->distances[i], PB);

    int setA = point_getSet(pA);
    int setB = point_getSet(pB);

    int rootA = _MST_findRoot(cluster, setA);
    int rootB = _MST_findRoot(cluster, setB);

    if (rootA != rootB)
    {
      _MST_union(cluster, rootA, rootB);

      // add edge to the edges array
      cluster->edges[edgesIndex] = cluster->distances[i];
      edgesIndex++;
    }
    else
    {
      // free distance
      free(cluster->distances[i]);
    }
  }
}

Group _exist_group_that_contains(Cluster cluster, char *pointId)
{
  for (int i = 0; i < cluster->groups_u; i++)
  {
    Group g = cluster->groups[i];

    if (g == NULL)
      continue;

    for (int j = 0; j < group_getSize(g); j++)
    {
      if (strcmp(group_getPointId(g, j), pointId) == 0)
        return g;
    }
  }

  return NULL;
}

void _merge_groups(Group groupA, Group groupB)
{
  for (int i = 0; i < group_getSize(groupB); i++)
  {
    group_addPoint(groupA, group_getPointId(groupB, i));
  }

  group_vanish(groupB);
}

void cluster_identifyGroups(Cluster cluster)
{
  int edgesSize = cluster->n - 1;
  int numberOfGroupSeparations = cluster->k - 1;

  for (int i = 0; i < (edgesSize - numberOfGroupSeparations); i++)
  {
    Distance edge = cluster->edges[i];
    Point pA = distance_getPoint(edge, PA);
    Point pB = distance_getPoint(edge, PB);

    char *pA_id = point_getId(pA);
    char *pB_id = point_getId(pB);

    Group groupA = _exist_group_that_contains(cluster, pA_id);
    Group groupB = _exist_group_that_contains(cluster, pB_id);

    if (groupA != NULL && groupB == NULL)
    {
      group_addPoint(groupA, pB_id);
    }
    else if (groupA == NULL && groupB != NULL)
    {
      group_addPoint(groupB, pA_id);
    }
    else if (groupA != NULL && groupB != NULL)
    {
      _merge_groups(groupA, groupB);
    }
    else if (groupA == NULL && groupB == NULL)
    {
      if (cluster->groups_u == cluster->groups_a)
      {
        cluster->groups_a += 1;
        cluster->groups = (Group *)realloc(cluster->groups, cluster->groups_a * sizeof(Group));
      }

      // Add pA and pB to the group
      cluster->groups[cluster->groups_u] = group_init();

      group_addPoint(cluster->groups[cluster->groups_u], pA_id);
      group_addPoint(cluster->groups[cluster->groups_u], pB_id);

      cluster->groups_u++;
    }
  }
}

void cluster_generateResult(Cluster cluster, char *filename)
{
  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    printf("Error: File not found.\n");
    exit(1);
  }

  // get the real groups array
  Group real[cluster->k];

  int idx = 0;
  for (int i = 0; i < cluster->groups_u; i++)
  {
    if (group_getSize(cluster->groups[i]) > 0)
    {
      real[idx] = cluster->groups[i];
      idx++;
    }
  }

  for (int i = 0; i < cluster->k; i++)
  {
    group_sort(real[i]);
  }

  // order groups by first point id
  qsort(real, cluster->k, sizeof(Group), _group_compare);

  for (int i = 0; i < cluster->k; i++)
  {
    group_printOnFile(real[i], file);
  }

  fclose(file);
}

void cluster_destroy(Cluster cluster)
{
  free(cluster->distances);

  for (int i = 0; i < cluster->n - 1; i++)
  {
    free(cluster->edges[i]);
  }

  free(cluster->edges);

  for (int i = 0; i < cluster->n; i++)
    point_destroy(cluster->points[i]);

  for (int i = 0; i < cluster->groups_u; i++)
    group_destroy(cluster->groups[i]);

  free(cluster->points);
  free(cluster->groups);
  free(cluster->sz);
  free(cluster);
}
