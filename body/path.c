/* > Description ****************************************************************/
/**
 * @file path.c
 * @brief 
 *   This file provides the core functionality for the Path data structure, which
 *   is represented by a struct with a flexible array member. It includes functions
 *   for initialization, memory management, and path manipulation.
 */

/* > Includes ****************************************************************/
#include "path.h"
#include "matrixWorld.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* > Defines *****************************************************************/
#define SEVENTY_FIVE_PERCENT 0.75

/* > Type Declarations *******************************************************/ 

/**
 * @brief Defines the internal structure of the Path.
 */
struct Path {
  size_t pathSize; ///< The total capacity of the path.
  size_t currentNoOfCordsInPath; ///< The current number of coordinates stored.
  Cords pathArray[]; ///< A flexible array member to hold the coordinates.
};

/* > Global Constant Definitions *********************************************/

/* > Global Variable Definitions *********************************************/

/* > Local Constant Definitions **********************************************/

/* > Local Variable Definitions **********************************************/

/* > Local Function Declarations *********************************************/

/**
 * @brief Internal function to check for null pointers and exit on failure.
 * @param path_p[in]  Pointer to the Path.
 * @param message[in] The error message to print on failure.
 */
static void PATH_internal_nullCheck(const Path *const path_p,
                                    const char *restrict message);

/* > Global Function Definitions *********************************************/

Path *PATH_initializePath(const size_t pathSize,
                          const WorldMatrix *const matrix_p) {
  size_t matrixSize = MATRIXWORLD_getSize(matrix_p);
  if (pathSize == 0) {
    fprintf(stderr, "FATAL ERROR: Path size can not be 0!\n");
    exit(EXIT_FAILURE);
  } else if (pathSize > (matrixSize * SEVENTY_FIVE_PERCENT)) {
    fprintf(stderr, "ERROR: Path size must be within 75%% size "
                    "constraint of the matrix!\n");
  }
  size_t sizeOfThePathStruct = sizeof(Path) + (sizeof(Cords) * pathSize);
  Path *newPath = (Path*)malloc(sizeOfThePathStruct);
  PATH_internal_nullCheck(
      newPath,
      "FATAL ERROR: Path structure could not be initialized. No memory!\n");
  newPath->pathSize = pathSize;
  newPath->currentNoOfCordsInPath = 0;

  return newPath;
}

void PATH_freePath(Path** const path_pp) { free(*path_pp); }

void PATH_addCoordinates(Path* const path_p, uint16_t row, uint16_t col) {
  PATH_internal_nullCheck(
      path_p, "FATAL ERROR: Trying to add coordinates to Path structure, but "
              "the path structure is not initialized!\n");
  if (path_p->currentNoOfCordsInPath < path_p->pathSize) {
    Cords coordinates = {.row = row, .col = col};
    path_p->pathArray[path_p->currentNoOfCordsInPath] = coordinates;
    path_p->currentNoOfCordsInPath++;
  } else {
    fprintf(stderr, "FATAL ERROR: Trying to add new coordinates to the Path "
                    "but reached the maximum size of the Path struct!\n");
    exit(EXIT_FAILURE);
  }
}

Cords PATH_getLastCoordinates(const Path* const path_p) {
  PATH_internal_nullCheck(
      path_p,
      "FATAL ERROR: Trying to get last coordinates of Path structure, but "
      "the path structure is not initialized!\n");
  if (PATH_isEmpty(path_p)) {
    fprintf(stderr,
            "ERROR: The Path structure is initialized, but there are no "
            "coordinates in the Path structure. The Path is EMPTY!\n");
    return (Cords){.row = UINT16_MAX, .col = UINT16_MAX};
  }
  Cords lastCoordinates = path_p->pathArray[path_p->currentNoOfCordsInPath - 1];
  return lastCoordinates;
}

