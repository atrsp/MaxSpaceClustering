#ifndef POINT_H
#define POINT_H

typedef struct point *Point;

/**
 * Initializes a point.
 *
 * Uses calloc() from stdlib to allocate memory for the point.
 *
 * @param m Dimension of the space.
 * @param idInitialSize Initial length of the identifier.
 * @return A pointer to the point.
 */
Point point_init(int m, int idInitialLength);

/**
 * Gets the identifier of a point.
 *
 * @param point The point.
 * @return The identifier of the point.
 */
char *point_getId(Point point);

/**
 * Gets the coordinate of a point.
 *
 * @param point The point.
 * @param i The index of the coordinate.
 * @return The coordinate of the point.
 */
double point_getCoord(Point point, int i);

/**
 * Sets the identifier of a point.
 *
 * @param point The point.
 * @param id The identifier of the point.
 */
void point_setId(Point point, char *id);

/**
 * Sets the index of the set (immediate parent) that the point belongs to.
 * 
 * @param p The Point object for which to set the index of the set.
 * @param set The index of the set that the point belongs to.
 */
void point_setSet(Point p, int set);

/**
 * Retrieves the index of the set (immediate parent) that the point belongs to.
 * 
 * @param p The Point object for which to retrieve the index of the set.
 * @return The index of the set that the point belongs to.
 */
int point_getSet(Point p);

/**
 * Sets the coordinate of a point.
 *
 * @param point The point.
 * @param i The index of the coordinate.
 * @param coord The coordinate of the point.
 */
void point_setCoord(Point point, int i, double coord);


void point_setIdx(Point point, int idx);

int point_getIdx(Point point);

/**
 * Calculates the Euclidian Distance between two points.
 *
 * @param pA First point.
 * @param pB Second point.
 * @param dimension The dimension from the points.
 * @return The calculated Euclidian Distance.
 */
double point_euclidianDistance (Point pA, Point pB, int dimension);

/**
 * Destroys a point.
 *
 * Uses free() from stdlib to deallocate memory used by the point.
 *
 * @return A pointer to the point.
 */
void point_destroy(Point point);

#endif