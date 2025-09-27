/**
 * @file matrixWorld.c
 * @brief Implements the worldMatrix data structure and its manipulation functions.
 *
 * This file provides the core functionality for creating, modifying, and querying a 2D matrix
 * that represents the world. The matrix consists of cells that can be either "blocked" or "unblocked",
 * forming the basis for pathfinding algorithms. It includes functions for initialization, memory management,
 * cell state manipulation, and querying matrix properties.
 */

/* > Includes ****************************************************************/
#include "matrixWorld.h"
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* > Defines *****************************************************************/
#define FOUR_DIRECTIONS 4
/* > Type Declarations *******************************************************/ 
struct worldMatrix {
  uint16_t rows;
  uint16_t cols;
  uint32_t noOfUnblockedCells;
  uint32_t noOfBlockedCells;
  size_t worldSize;
  bool worldMatrix[];
};

typedef struct {
  int8_t row;
  int8_t col;
} fourWayDirections;
/* > Global Constant Definitions *********************************************/

/* > Global Variable Definitions *********************************************/

/* > Local Constant Definitions **********************************************/

/* > Local Variable Definitions **********************************************/

/* > Local Function Declarations *********************************************/
static void MATRIXWORLD_internal_nullCheck(worldMatrix *matrix_p,
                                           const char *restrict message);
static void
MATRIXWORLD_internal_checkSizeBoundaries(worldMatrix *matrix_p, uint16_t row,
                                         uint16_t col,
                                         const char *restrict message, ...);
/* > Global Function Definitions *********************************************/

/**
 * @brief Initializes a new worldMatrix instance.
 *
 * Allocates memory for a worldMatrix structure and its internal boolean array
 * representing the grid. All cells are initialized to the "unblocked" state (false).
 *
 * @param rows[in] The number of rows for the new matrix.
 * @param cols[in] The number of columns for the new matrix.
 * @return A pointer to the newly allocated and initialized worldMatrix.
 *         Exits with a fatal error if memory allocation fails.
 */
worldMatrix *MATRIXWORLD_matrixInitialization(uint16_t rows, uint16_t cols) {
  size_t noOfMatrixBoolElements = rows * cols;
  worldMatrix *matrix_p = (worldMatrix *)malloc(
      sizeof(worldMatrix) + (sizeof(bool) * noOfMatrixBoolElements));
  MATRIXWORLD_internal_nullCheck(
      matrix_p,
      "FATAL ERROR: Failed to allocate memory for the worldMatrix.\n");
  matrix_p->rows = rows;
  matrix_p->cols = cols;
  matrix_p->worldSize = noOfMatrixBoolElements;
  matrix_p->noOfBlockedCells = 0;
  matrix_p->noOfUnblockedCells = (uint32_t)noOfMatrixBoolElements;
  memset(matrix_p->worldMatrix, 0,
         (uint64_t)(sizeof(bool) * noOfMatrixBoolElements));

  return matrix_p;
}

/**
 * @brief Frees the memory allocated for a worldMatrix.
 *
 * @param matrix_pp[in] Pointer to the pointer of the worldMatrix to be freed.
 */
void MATRIXWORLD_matrixFree(worldMatrix **const matrix_pp) { free(*matrix_pp); }

/**
 * @brief Resizes matrix to new dimensions with error handling
 *
 * Resizes the worldMatrix by freeing current instance of matrix and creating
 * new one. Data stored in the current matrix is erased.
 *
 * @param matrix_p[in/out]  Pointer to worldMatrix that is about to be resized
 * @param rows[in]          New number of rows
 * @param cols[in]          New number of columns
 *
 * @return Pointer to newly allocated worldMatrix
 */
worldMatrix *MATRIXWORLD_matrixResize(worldMatrix *matrix_p, uint16_t rows,
                                      uint16_t cols) {
  // For memory safety just free and reinitialize the matrix
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: Trying to resize uninitialized matrix\n");
  MATRIXWORLD_matrixFree(&matrix_p);
  return MATRIXWORLD_matrixInitialization(rows, cols);
}

/**
 * @brief Sets a specified number of cells to a "blocked" state.
 *
 * Iterates through a list of coordinates and sets each corresponding cell
 * to the "blocked" state (true). Exits with a fatal error if the number of
 * elements to block exceeds the total size of the matrix.
 *
 * @param matrix_p[in]            Pointer to the worldMatrix.
 * @param coordinates_p[in]       Pointer to an array of Cords to be blocked.
 * @param noOfElementsToBlock[in] The number of elements in the coordinates array.
 *
 * @return true if the operation was successful, false otherwise.
 */
bool MATRIXWORLD_matrixBlanking(worldMatrix *const matrix_p,
                                const Cords *coordinates_p,
                                uint16_t noOfElementsToBlock) {
  if (noOfElementsToBlock > matrix_p->worldSize) {
    fprintf(
        stderr,
        "FATAL ERROR: Can not blank more elements than a matrix can hold\n");
    exit(EXIT_FAILURE); // Exit with a failure status
  }
  for (uint16_t index = 0; index < noOfElementsToBlock; index++) {
    // For easier handling
    Cords elementToSet = *coordinates_p;
    if (!MATRIXWORLD_setCell(matrix_p, elementToSet.row, elementToSet.col,
                             true))
      return false;
    coordinates_p++;
  }
  matrix_p->noOfBlockedCells = noOfElementsToBlock;
  matrix_p->noOfUnblockedCells =
      matrix_p->noOfUnblockedCells - noOfElementsToBlock;
  return true;
}

