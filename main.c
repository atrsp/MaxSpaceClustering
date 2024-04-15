#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tads/Cluster.h"

int main(int argc, char *argv[])
{
  clock_t start, end, head, tail;
  double cpu_time_used;

  head = clock();
  Cluster cluster = cluster_init(atoi(argv[2]));

  start = clock();
  cluster_read(cluster, argv[1]);
  end = clock();

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("cluster_read() time: %.2f seconds\n", cpu_time_used);

  start = clock();
  cluster_calcDistances(cluster);
  end = clock();

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("cluster_calcDistances() time: %.2f seconds\n", cpu_time_used);

  start = clock();
  cluster_sortDistances(cluster);
  end = clock();

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("cluster_sortDistances() time: %.2f seconds\n", cpu_time_used);

  start = clock();
  cluster_kruskal(cluster);
  end = clock();

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("cluster_kruskal() time: %.2f seconds\n", cpu_time_used);

  start = clock();
  cluster_identifyGroups(cluster, atoi(argv[2]));
  end = clock();

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("cluster_identifyGroups() time: %.2f seconds\n", cpu_time_used);

  cluster_generateResult(cluster, argv[3]);

  // Proves that the points were read correctly
  // _cluster_printPoints(cluster);
  // _cluster_printDistances(cluster);

  start = clock();
  cluster_destroy(cluster);
  end = clock();

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("cluster_destroy() time: %.2f seconds\n", cpu_time_used);

  tail = clock();

  cpu_time_used = ((double)(tail - head)) / CLOCKS_PER_SEC;
  printf("total time: %.2f seconds\n", cpu_time_used);

  return 0;
}
