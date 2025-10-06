/* > Description *******************************************************************/
/**
 * @file matrixWorld.h
 * @brief
 *   This header file defines the public interface for the WorldMatrix data
 *   structure. It provides functions for creating, manipulating, and querying
 *   the state of the 2D world grid.
 *
 *****************************************************************************/

/* > Multiple Inclusion Protection ****************************************/
#ifndef MATRIX_WORLD_H
#define MATRIX_WORLD_H

/* > Includes *************************************************************/
#include "utilities.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* > Defines **************************************************************/

/* > Type Declarations ****************************************************/

/**
 * @brief Opaque pointer to the internal WorldMatrix structure.
 */
typedef struct WorldMatrix WorldMatrix;

/* > Constant Declarations ************************************************/

/* > Variable Declarations ************************************************/

/* > Function Declarations ************************************************/

/**
 * @brief Initializes a new WorldMatrix instance.
 * @param rows[in] The number of rows for the new matrix.
 * @param cols[in] The number of columns for the new matrix.
 * @return A pointer to the newly allocated and initialized WorldMatrix.
 */
[[nodiscard]] WorldMatrix *MATRIXWORLD_matrixInitialization(uint16_t rows, uint16_t cols);

/**
 * @brief Frees the memory allocated for a WorldMatrix.
 * @param matrix_pp[in,out] Pointer to the pointer of the WorldMatrix to be freed.
 */
void MATRIXWORLD_matrixFree(WorldMatrix **const matrix_pp);

/**
 * @brief Resizes matrix to new dimensions with error handling.
 * @param matrix_p[in] Pointer to WorldMatrix that is about to be resized.
 * @param rows[in] New number of rows.
 * @param cols[in] New number of columns.
 * @return Pointer to the newly allocated WorldMatrix.
 */
[[nodiscard]] WorldMatrix *MATRIXWORLD_matrixResize(WorldMatrix *matrix_p, uint16_t rows, uint16_t cols);

/**
 * @brief Sets a specified number of cells to a "blocked" state.
 * @param matrix_p[in,out] Pointer to the WorldMatrix.
 * @param coordinates_p[in] Pointer to an array of Cords to be blocked.
 * @param noOfElementsToBlock[in] The number of elements in the coordinates array.
 * @return true if the operation was successful, false otherwise.
 */
bool MATRIXWORLD_matrixBlanking(WorldMatrix *const matrix_p, const Cords *const coordinates_p, uint16_t noOfElementsToBlock);

/**
 * @brief Checks if the matrix has any blocked cells.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return true if the matrix has no blocked cells, false otherwise.
 */
[[nodiscard]] bool MATRIXWORLD_matrixIsEmpty(WorldMatrix *const matrix_p);

/**
 * @brief Sets the state of a specific cell in the matrix.
 * @param matrix_p[in,out] Pointer to the WorldMatrix.
 * @param row[in] The row of the cell to set.
 * @param col[in] The column of the cell to set.
 * @param state[in] The new state of the cell (true for blocked, false for unblocked).
 */
void MATRIXWORLD_setCell(WorldMatrix *const matrix_p, uint16_t row, uint16_t col, bool state);

/**
 * @brief Clears the matrix, setting all cells to unblocked.
 * @param matrix_p[in,out] Pointer to the WorldMatrix.
 */
void MATRIXWORLD_clearMatrix(WorldMatrix *const matrix_p);

/**
 * @brief Gets the number of rows in the matrix.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return The number of rows.
 */
[[nodiscard]] uint16_t MATRIXWORLD_getRowSize(const WorldMatrix *const matrix_p);

/**
 * @brief Gets the number of columns in the matrix.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return The number of columns.
 */
[[nodiscard]] uint16_t MATRIXWORLD_getColSize(const WorldMatrix *const matrix_p);

/**
 * @brief Counts unblocked neighbors in 4 cardinal directions.
 * @param matrix_p[in] Pointer to WorldMatrix structure.
 * @param row[in] Row coordinate of center cell.
 * @param col[in] Column coordinate of center cell.
 * @return Count of unblocked neighbors (0-4).
 */
[[nodiscard]] uint16_t MATRIXWORLD_countUnblockedNeighbors(WorldMatrix *const matrix_p, uint16_t row, uint16_t col);

/**
 * @brief Checks if a specific cell is blocked.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @param row[in] The row of the cell to check.
 * @param col[in] The column of the cell to check.
 * @return true if the cell is blocked, false otherwise.
 */
[[nodiscard]] bool MATRIXWORLD_isBlocked(const WorldMatrix *const matrix_p, uint16_t row, uint16_t col);

/**
 * @brief Gets the number of unblocked cells in the matrix.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return The total number of unblocked cells.
 */
[[nodiscard]] uint16_t MATRIXWORLD_getNoOfUnblockedCells(const WorldMatrix *const matrix_p);

/**
 * @brief Gets the number of blocked cells in the matrix.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return The total number of blocked cells.
 */
[[nodiscard]] uint16_t MATRIXWORLD_getNoOfBlockedCells(WorldMatrix *const matrix_p);

/**
 * @brief Calculates the ratio of blocked to unblocked cells.
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return The ratio of blocked to unblocked cells.
 */
[[nodiscard]] double MATRIXWORLD_getBlockedToUnblockedRatio(WorldMatrix *const matrix_p);

/**
 * @brief Gets the total size of the matrix (rows * cols).
 * @param matrix_p[in] Pointer to the WorldMatrix.
 * @return The total number of cells in the matrix.
 */
[[nodiscard]] size_t MATRIXWORLD_getSize(const WorldMatrix *const matrix_p);

/* > End of Multiple Inclusion Protection *********************************/
#endif /* MATRIX_WORLD_H */
