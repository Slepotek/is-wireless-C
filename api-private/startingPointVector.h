/* > Description *******************************************************************/
/**
 * @file startingPointVector.c
 * @brief
 *   This header file defines the public interface for the StartingPointVector data
 *   structure. This structure is used to track exhausted starting points in a
 *   thread-aware manner for the pathfinding algorithm.
 *
 *****************************************************************************/

/* > Multiple Inclusion Protection ****************************************/
#ifndef STARTING_POINT_VECTOR_H
#define STARTING_POINT_VECTOR_H

/* > Includes *************************************************************/
#include "matrixWorld.h"
#include "utilities.h"
#include <stdbool.h>
#include <stdint.h>

/* > Defines **************************************************************/

/* > Type Declarations ****************************************************/
typedef struct StartingPointVector StartingPointVector;

/* > Constant Declarations ************************************************/

/* > Variable Declarations ************************************************/

/* > Function Declarations ************************************************/

/**
 * @brief Creates and allocates a new vector for storing exhausted points.
 * @param matrix_p A pointer to an initialized WorldMatrix.
 * @return A pointer to the newly created StartingPointVector.
 */
[[nodiscard]] StartingPointVector *STPOINT_createVector(const WorldMatrix *const matrix_p);

/**
 * @brief Frees all memory associated with the StartingPointVector.
 * @param vector_pp A pointer to the pointer of the vector to be destroyed.
 */
void STPOINT_destroyVector(StartingPointVector **const vector_pp);

/**
 * @brief Adds a point to the vector while maintaining sorted order.
 * @param vector_p The vector to which the point will be added.
 * @param point The coordinate to add.
 */
void STPOINT_addPoint(StartingPointVector *const vector_p, Cords point);

/**
 * @brief Checks if a point exists in the vector using a binary search.
 * @param vector_p The vector to search within.
 * @param point The coordinate to look for.
 * @return `true` if the point is found, `false` otherwise.
 */
[[nodiscard]] bool STPOINT_containsPoint(const StartingPointVector *const vector_p, Cords point);

/* > End of Multiple Inclusion Protection *********************************/
#endif // STARTING_POINT_VECTOR_H
