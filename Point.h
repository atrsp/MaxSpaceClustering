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
 * Destroys a point.
 *
 * Uses free() from stdlib to deallocate memory used by the point.
 *
 * @return A pointer to the point.
 */
void point_destroy(Point point);

#endif