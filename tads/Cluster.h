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

/**
 * Reads a file containing points and save them in the cluster.
 *
 * Uses fopen() and getline() from stdio to open the file and read its contents.
 * Uses strtok() from string.h to split the lines of the file.
 *
 * Each line of the file should contain the identifier of the point followed by the coordinates separated by commas.
 *
 * @param filepath The path to the file containing the points.
 */
void cluster_read(Cluster cluster, char *filepath);

/**
 * Prints the points of a cluster.
 *
 * Uses printf() from stdio to print the identifier and coordinates of each point in the cluster.
 *
 * @param cluster The cluster to be printed.
 */
void _cluster_printPoints(Cluster cluster);

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
