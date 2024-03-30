#include "Cluster.h"
#include "Point.h"
#include "Distance.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct cluster
{
  int k;                // Number of groups to be formed
  int m;                // Dimension of the space
  int n;                // Number of points
  Point *points;        // Array of points
  int points_alloc;     // Number of points allocated
  Distance *distances;  // Array of distances
};

Cluster cluster_init()
{
  Cluster cluster = (Cluster)calloc(1, sizeof(struct cluster));
  cluster->points_alloc = 24;
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
    printf("ID: %s\n", token);
    point_setId(point, token);

    for (int i = 0; i < cluster->m; i++)
    {
      token = strtok(NULL, ",");
      printf("Coord: %s\n", token);
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

void cluster_calcDistances(Cluster cluster) {
  cluster->distances = distance_init(cluster->n);

  for (int i = 0; i < distance_getSize(cluster->distances); i++) {
    
  }
}

void cluster_destroy(Cluster cluster)
{
  for (int i = 0; i < cluster->n; i++)
  {
    point_destroy(cluster->points[i]);
  }
  free(cluster->points);
  free(cluster);
}
