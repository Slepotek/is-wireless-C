/* > Description
 * ****************************************************************/
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
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <threads.h>

/* > Defines *****************************************************************/
#define NO_OF_THREADS 5
/* > Type Declarations *******************************************************/
/*
 * @brief Arguments to be passed to each DFS pathfinding thread.
 */
typedef struct {
  // Thread-specific identifier
  uint16_t thread_id;

  // Input parameters (read-only for the thread)
  const WorldMatrix *matrix_p;
  uint16_t desiredPathLength;

  // Pointers to shared resources and their corresponding mutexes
  StartingPointVector *usedStartingPoints_p;
  pthread_mutex_t *used_points_mutex_p;

  Path *final_path_p;
  volatile bool *path_is_found_p;
  pthread_mutex_t *completion_mutex_p;

} DFS_ThreadArgs_t;
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
 * @param[in]     matrix_p        A pointer to the world matrix.
 * @param[in,out] path_p          A pointer to the current path being built.
 * @param[in,out] visited_p       A pointer to the vector of visited cells for the
 *                                current search attempt.
 * @param[in]     path_is_found_p A pointer to flag used to indicate the
 *                                thread success during multithreaded scenario,
 *                                otherwise it is NULL
 * @param[in]     pathLength      The target length of the path.
 *
 * @return `true` if a path of the target length is successfully found,
 *         `false` otherwise.
 */
static bool DFS_internal_backtracking(const WorldMatrix *const matrix_p,
                                      Path *path_p,
                                      StartingPointVector *visited_p,
                                      volatile bool *path_is_found_p,
                                      uint32_t pathLength);

/**
 * @brief The worker function for each thread, responsible for executing the DFS
 * search.
 *
 * This function continuously searches for a valid path from random starting
 * points until a path is found by any thread or the search space is exhausted.
 * It manages its own local path and visited points vectors and only interacts
 * with shared data structures under mutex protection.
 *
 * @param[in] threadArgs A void pointer to a DFS_ThreadArgs_t struct containing
 *                       all necessary data and mutexes for the thread.
 * @return Always returns NULL.
 */
static void *DFS_findPathThreaded(void *threadArgs);

/**
 * @brief The original single-threaded implementation of the DFS pathfinding
 * algorithm.
 *
 * This function serves as the fallback when multithreading is disabled. It
 * iteratively picks random starting points and performs a DFS search until a
 * path is found or the search space is exhausted.
 *
 * @param[in] matrix_p   A pointer to the WorldMatrix to search within.
 * @param[in] pathLength The desired length of the path.
 * @return A pointer to a Path object if a path is found, otherwise NULL.
 */
static Path *
DFS_internal_findPathSingleThread(const WorldMatrix *const matrix_p,
                                  uint32_t pathLength);

/* > Global Function Definitions
 * *********************************************/

Path *DFS_findPath(WorldMatrix *matrix_p, uint32_t pathLength,
                   bool isMultithreading) {
  if (isMultithreading) {
    // Keep track of starting points that have already been tried.
    StartingPointVector *usedStartingPoints_p = STPOINT_createVector(matrix_p);
    // The path to be built and returned.
    Path *foundPath_p = PATH_initializePath(pathLength, matrix_p);

    pthread_t arrThread[NO_OF_THREADS] = {0};
    pthread_mutex_t usedStartingPointsMutex = {0};
    pthread_mutex_t completionCondMutex = {0};

    pthread_mutex_init(&usedStartingPointsMutex, NULL);
    pthread_mutex_init(&completionCondMutex, NULL);

    bool isPathFound = false;
    DFS_ThreadArgs_t threadArgsArr[NO_OF_THREADS] = {0};

    for (uint16_t thrIndex = 0; thrIndex < NO_OF_THREADS; thrIndex++) {
      DFS_ThreadArgs_t threadArgs = {
          .thread_id = thrIndex,
          .completion_mutex_p = &completionCondMutex,
          .used_points_mutex_p = &usedStartingPointsMutex,
          .usedStartingPoints_p = usedStartingPoints_p,
          .desiredPathLength = pathLength,
          .final_path_p = foundPath_p,
          .matrix_p = matrix_p,
          .path_is_found_p = &isPathFound};
      threadArgsArr[thrIndex] = threadArgs;

      pthread_create(&arrThread[thrIndex], NULL, DFS_findPathThreaded,
                     &threadArgsArr[thrIndex]);
    }
    for (int i = 0; i < NO_OF_THREADS; i++) {
      pthread_join(arrThread[i], NULL);
    }
    pthread_mutex_destroy(&usedStartingPointsMutex);
    pthread_mutex_destroy(&completionCondMutex);
    STPOINT_destroyVector(&usedStartingPoints_p);
    if (isPathFound) {
      return foundPath_p;
    }
    PATH_freePath(&foundPath_p);
    return NULL;
  } else {
    return DFS_internal_findPathSingleThread(matrix_p, pathLength);
  }
}

/* > Local Function Definitions **********************************************/

