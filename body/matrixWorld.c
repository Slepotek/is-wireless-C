/**
 * @file matrixWorld.c
 * @author Slepotek (slepotek.dev@gmail.com)
 * @brief Implements the WorldMatrix data structure and its manipulation functions.
 * @version 1.0
 * @date 2025-09-28
 *
 * This file provides the core functionality for creating, modifying, and querying a 2D matrix
 * that represents the world. The matrix consists of cells that can be either "blocked" or "unblocked",
 * forming the basis for pathfinding algorithms.
 */

/* > Includes ****************************************************************/
#include "matrixWorld.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* > Defines *****************************************************************/
#define MINIMUM_MATRIX_SIZE 4
/* > Type Declarations *******************************************************/ 
struct WorldMatrix {
  uint16_t rows;
  uint16_t cols;
  uint32_t noOfUnblockedCells;
  uint32_t noOfBlockedCells;
  size_t worldSize;
  bool worldMatrix[];
};

/* > Global Constant Definitions *********************************************/

/* > Global Variable Definitions *********************************************/

/* > Local Constant Definitions **********************************************/

/* > Local Variable Definitions **********************************************/

/* > Local Function Declarations *********************************************/

/**
 * @brief Internal function to check for null pointers and exit on failure.
 *
 * @param matrix_p[in]  Pointer to the WorldMatrix.
 * @param message[in]  The error message to print on failure.
 */
static void MATRIXWORLD_internal_nullCheck(const WorldMatrix *const matrix_p,
                                           const char *restrict message);

/**
 * @brief Internal function to check for out-of-bounds access and exit on failure.
 *
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @param row[in] The row to check.
 * @param col[in] The column to check.
 * @param message[in] The formatted error message to print on failure.
 * @param ...[in] Variable arguments for the format string.
 */
static void
MATRIXWORLD_internal_checkSizeBoundaries(const WorldMatrix *const matrix_p, uint16_t row,
                                         uint16_t col,
                                         const char *restrict message, ...);

/* > Global Function Definitions *********************************************/

WorldMatrix *MATRIXWORLD_matrixInitialization(uint16_t rows, uint16_t cols) {
  size_t noOfMatrixBoolElements = rows * cols;
  if (noOfMatrixBoolElements < MINIMUM_MATRIX_SIZE)
  {
    fprintf(stderr, "FATAL ERROR: Matrix size can not be smaller than 4!\n");
    exit(EXIT_FAILURE);
  }
  WorldMatrix *matrix_p = (WorldMatrix*)malloc(
      sizeof(WorldMatrix) + (sizeof(bool) * noOfMatrixBoolElements));

  if (matrix_p == NULL) {
    fprintf(stderr,
            "FATAL ERROR: Failed to allocate memory for the WorldMatrix.\n");
    exit(EXIT_FAILURE);
  }
  matrix_p->rows = rows;
  matrix_p->cols = cols;
  matrix_p->worldSize = noOfMatrixBoolElements;
  matrix_p->noOfBlockedCells = 0;
  matrix_p->noOfUnblockedCells = (uint32_t)noOfMatrixBoolElements;
  memset(matrix_p->worldMatrix, 0,
         (uint64_t)(sizeof(bool) * noOfMatrixBoolElements));

  return matrix_p;
}

void MATRIXWORLD_matrixFree(WorldMatrix **const matrix_pp) { free(*matrix_pp); }

WorldMatrix *MATRIXWORLD_matrixResize(WorldMatrix *matrix_p, uint16_t rows,
                                      uint16_t cols) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: Trying to resize uninitialized matrix\n");
  WorldMatrix* resizedMatrix_p = MATRIXWORLD_matrixInitialization(rows, cols);
  MATRIXWORLD_matrixFree(&matrix_p);
  return resizedMatrix_p;
}

bool MATRIXWORLD_matrixBlanking(WorldMatrix *const matrix_p,
                                const Cords *const coordinates_p,
                                uint16_t noOfElementsToBlock) {
  if (noOfElementsToBlock > matrix_p->worldSize) {
    fprintf(
        stderr,
        "FATAL ERROR: Can not blank more elements than a matrix can hold\n");
    exit(EXIT_FAILURE);
  }
  const Cords* currentCoords_p = coordinates_p;
  for (uint16_t index = 0; index < noOfElementsToBlock; index++) {
    Cords elementToSet = *currentCoords_p;
    MATRIXWORLD_setCell(matrix_p, elementToSet.row, elementToSet.col, true);
    currentCoords_p++;
  }
  matrix_p->noOfBlockedCells = noOfElementsToBlock;
  matrix_p->noOfUnblockedCells =
      matrix_p->noOfUnblockedCells - noOfElementsToBlock;
  return true;
}

bool MATRIXWORLD_matrixIsEmpty(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: WorldMatrix is uninitialized\n");
  return matrix_p->noOfBlockedCells == 0;
}

