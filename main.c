#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tads/Cluster.h"

int main(int argc, char *argv[])
{
  clock_t start, end;
  double cpu_time_used;

  Cluster cluster = cluster_init();

  start = clock();
  cluster_read(cluster, argv[1]);
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("cluster_read() time: %.2f seconds\n", cpu_time_used);

  start = clock();
  cluster_calcDistances(cluster);
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("cluster_calcDistances() time: %.2f seconds\n", cpu_time_used);

  start = clock();
  cluster_sortDistances(cluster);
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("cluster_sortDistances() time: %.2f seconds\n", cpu_time_used);

  // cluster_kruskal();

  // cluster_identifyGroups(atoi(argv[2]));

  // cluster_generateResult(argv[3]);

  // Proves that the points were read correctly
  _cluster_printPoints(cluster);
  _cluster_printDistances(cluster);

  start = clock();
  cluster_destroy(cluster);
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("cluster_destroy() time: %.2f seconds\n", cpu_time_used);


  return 0;
}