static bool DFS_internal_backtracking(const WorldMatrix *const matrix_p,
                                      Path *path_p,
                                      StartingPointVector *visited_p,
                                      volatile bool *path_is_found_p,
                                      uint32_t pathLength) {
  // Base case: If the path has reached the desired length, we are done.
  if (PATH_getLength(path_p) == pathLength) {
    return true;
  }
  if (path_is_found_p != NULL && *path_is_found_p == true) {
    return false;
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
        !STPOINT_containsPoint(visited_p, newPoint)) {
      // Mark the new point as visited and add it to the current path.
      STPOINT_addPoint(visited_p, newPoint);
      PATH_addCoordinates(path_p, newRow, newCol);

      // Recursively explore from the new point.
      if (DFS_internal_backtracking(matrix_p, path_p, visited_p,
                                    path_is_found_p, pathLength)) {
        return true; // Path found, propagate success upwards.
      }

      // Backtrack: If the recursive call failed, remove the point from the
      // path.
      UNUSED(PATH_popCoordinates(path_p));
    }
  }

  // If all neighbors have been explored and no path was found, backtrack
  // further.
  return false;
}

static void *DFS_findPathThreaded(void *threadParams) {
  DFS_ThreadArgs_t *thisThreadArgs_p = (DFS_ThreadArgs_t *)threadParams;
  uint32_t noOfUnblockedElements =
      MATRIXWORLD_getNoOfUnblockedCells(thisThreadArgs_p->matrix_p);
  uint16_t noOfCols = MATRIXWORLD_getRowSize(thisThreadArgs_p->matrix_p);
  uint16_t noOfRows = MATRIXWORLD_getColSize(thisThreadArgs_p->matrix_p);

  // Keep track of visited points for a single search attempt.
  StartingPointVector *visitedPoints_p =
      STPOINT_createVector(thisThreadArgs_p->matrix_p);
  // The path to be built and returned.
  Path *foundPath_p = PATH_initializePath(thisThreadArgs_p->desiredPathLength,
                                          thisThreadArgs_p->matrix_p);

  // Loop to try different random starting points.
  for (uint32_t index = 0; index < noOfUnblockedElements; index++) {
    // Clear vectors for the new attempt.
    STPOINT_cleanVector(visitedPoints_p);
    PATH_clearPath(foundPath_p);

    Cords startingPoint = {.row = UTILITY_generateRandomNumber(noOfRows),
                           .col = UTILITY_generateRandomNumber(noOfCols)};

    bool isStartingPointValid = false;
    // **** Lock the usedStartingPoints_p vector ****
    pthread_mutex_lock(thisThreadArgs_p->used_points_mutex_p);
    if (!STPOINT_containsPoint(thisThreadArgs_p->usedStartingPoints_p,
                               startingPoint)) {
      STPOINT_addPoint(thisThreadArgs_p->usedStartingPoints_p, startingPoint);
      isStartingPointValid = true;
    }
    pthread_mutex_unlock(thisThreadArgs_p->used_points_mutex_p);
    // **** Unlock the usedStartingPoints_p vector ****

    // Try this starting point if it's valid and hasn't been used before.
    if (isStartingPointValid &&
        !MATRIXWORLD_isBlocked(thisThreadArgs_p->matrix_p, startingPoint.row,
                               startingPoint.col)) {
      if (*thisThreadArgs_p->path_is_found_p)
      {
        // Finish earlier if the path has been already found.
        break;
      }
      STPOINT_addPoint(visitedPoints_p, startingPoint);
      PATH_addCoordinates(foundPath_p, startingPoint.row, startingPoint.col);

      // Start the recursive search.
      if (DFS_internal_backtracking(thisThreadArgs_p->matrix_p, foundPath_p,
                                    visitedPoints_p,
                                    thisThreadArgs_p->path_is_found_p,
                                    thisThreadArgs_p->desiredPathLength) ||
          *thisThreadArgs_p->path_is_found_p) {
        // On success, free the helper vectors and return the found path.

        // **** Lock the completion_mutex_p vector ****
        pthread_mutex_lock(thisThreadArgs_p->completion_mutex_p);
        if (*thisThreadArgs_p->path_is_found_p == false) {
          *thisThreadArgs_p->path_is_found_p = true;
          memcpy(thisThreadArgs_p->final_path_p, foundPath_p,
                 PATH_getByteSize(foundPath_p));
        }
        pthread_mutex_unlock(thisThreadArgs_p->completion_mutex_p);
        // **** Unlock the usedStartingPoints_p vector ****
        STPOINT_destroyVector(&visitedPoints_p);
      }
    }
  }

  // If loop finishes and no path was found. Clean up all resources.
  PATH_freePath(&foundPath_p);
  STPOINT_destroyVector(&visitedPoints_p);
  return NULL;
}

static Path *
DFS_internal_findPathSingleThread(const WorldMatrix *const matrix_p,
                                  uint32_t pathLength) {
  uint32_t noOfUnblockedElements = MATRIXWORLD_getNoOfUnblockedCells(matrix_p);
  uint16_t noOfCols = MATRIXWORLD_getRowSize(matrix_p);
  uint16_t noOfRows = MATRIXWORLD_getColSize(matrix_p);

  // Keep track of starting points that have already been tried.
  StartingPointVector *usedStartingPoints_p = STPOINT_createVector(matrix_p);
  // Keep track of visited points for a single search attempt.
  StartingPointVector *visitedPoints_p = STPOINT_createVector(matrix_p);
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
        !MATRIXWORLD_isBlocked(matrix_p, startingPoint.row,
                               startingPoint.col)) {
      STPOINT_addPoint(usedStartingPoints_p, startingPoint);
      STPOINT_addPoint(visitedPoints_p, startingPoint);
      PATH_addCoordinates(foundPath_p, startingPoint.row, startingPoint.col);

      // Start the recursive search.
      if (DFS_internal_backtracking(matrix_p, foundPath_p, visitedPoints_p,
                                    NULL, pathLength)) {
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