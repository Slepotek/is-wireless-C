/**
 * @file matrixWorld.h
 * @brief Public API for the worldMatrix data structure.
 *
 * This header file defines the public interface for creating, manipulating, and
 * querying the worldMatrix. It includes type definitions for the matrix and
 * coordinate structures, and prototypes for all publicly accessible functions.
 * It is intended to be used by other modules that need to interact with the
 * world grid.
 */

/* > Multiple Inclusion Protection ****************************************/
#ifndef MATRIX_WORLD_H
#define MATRIX_WORLD_H
/* > Includes *************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* > Defines **************************************************************/

/* > Type Declarations ****************************************************/

/**
 * @brief Opaque pointer to the internal worldMatrix structure.
 * @see worldMatrix
 */
typedef struct worldMatrix worldMatrix;

/**
 * @brief A structure to hold 2D coordinates.
 */
typedef struct {
  uint16_t row; /**< The row coordinate. */
  uint16_t col; /**< The column coordinate. */
} Cords;

/* > Constant Declarations ************************************************/

/* > Variable Declarations ************************************************/

/* > Function Declarations ************************************************/

/**
 * @brief Initializes a new worldMatrix instance.
 * @param rows[in] The number of rows for the new matrix.
 * @param cols[in] The number of columns for the new matrix.
 * @return A pointer to the newly created worldMatrix.
 */
worldMatrix *MATRIXWORLD_matrixInitialization(uint16_t rows, uint16_t cols);

/**
 * @brief Frees the memory allocated for a worldMatrix.
 * @param matrix_pp[in] Pointer to the pointer of the worldMatrix to be freed.
 */
void MATRIXWORLD_matrixFree(worldMatrix **const matrix_pp);

/**
 * @brief Resizes matrix to new dimensions.
 * @param matrix_p[in/out]  Pointer to worldMatrix that is about to be resized.
 * @param rows[in]          New number of rows.
 * @param cols[in]          New number of columns.
 * @return Pointer to newly allocated worldMatrix.
 */
worldMatrix *MATRIXWORLD_matrixResize(worldMatrix *const matrix_p,
                                      uint16_t rows, uint16_t cols);

/**
 * @brief Sets a specified number of cells to a "blocked" state.
 * @param matrix_p[in]            Pointer to the worldMatrix.
 * @param coordinates_p[in]       Pointer to an array of Cords to be blocked.
 * @param noOfElementsToBlock[in] The number of elements in the coordinates array.
 * @return true if the operation was successful, false otherwise.
 */
bool MATRIXWORLD_matrixBlanking(worldMatrix *const matrix_p,
                                const Cords *const coordinates_p,
                                uint16_t noOfElementsToBlock);

/**
 * @brief Checks if the matrix has any blocked cells.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return true if the matrix has no blocked cells, false otherwise.
 */
[[nodiscard]] bool MATRIXWORLD_matrixIsEmpty(worldMatrix *const matrix_p);

/**
 * @brief Sets the state of a specific cell in the matrix.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @param row[in]      The row of the cell to set.
 * @param col[in]      The column of the cell to set.
 * @param state[in]    The new state of the cell (true for blocked, false for unblocked).
 * @return true if the cell state was changed, false otherwise.
 */
bool MATRIXWORLD_setCell(worldMatrix *const matrix_p, uint16_t row,
                         uint16_t col, bool state);

/**
 * @brief Clears the matrix, setting all cells to unblocked.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return true if the matrix was cleared, false otherwise.
 */
bool MATRIXWORLD_clearMatrix(worldMatrix *const matrix_p);

/**
 * @brief Gets the number of rows in the matrix.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The number of rows.
 */
[[nodiscard]] uint16_t MATRIXWORLD_getRowSize(worldMatrix *const matrix_p);

/**
 * @brief Gets the number of columns in the matrix.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The number of columns.
 */
[[nodiscard]] uint16_t MATRIXWORLD_getColSize(worldMatrix *const matrix_p);

/**
 * @brief Counts unblocked neighbors in 4 cardinal directions.
 * @param matrix_p[in]  Pointer to worldMatrix structure.
 * @param row[in]       Row coordinate of center cell.
 * @param col[in]       Column coordinate of center cell.
 * @return Count of unblocked neighbors (0-4).
 */
[[nodiscard]] uint16_t
MATRIXWORLD_countUnblockedNeighbors(worldMatrix *const matrix_p, uint16_t row,
                                    uint16_t col);

/**
 * @brief Checks if a specific cell is blocked.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @param row[in]      The row of the cell to check.
 * @param col[in]      The column of the cell to check.
 * @return true if the cell is blocked, false otherwise.
 */
[[nodiscard]] bool MATRIXWORLD_isBlocked(worldMatrix *const matrix_p,
                                         uint16_t row, uint16_t col);

/**
 * @brief Gets the number of unblocked cells in the matrix.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The number of unblocked cells.
 */
[[nodiscard]] uint16_t
MATRIXWORLD_getNoOfUnblockedCells(worldMatrix *const matrix_p);

/**
 * @brief Gets the number of blocked cells in the matrix.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The number of blocked cells.
 */
[[nodiscard]] uint16_t
MATRIXWORLD_getNoOfBlockedCells(worldMatrix *const matrix_p);

/**
 * @brief Calculates the ratio of blocked to unblocked cells.
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The ratio of blocked to unblocked cells.
 */
[[nodiscard]] double
MATRIXWORLD_getBlockedToUnblockedRatio(worldMatrix *const matrix_p);

/**
 * @brief Gets the total size of the matrix (rows * cols).
 * @param matrix_p[in] Pointer to the worldMatrix.
 * @return The total size of the matrix.
 */
[[nodiscard]] size_t MATRIXWORLD_getSize(worldMatrix *const matrix_p);

/* > End of Multiple Inclusion Protection *********************************/
#endif