/**
 * @brief Checks if the matrix has any blocked cells.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return true if the matrix has no blocked cells, false otherwise.
 */
bool MATRIXWORLD_matrixIsEmpty(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: worldMatrix is uninitialized\n");
  if (matrix_p->noOfBlockedCells == 0) {
    return true;
  }
  return false;
}

/**
 * @brief Sets the state of a specific cell in the matrix.
 *
 * Updates the state of a cell (blocked/unblocked) and adjusts the
 * blocked/unblocked cell counters accordingly.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @param row[in]      The row of the cell to set.
 * @param col[in]      The column of the cell to set.
 * @param state[in]    The new state of the cell (true for blocked, false for unblocked).
 *
 * @return true if the cell state was changed or was already the desired state,
 *         logs an abnormal message if the state was not changed.
 */
bool MATRIXWORLD_setCell(worldMatrix *const matrix_p, uint16_t row,
                         uint16_t col, bool state) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "ERROR: Trying to setCell but worldMatrix is uninitialized!\n");
  MATRIXWORLD_internal_checkSizeBoundaries(
      matrix_p, row, col,
      "ERROR: Trying to setCell in worldMatrix for cell(%d,%d), but the cell "
      "coordinates are out of bounds for this matrix!\n",
      row, col);

  bool result = false;
  uint32_t oneDimMappedIndex = (uint32_t)((row * matrix_p->cols) + col);

  if (matrix_p->worldMatrix[oneDimMappedIndex] != state) {
    matrix_p->worldMatrix[oneDimMappedIndex] = state;
    matrix_p->noOfBlockedCells = state ? (matrix_p->noOfBlockedCells + 1)
                                       : (matrix_p->noOfBlockedCells - 1);
    matrix_p->noOfUnblockedCells = state ? (matrix_p->noOfUnblockedCells - 1)
                                         : (matrix_p->noOfUnblockedCells + 1);
    result = true;
  } else {
    result = true;
    fprintf(stdout,
            "ABNORMAL: The cell state for cell (%d,%d) has not been changed as "
            "it was "
            "already in state=%d\n",
            row, col, state);
  }

  return result;
}

/**
 * @brief Clears the matrix, setting all cells to unblocked.
 *
 * Resets all cells in the matrix to the "unblocked" state (false) and
 * updates the blocked/unblocked cell counters.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return true if the matrix was cleared or was already empty.
 */
bool MATRIXWORLD_clearMatrix(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p,
      "ERROR: Trying to clearMatrix but worldMatrix is uninitialized!\n");
  bool result = false;
  if (matrix_p->noOfBlockedCells != 0) {
    memset(matrix_p->worldMatrix, 0, (sizeof(bool) * matrix_p->worldSize));
    matrix_p->noOfBlockedCells = 0;
    matrix_p->noOfUnblockedCells = matrix_p->worldSize;
    result = true;
  } else {
    result = true;
    fprintf(stdout, "ABNORMAL: Attempting to clear the worldMatrix but it is "
                    "empty already\n");
  }
  return result;
}

/**
 * @brief Gets the number of rows in the matrix.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The number of rows.
 */
uint16_t MATRIXWORLD_getRowSize(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: worldMatrix is uninitialized\n");
  return matrix_p->rows;
}

/**
 * @brief Gets the number of columns in the matrix.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The number of columns.
 */
uint16_t MATRIXWORLD_getColSize(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: worldMatrix is uninitialized\n");
  return matrix_p->cols;
}

/**
 * @brief Counts unblocked neighbors in 4 cardinal directions
 *
 * Implements 4-directional neighbor analysis (up, down, left, right).
 * Validates center coordinates first, then checks each neighbor position
 * for bounds and blocked state.
 *
 * @param matrix_p[in]  Pointer to worldMatrix structure
 * @param row[in]       Row coordinate of center cell
 * @param col[in]       Column coordinate of center cell
 *
 * @return Count of unblocked neighbors (0-4), or fatal error if center
 * coordinates invalid
 */
uint16_t MATRIXWORLD_countUnblockedNeighbors(worldMatrix *const matrix_p,
                                             uint16_t row, uint16_t col) {
  MATRIXWORLD_internal_nullCheck(matrix_p,
                                 "FATAL ERROR: worldMatrix is uninitialized\n");
  MATRIXWORLD_internal_checkSizeBoundaries(
      matrix_p, row, col,
      "ERROR: Trying to countUnblockedNeighbors in worldMatrix, but the cell "
      "coordinates are out of bounds for this matrix!\n");

  uint16_t unblockedNeighbours = 0;

  fourWayDirections directions[4] = {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};

  for (uint8_t index = 0; index < FOUR_DIRECTIONS; index++) {
    uint16_t newRow = 0;
    uint16_t newCol = 0;
    // border case check for row
    if (row == 0 && directions[index].row == -1) {
      continue;
    } else {
      newRow = row + directions[index].row;
    }
    // border case check for col
    if (col == 0 && directions[index].col == -1) {
      continue;
    } else {
      newCol = col + directions[index].col;
    }

    if (((newRow < matrix_p->rows)) && ((newCol < matrix_p->cols))) {
      if (!MATRIXWORLD_isBlocked(matrix_p, newRow, newCol)) {
        unblockedNeighbours++;
      }
    }
  }
  return unblockedNeighbours;
}

