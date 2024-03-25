#include <stdlib.h>

#include "Cluster.h"

int main(int argc, char *argv[])
{
  Cluster *cluster = cluster_init();

  cluster_read(argv[0]);

  cluster_calcDistances();

  cluster_orderDistances();

  cluster_kruskal();

  cluster_identifyGroups(atoi(argv[1]));

  cluster_generateResult(argv[2]);

  return 0;
}
