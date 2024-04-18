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
  int k; // Number of groups to be formed
  int m; // Dimension of the space
  int n; // Number of points

  Point *points;    // Array of points
  int points_alloc; // Number of points allocated

  Distance *distances; // Array of distances
  int distances_size;  // Size of distances array

  Distance *edges; // Array of connected edges
  int *sz;         // For each item, gives the number of objects in the tree rooted in that item (used in kruskal's weighted quick union)

  Group *groups; // Array of groups
  int groups_a;  // Number of groups allocated
  int groups_u;  // Number of groups used in the memory
};

Cluster cluster_init(int k)
{
  Cluster cluster = (Cluster)calloc(1, sizeof(struct cluster));
  cluster->points_alloc = 2;
  cluster->points = (Point *)calloc(cluster->points_alloc, sizeof(Point));
  cluster->k = k;

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

    // get the point coordinates
    for (int i = 0; i < cluster->m; i++)
    {
      token = strtok(NULL, ",");
      point_setCoord(point, i, atof(token));
    }

    cluster->points[cluster->n] = point;
    cluster->n++;
  }

  free(line);
  free(token);
  fclose(file);

  // Alloc edges array (minimum size to connect all points (n-1))
  cluster->edges = (Distance *)calloc(cluster->n - 1, sizeof(Distance));
}

void cluster_calcDistances(Cluster cluster)
{
  cluster->distances_size = ((cluster->n * cluster->n) - cluster->n) / 2; // Size of the lower triangle of a square matrix sized n;
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
  qsort(cluster->distances, cluster->distances_size, sizeof(Distance), distance_compare);
}

void _MST_init(Cluster cluster) // weighted quick union implementation
{
  cluster->sz = (int *)calloc(cluster->n, sizeof(int));

  for (int i = 0; i < cluster->n; i++)
  {
    point_setSet(cluster->points[i], i);
    cluster->sz[i] = 1;
  }
}

int _MST_findRoot(Cluster cluster, int pointSet)
{
  while (pointSet != point_getSet(cluster->points[pointSet]))
  {
    pointSet = point_getSet(cluster->points[pointSet]);
  }

  return pointSet;
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

void cluster_kruskal(Cluster cluster)
{

  _MST_init(cluster); // generates a forest of points

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

      // add edge (current distance) to the edges array
      cluster->edges[edgesIndex] = cluster->distances[i];
      edgesIndex++;
    }
    else
    {
      // free non-edge distances
      free(cluster->distances[i]);
    }
  }
}

Group _exist_group_that_contains(Cluster cluster, char *pointId)
{
  for (int i = 0; i < cluster->groups_u; i++) // iterate over all groups
  {
    Group g = cluster->groups[i];

    if (g == NULL)
      continue;

    for (int j = 0; j < group_getSize(g); j++) // search point in the current group
    {
      if (strcmp(group_getPointId(g, j), pointId) == 0) // compares if point Ids are equal
        return g;                                       // return the group that contains the point
    }
  }

  return NULL; // point is not in a group
}

void _merge_groups(Group groupA, Group groupB)
{
  Group greater = NULL;
  Group smaller = NULL;

  if (group_getSize(groupA) > group_getSize(groupB))
  {
    greater = groupA;
    smaller = groupB;
  }
  else
  {
    greater = groupB;
    smaller = groupA;
  }

  for (int i = 0; i < group_getSize(smaller); i++) // iterate over groupB
  {
    group_addPoint(greater, group_getPointId(smaller, i)); // copies pointsIds from groupB to groupA
  }

  group_vanish(smaller); // empties groupB
}

void cluster_identifyGroups(Cluster cluster)
{
  // Init groups array
  if (cluster->k > cluster->n) // WARNING: if k > n, the number of groups will be equal to the number of points
  {
    cluster->k = cluster->n;
  }

  cluster->groups = (Group *)calloc(cluster->n, sizeof(Group));
  cluster->groups_a = cluster->n;
  cluster->groups_u = 0;

  // Put each point in a group (sets a forest of groups, each one containing 1 point)
  for (int i = 0; i < cluster->n; i++)
  {
    cluster->groups[i] = group_init();
    group_setId(cluster->groups[i], i);
    group_addPoint(cluster->groups[i], point_getId(cluster->points[i]));
    cluster->groups_u++;
  }

  int edgesSize = cluster->n - 1;
  int numberOfGroupSeparations = cluster->k - 1; // ignores the k-1 greatest distances

  for (int i = 0; i < (edgesSize - numberOfGroupSeparations); i++)
  {
    Distance edge = cluster->edges[i];
    Point pA = distance_getPoint(edge, PA);
    Point pB = distance_getPoint(edge, PB);

    char *pA_id = point_getId(pA);
    char *pB_id = point_getId(pB);

    Group groupA = _exist_group_that_contains(cluster, pA_id);
    Group groupB = _exist_group_that_contains(cluster, pB_id);

    if (group_getId(groupA) != group_getId(groupB))
    {
      _merge_groups(groupA, groupB);
    }
  }
}

void cluster_generateResult(Cluster cluster, char *filename)
{
  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    // printf("Error: File not found.\n");
    exit(1);
  }

  // get the real groups array, filtering empty groups in the groups array
  Group real[cluster->k];

  int idx = 0;
  for (int i = 0; i < cluster->groups_u; i++) // iterates through all groups previously created
  {
    if (group_getSize(cluster->groups[i]) > 0) // if groups is not empty
    {
      real[idx] = cluster->groups[i]; // add group to real groups array
      idx++;
    }
  }

  for (int i = 0; i < cluster->k; i++)
  {
    group_sort(real[i]);
  }

  // order groups by first point id
  qsort(real, cluster->k, sizeof(Group), group_compare);

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