/**
 * @brief Checks if a specific cell is blocked.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @param row[in]      The row of the cell to check.
 * @param col[in]      The column of the cell to check.
 *
 * @return true if the cell is blocked, false otherwise.
 */
bool MATRIXWORLD_isBlocked(worldMatrix *const matrix_p, uint16_t row,
                           uint16_t col) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: Trying to check if the cell isBlocked but the "
                "matrix is uninitialized\n");
  MATRIXWORLD_internal_checkSizeBoundaries(
      matrix_p, row, col,
      "ERROR: Trying to check if cell (%d,%d) isBlocked in worldMatrix, but "
      "the cell "
      "coordinates are out of bounds for this matrix!\n",
      row, col);

  uint32_t oneDimMappedIndex = (uint32_t)((row * matrix_p->cols) + col);

  return matrix_p->worldMatrix[oneDimMappedIndex];
}

/**
 * @brief Gets the number of unblocked cells in the matrix.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The total number of unblocked cells.
 */
uint16_t MATRIXWORLD_getNoOfUnblockedCells(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get noOfUnblockedCells, but "
                "worldMatrix is uninitialized\n");
  return matrix_p->noOfUnblockedCells;
}

/**
 * @brief Gets the number of blocked cells in the matrix.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The total number of blocked cells.
 */
uint16_t MATRIXWORLD_getNoOfBlockedCells(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get noOfBlockedCells, but "
                "worldMatrix is uninitialized\n");
  return matrix_p->noOfBlockedCells;
}

/**
 * @brief Calculates the ratio of blocked to unblocked cells.
 *
 * If either blocked or unblocked cell count is zero, this function will
 * log an abnormal message and return 1.0 to avoid division by zero.
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The ratio as a double.
 */
double MATRIXWORLD_getBlockedToUnblockedRatio(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get blockedToUnblockedRatio, but "
                "worldMatrix is uninitialized\n");
  if (matrix_p->noOfBlockedCells == 0 || matrix_p->noOfUnblockedCells == 0) {
    // Division by zero, note the abnormal, but return the ratio as 1
    fprintf(stderr,
            "ABNORMAL: Division by zero when getBlockedToUnblockedRatio "
            "noOfBlockedCells=%d noOfUnblockedCells=%d\n",
            matrix_p->noOfBlockedCells, matrix_p->noOfUnblockedCells);
    return 1;
  }
  return (double)matrix_p->noOfBlockedCells / matrix_p->noOfUnblockedCells;
}

/**
 * @brief Gets the total size of the matrix (rows * cols).
 *
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The total number of cells in the matrix.
 */
size_t MATRIXWORLD_getSize(worldMatrix *const matrix_p) {
  MATRIXWORLD_internal_nullCheck(
      matrix_p, "FATAL ERROR: trying to get matrixWorld size, but "
                "worldMatrix is uninitialized\n");
  return matrix_p->worldSize;
}

/* > Local Function Definitions **********************************************/

/**
 * @brief Internal function to check for null pointers and exit on failure.
 *
 * If the provided pointer is NULL, this function prints a critical error
 * message to stderr and terminates the program.
 *
 * @param matrix_p[in]  Pointer to the worldMatrix.
 * @param message[in]   The error message to print on failure.
 */
static void MATRIXWORLD_internal_nullCheck(worldMatrix *matrix_p,
                                           const char *restrict message) {
  if (matrix_p == NULL) {
    // Critical, unrecoverable error
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE); // Exit with a failure status
  }
}

/**
 * @brief Internal function to check for out-of-bounds access and exit on failure.
 *
 * If the provided row or column are outside the matrix dimensions, this
 * function prints a formatted error message to stderr and terminates the program.
 *
 * @param matrix_p[in]  Pointer to the worldMatrix.
 * @param row[in]       The row to check.
 * @param col[in]       The column to check.
 * @param message[in]   The formatted error message to print on failure.
 * @param ...           Variable arguments for the format string.
 */
static void
MATRIXWORLD_internal_checkSizeBoundaries(worldMatrix *matrix_p, uint16_t row,
                                         uint16_t col,
                                         const char *restrict message, ...) {
  if (row >= matrix_p->rows || col >= matrix_p->cols) {
    va_list args;
    va_start(args, message);         // Initialize va_list
    vfprintf(stderr, message, args); // Print formatted string to stderr
    va_end(args);                    // Clean up va_list
    exit(EXIT_FAILURE);              // Exit with a failure status
  }
}