bool PATH_containsCoordinates(const Path* const path_p, const Cords* const coordinates) {
  PATH_internal_nullCheck(
      path_p,
      "FATAL ERROR: Trying to get last coordinates of Path structure, but "
      "the path structure is not initialized!\n");
  if (PATH_isEmpty(path_p)) {
    fprintf(stderr,
            "ERROR: The Path structure is initialized, but there are no "
            "coordinates in the Path structure. The Path is EMPTY!\n");
    return false;
  }
  if (coordinates->row == UINT16_MAX || coordinates->col == UINT16_MAX)
  {
    fprintf(stderr, "FATAL ERROR: coordinates that are being checked in the Path struct are invalid!");
    exit(EXIT_FAILURE);
  }
  for (uint32_t index = 0; index < path_p->currentNoOfCordsInPath; index++)
  {
    Cords tempCheck = path_p->pathArray[index];
    if (tempCheck.row == coordinates->row && tempCheck.col == coordinates->col)
    {
      return true;
    }
  }
  return false;
}

Cords PATH_popCoordinates(Path* const path_p) {
  PATH_internal_nullCheck(
      path_p,
      "FATAL ERROR: Trying to get last coordinates of Path structure, but "
      "the path structure is not initialized!\n");
  if (PATH_isEmpty(path_p)) {
    fprintf(stderr,
            "ERROR: The Path structure is initialized, but there are no "
            "coordinates in the Path structure. The Path is EMPTY!\n");
    return (Cords){.row = UINT16_MAX, .col = UINT16_MAX};
  }
  Cords lastCoordinates = PATH_getLastCoordinates(path_p);
  path_p->currentNoOfCordsInPath--;
  path_p->pathArray[path_p->currentNoOfCordsInPath] =
      (Cords){.row = 0, .col = 0};
  return lastCoordinates;
}

bool PATH_isContiguous(const Path * const path_p) {
  PATH_internal_nullCheck(
      path_p, "FATAL ERROR: Trying to check if the Path is contiguous but the "
              "path structure is not initialized!\n");
  if (PATH_isEmpty(path_p) || path_p->currentNoOfCordsInPath < 2) {
    return true;
  }

  for (size_t index = 1; index < path_p->currentNoOfCordsInPath; index++) {
    Cords currentCords = path_p->pathArray[index - 1];
    Cords nextCords = path_p->pathArray[index];

    uint16_t row_dist = currentCords.row > nextCords.row
                            ? (currentCords.row - nextCords.row)
                            : (nextCords.row - currentCords.row);
    uint16_t col_dist = currentCords.col > nextCords.col
                            ? (currentCords.col - nextCords.col)
                            : (nextCords.col - currentCords.col);

    if ((row_dist + col_dist) != 1) {
      return false;
    }
  }
  return true;
}

bool PATH_isEmpty(const Path * const path_p) {
  PATH_internal_nullCheck(path_p, "FATAL ERROR: Path is not initialized!\n");
  return path_p->currentNoOfCordsInPath == 0;
}

size_t PATH_getLength(const Path* const path_p) {
  PATH_internal_nullCheck(path_p, "FATAL ERROR: Path is not initialized!\n");
  return path_p->currentNoOfCordsInPath;
}

void PATH_clearPath(Path* const path_p) {
  PATH_internal_nullCheck(path_p, "FATAL ERROR: Path is not initialized!\n");
  memset(path_p->pathArray, 0, sizeof(Cords) * path_p->currentNoOfCordsInPath);
  path_p->currentNoOfCordsInPath = 0;
}

void PATH_printPath(const Path* const path_p) {
  PATH_internal_nullCheck(path_p, "FATAL ERROR: Path is not initialized!\n");
  if (PATH_isEmpty(path_p)) {
    printf("Path is empty.\n");
    return;
  }
  printf("Path (length %zu):\n", path_p->currentNoOfCordsInPath);
  for (size_t i = 0; i < path_p->currentNoOfCordsInPath; ++i) {
    printf("  [%zu]: (%u, %u)\n", i, path_p->pathArray[i].row,
           path_p->pathArray[i].col);
  }
}

size_t PATH_getByteSize(const Path* const path_p)
{
  return (sizeof(Path) + (sizeof(Cords) * path_p->currentNoOfCordsInPath));
}

/* > Local Function Definitions **********************************************/

static void PATH_internal_nullCheck(const Path *const path_p,
                                    const char *restrict message) {
  if (path_p == NULL) {
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
  }
}