void MATRIXWORLD_setCell(WorldMatrix *const matrix_p, uint16_t row,
                         uint16_t col, bool state) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "ERROR: Trying to setCell but WorldMatrix is uninitialized!\n");
  MATRIXWORLD_internal_checkSizeBoundaries(
      matrix_p, row, col,
      "ERROR: Trying to setCell in WorldMatrix for cell(%d,%d), but the cell "
      "coordinates are out of bounds for this matrix!\n",
      row, col);

  uint32_t oneDimMappedIndex = (uint32_t)((row * matrix_p->cols) + col);

  if (matrix_p->worldMatrix[oneDimMappedIndex] != state) {
    matrix_p->worldMatrix[oneDimMappedIndex] = state;
    matrix_p->noOfBlockedCells = state ? (matrix_p->noOfBlockedCells + 1)
                                       : (matrix_p->noOfBlockedCells - 1);
    matrix_p->noOfUnblockedCells = state ? (matrix_p->noOfUnblockedCells - 1)
                                         : (matrix_p->noOfUnblockedCells + 1);
  } else {
    fprintf(stdout,
            "ABNORMAL: The cell state for cell (%d,%d) has not been changed as "
            "it was already in state=%d\n",
            row, col, state);
  }
}

void MATRIXWORLD_clearMatrix(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p,
      "ERROR: Trying to clearMatrix but WorldMatrix is uninitialized!\n");
  if (matrix_p->noOfBlockedCells != 0) {
    memset(matrix_p->worldMatrix, 0, (sizeof(bool) * matrix_p->worldSize));
    matrix_p->noOfBlockedCells = 0;
    matrix_p->noOfUnblockedCells = matrix_p->worldSize;
  } else {
    fprintf(stdout, "ABNORMAL: Attempting to clear the WorldMatrix but it is "
                    "empty already\n");
  }
}

uint16_t MATRIXWORLD_getRowSize(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: WorldMatrix is uninitialized\n");
  return matrix_p->rows;
}

uint16_t MATRIXWORLD_getColSize(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: WorldMatrix is uninitialized\n");
  return matrix_p->cols;
}

uint16_t MATRIXWORLD_countUnblockedNeighbors(WorldMatrix *const matrix_p,
                                             uint16_t row, uint16_t col) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: WorldMatrix is uninitialized\n");
  MATRIXWORLD_internal_checkSizeBoundaries(
      matrix_p, row, col,
      "ERROR: Trying to countUnblockedNeighbors in WorldMatrix, but the cell "
      "coordinates are out of bounds for this matrix!\n");

  uint16_t unblockedNeighbours = 0;
  // Note: This function relies on a `directions` array and `FOUR_DIRECTIONS` define
  // located in the someLibrary.h header file
  for (uint8_t index = 0; index < FOUR_DIRECTIONS; index++) {
    int16_t newRow = row + directions[index].row;
    int16_t newCol = col + directions[index].col;

    // Check if the new coordinates are within matrix boundaries
    if (newRow >= 0 && newRow < matrix_p->rows && newCol >= 0 &&
        newCol < matrix_p->cols) {
      if (!MATRIXWORLD_isBlocked(matrix_p, (uint16_t)newRow,
                                 (uint16_t)newCol)) {
        unblockedNeighbours++;
      }
    }
  }
  return unblockedNeighbours;
}

bool MATRIXWORLD_isBlocked(WorldMatrix *const matrix_p, uint16_t row,
                           uint16_t col) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: Trying to check if the cell isBlocked but the "
                "matrix is uninitialized\n");
  MATRIXWORLD_internal_checkSizeBoundaries(
      matrix_p, row, col,
      "ERROR: Trying to check if cell (%d,%d) isBlocked in WorldMatrix, but "
      "the cell coordinates are out of bounds for this matrix!\n",
      row, col);

  uint32_t oneDimMappedIndex = (uint32_t)((row * matrix_p->cols) + col);

  return matrix_p->worldMatrix[oneDimMappedIndex];
}

uint16_t MATRIXWORLD_getNoOfUnblockedCells(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get noOfUnblockedCells, but "
                "WorldMatrix is uninitialized\n");
  return matrix_p->noOfUnblockedCells;
}

uint16_t MATRIXWORLD_getNoOfBlockedCells(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get noOfBlockedCells, but "
                "WorldMatrix is uninitialized\n");
  return matrix_p->noOfBlockedCells;
}

double MATRIXWORLD_getBlockedToUnblockedRatio(WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get blockedToUnblockedRatio, but "
                "WorldMatrix is uninitialized\n");
  if (matrix_p->noOfBlockedCells == 0 || matrix_p->noOfUnblockedCells == 0) {
    fprintf(stderr,
            "ABNORMAL: Division by zero when getBlockedToUnblockedRatio "
            "noOfBlockedCells=%d noOfUnblockedCells=%d\n",
            matrix_p->noOfBlockedCells, matrix_p->noOfUnblockedCells);
    return 1.0;
  }
  return (double)matrix_p->noOfBlockedCells / matrix_p->noOfUnblockedCells;
}

size_t MATRIXWORLD_getSize(const WorldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get matrixWorld size, but "
                "WorldMatrix is uninitialized\n");
  return matrix_p->worldSize;
}

/* > Local Function Definitions **********************************************/

static void
MATRIXWORLD_internal_nullCheck(const WorldMatrix *const matrix_p,
                               const char *restrict message) {
  if (matrix_p == NULL) {
    // Critical, unrecoverable error
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE); // Exit with a failure status
  }
}

static void
MATRIXWORLD_internal_checkSizeBoundaries(const WorldMatrix *const matrix_p, uint16_t row,
                                         uint16_t col,
                                         const char *restrict message, ...) {
  if (row >= matrix_p->rows || col >= matrix_p->cols) {
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    exit(EXIT_FAILURE);
  }
}
