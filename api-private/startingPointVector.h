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
 *
 * @param[in] matrix_p A pointer to an initialized WorldMatrix.
 * @return A pointer to the newly created StartingPointVector.
 */
[[nodiscard]] StartingPointVector *STPOINT_createVector(const WorldMatrix *const matrix_p);

/**
 * @brief Frees all memory associated with the StartingPointVector.
 *
 * @param[in,out] vector_pp A pointer to the pointer of the vector to be destroyed.
 *                          The pointer is set to NULL after destruction.
 */
void STPOINT_destroyVector(StartingPointVector **const vector_pp);

/**
 * @brief Clears all points from the vector.
 *
 * This function resets the vector's size to zero, effectively clearing it
 * without deallocating the underlying memory.
 *
 * @param[in,out] vector_p A pointer to the vector to be cleared.
 */
void STPOINT_cleanVector(StartingPointVector *const vector_p);

/**
 * @brief Adds a point to the vector while maintaining sorted order.
 *
 * If the point already exists, the function does nothing.
 *
 * @param[in,out] vector_p The vector to which the point will be added.
 * @param[in]     point    The coordinate to add.
 */
void STPOINT_addPoint(StartingPointVector *const vector_p, Cords point);

/**
 * @brief Removes a point from the vector while maintaining sorted order.
 *
 * If the point is not found in the vector, the function does nothing.
 *
 * @param[in,out] vector_p A pointer to the vector to be modified.
 * @param[in]     point    The coordinate to remove.
 */
void STPOINT_removePoint(StartingPointVector *const vector_p, Cords point);

/**
 * @brief Checks if a point exists in the vector using a binary search.
 *
 * @param[in] vector_p The vector to search within.
 * @param[in] point    The coordinate to look for.
 * @return `true` if the point is found, `false` otherwise.
 */
[[nodiscard]] bool STPOINT_containsPoint(const StartingPointVector *const vector_p, Cords point);

/**
 * @brief Gets the current number of points in the vector.
 *
 * @param[in] vector_p A pointer to the vector.
 * @return The number of points currently stored in the vector.
 */
[[nodiscard]] size_t STPOINT_getSize(const StartingPointVector *const vector_p);

/* > End of Multiple Inclusion Protection *********************************/
#endif // STARTING_POINT_VECTOR_H
