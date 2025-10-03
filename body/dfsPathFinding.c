/* > Description ****************************************************************/
/**
 * @file dfsPathFinding.c
 * @brief Implements a randomized Depth-First Search (DFS) with backtracking
 *        to find a contiguous path in a 2D matrix.
 *
 */

/* > Includes ****************************************************************/
#include "dfsPathFinding.h"
#include "matrixWorld.h"
#include "path.h"
#include "startingPointVector.h"
#include "utilities.h"
#include <pthread.h> // TODO: multithread this algorithm
#include <stdint.h>

/* > Defines *****************************************************************/

/* > Type Declarations *******************************************************/

/* > Global Constant Definitions *********************************************/

/* > Global Variable Definitions *********************************************/

/* > Local Constant Definitions **********************************************/

/* > Local Variable Definitions **********************************************/

/* > Local Function Declarations *********************************************/

/**
 * @brief The recursive backtracking helper function for the DFS algorithm.
 *
 * This function attempts to extend the current path by exploring unvisited
 * neighbors. If a dead end is reached, it backtracks.
 *
 * @param[in]     matrix_p   A pointer to the world matrix.
 * @param[in,out] path_p     A pointer to the current path being built.
 * @param[in,out] visited_p  A pointer to the vector of visited cells for the
 *                           current search attempt.
 * @param[in]     pathLength The target length of the path.
 * @return `true` if a path of the target length is successfully found,
 *         `false` otherwise.
 */
static bool DFS_internal_backtracking(WorldMatrix* matrix_p, Path* path_p, StartingPointVector* visited_p, uint32_t pathLength);

/* > Global Function Definitions *********************************************/

Path* DFS_findPath(WorldMatrix* matrix_p, uint32_t pathLength)
{
    uint32_t noOfUnblockedElements = MATRIXWORLD_getNoOfUnblockedCells(matrix_p);
    uint16_t noOfCols = MATRIXWORLD_getRowSize(matrix_p);
    uint16_t noOfRows = MATRIXWORLD_getColSize(matrix_p);

    // Keep track of starting points that have already been tried.
    StartingPointVector* usedStartingPoints_p = STPOINT_createVector(matrix_p);
    // Keep track of visited points for a single search attempt.
    StartingPointVector* visitedPoints_p = STPOINT_createVector(matrix_p);
    // The path to be built and returned.
    Path *foundPath_p = PATH_initializePath(pathLength, matrix_p);

    // Loop to try different random starting points.
    for (uint32_t index = 0; index < noOfUnblockedElements; index++) {
      // Clear vectors for the new attempt.
      STPOINT_cleanVector(visitedPoints_p);
      PATH_clearPath(foundPath_p);

      Cords startingPoint = {.row = UTILITY_generateRandomNumber(noOfRows),
                             .col = UTILITY_generateRandomNumber(noOfCols)};

      // Try this starting point if it's valid and hasn't been used before.
      if (!STPOINT_containsPoint(usedStartingPoints_p, startingPoint) &&
          !MATRIXWORLD_isBlocked(matrix_p, startingPoint.row, startingPoint.col))
      {
          STPOINT_addPoint(usedStartingPoints_p, startingPoint);
          STPOINT_addPoint(visitedPoints_p, startingPoint);
          PATH_addCoordinates(foundPath_p, startingPoint.row, startingPoint.col);

          // Start the recursive search.
          if (DFS_internal_backtracking(matrix_p, foundPath_p, visitedPoints_p, pathLength)) {
            // On success, free the helper vectors and return the found path.
            STPOINT_destroyVector(&usedStartingPoints_p);
            STPOINT_destroyVector(&visitedPoints_p);
            return foundPath_p;
          }
      }
    }

    // If loop finishes, no path was found. Clean up all resources.
    PATH_freePath(&foundPath_p);
    STPOINT_destroyVector(&usedStartingPoints_p);
    STPOINT_destroyVector(&visitedPoints_p);
    return NULL;
}

/* > Local Function Definitions **********************************************/

static bool DFS_internal_backtracking(WorldMatrix* matrix_p, Path* path_p, StartingPointVector* visited_p, uint32_t pathLength)
{
    // Base case: If the path has reached the desired length, we are done.
    if (PATH_getLength(path_p) == pathLength)
    {
        return true;
    }

    Cords currentPosition = PATH_getLastCoordinates(path_p);

    // Explore neighbors in all four directions.
    for (uint8_t index = 0; index < FOUR_DIRECTIONS; index++) {
        // Use signed integers for temporary calculation to detect underflow.
        int16_t tempNewRow = (currentPosition.row + directions[index].row);
        int16_t tempNewCol = currentPosition.col + directions[index].col;

        // Safely convert back to unsigned, handling potential negative values.
        uint16_t newRow = (tempNewRow >= 0) ? tempNewRow : UINT16_MAX;
        uint16_t newCol = (tempNewCol >= 0) ? tempNewCol : UINT16_MAX;
        Cords newPoint = {.row = newRow, .col = newCol};

        // Check if the new point is a valid move.
        if (newRow < MATRIXWORLD_getColSize(matrix_p) &&
            newCol < MATRIXWORLD_getRowSize(matrix_p) &&
            !MATRIXWORLD_isBlocked(matrix_p, newRow, newCol) &&
            !STPOINT_containsPoint(visited_p, newPoint)) 
        {
          // Mark the new point as visited and add it to the current path.
          STPOINT_addPoint(visited_p, newPoint);
          PATH_addCoordinates(path_p, newRow, newCol);

          // Recursively explore from the new point.
          if (DFS_internal_backtracking(matrix_p, path_p, visited_p, pathLength)) {
            return true; // Path found, propagate success upwards.
          }

          // Backtrack: If the recursive call failed, remove the point from the path.
          UNUSED(PATH_popCoordinates(path_p));
        }
    }

    // If all neighbors have been explored and no path was found, backtrack further.
    return false;
}
