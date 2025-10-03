/* > Description *******************************************************************/
/**
 * @file dfsPathFinding.h
 * @brief Defines the public interface for the Depth-First Search (DFS)
 *        pathfinding algorithm.
 *
 *****************************************************************************/

/* > Multiple Inclusion Protection ****************************************/
#ifndef DFS_PATH_FINDING_H
#define DFS_PATH_FINDING_H

/* > Includes *************************************************************/
#include "matrixWorld.h"
#include "path.h"
#include <stdint.h>

/* > Defines **************************************************************/

/* > Type Declarations ****************************************************/

/* > Constant Declarations ************************************************/

/* > Variable Declarations ************************************************/

/* > Function Declarations ************************************************/

/**
 * @brief Attempts to find a contiguous path of a specified length in a matrix.
 *
 * This function uses a randomized DFS with backtracking to find a path of
 * `pathLength` cells. It iterates by picking random, unblocked starting points
 * until a valid path is found or the search space is exhausted.
 *
 * @param[in] matrix_p   A pointer to the WorldMatrix to search within.
 * @param[in] pathLength The desired length of the path.
 * @return A pointer to a Path object if a path is found.
 *         Returns NULL if no path can be found. The caller is responsible
 *         for freeing the returned Path object using PATH_freePath().
 */
[[nodiscard]] Path* DFS_findPath(WorldMatrix* matrix_p, uint32_t pathLength);

#endif // DFS_PATH_FINDING_H