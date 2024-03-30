#include "Cluster.h"
#include "Point.h"
#include "Distance.h"

#include <stdlib.h>
#include <math.h>
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
  int distances_size;   // Size of distances array
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

void cluster_destroy(Cluster cluster)
{
  distance_destroy(cluster->distances, cluster->distances_size);
  
  for (int i = 0; i < cluster->n; i++)
    point_destroy(cluster->points[i]);
  
  free(cluster->points);
  free(cluster);
}
