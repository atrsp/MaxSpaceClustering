#include "Cluster.h"
#include "Point.h"

struct cluster
{
  int k;         // Number of groups to be formed
  int m;         // Dimension of the space
  int n;         // Number of points
  Point *points; // Array of points
};

Cluster cluster_init()
{
  return (Cluster)calloc(1, sizeof(struct cluster));
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
