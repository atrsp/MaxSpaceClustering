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
 * Sets the coordinate of a point.
 *
 * @param point The point.
 * @param i The index of the coordinate.
 * @param coord The coordinate of the point.
 */
void point_setCoord(Point point, int i, double coord);

/**
 * Destroys a point.
 *
 * Uses free() from stdlib to deallocate memory used by the point.
 *
 * @return A pointer to the point.
 */
void point_destroy(Point point);

#endif