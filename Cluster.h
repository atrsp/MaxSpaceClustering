#ifndef CLUSTER_H
#define CLUSTER_H

typedef struct cluster *Cluster;

/**
 * Initializes a cluster.
 *
 * Uses calloc() from stdlib to allocate memory for the cluster.
 *
 * @return A pointer to the cluster.
 */
Cluster cluster_init();

void cluster_read(char *filename);
void cluster_calcDistances();
void cluster_orderDistances();
void cluster_kruskal();
void cluster_identifyGroups(int k);
void cluster_generateResult(char *filename);

/**
 * Destroys a cluster.
 *
 * Uses free() from stdlib to deallocate memory used by the cluster.
 *
 * @param cluster The cluster to be destroyed.
 */
void cluster_destroy(Cluster cluster);

#endif
