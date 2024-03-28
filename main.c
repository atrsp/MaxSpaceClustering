#include <stdlib.h>

#include "Cluster.h"

int main(int argc, char *argv[])
{
  Cluster cluster = cluster_init();

  cluster_read(cluster, argv[1]);

  // cluster_calcDistances();

  // cluster_orderDistances();

  // cluster_kruskal();

  // cluster_identifyGroups(atoi(argv[2]));

  // cluster_generateResult(argv[3]);

  // Proves that the points were read correctly
  _cluster_printPoints(cluster);

  cluster_destroy(cluster);

  return 0;
}
