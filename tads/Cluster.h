#ifndef CLUSTER_H
#define CLUSTER_H

#include <stdbool.h>

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

/**
 * Prints the distances between points in the cluster.
 * 
 * @param cluster The cluster containing the distances to be printed.
 */
void _cluster_printDistances(Cluster cluster);

/**
 * Calculates the distances between points in the cluster and stores them in the cluster's distances array.
 * 
 * @param cluster The cluster for which distances are to be calculated.
 */
void cluster_calcDistances(Cluster cluster);

/**
 * Sorts the distances in the cluster's distances array with qsort() function.
 * 
 * @param cluster The cluster containing the distances to be sorted.
 */
void cluster_sortDistances(Cluster cluster);

void _MST_init(Cluster cluster);

bool _MST_isConnected(Cluster cluster, int setA, int setB);

int _MST_findRoot(Cluster cluster, int pointSet);

void _MST_union(Cluster cluster, int setA, int setB);

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
