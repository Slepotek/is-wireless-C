/**
 * @file path.h
 * @author Slepotek (slepotek.dev@gmail.com)
 * @brief Public API for the Path data structure.
 * @version 1.0
 * @date 2025-09-28
 *
 * This header file defines the public interface for creating and managing a
 * path of coordinates. The Path is implemented as a dynamic array with a
 * fixed capacity defined at initialization.
 */

#ifndef PATH_H
#define PATH_H

#include "matrixWorld.h"
#include "someLibFile.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Opaque pointer to the internal Path structure.
 */
typedef struct Path Path;

/**
 * @brief Initializes a new Path instance with a specified capacity.
 *
 * Allocates memory for a Path structure. The path's capacity is fixed on
 * creation and cannot be changed.
 *
 * @note The function will cause a fatal error and exit if pathSize is 0 or
 *       exceeds 75% of the total cells in the associated matrix. It also
 *       exits on memory allocation failure.
 *
 * @param pathSize[in] The maximum number of coordinates the path can hold.
 * @param matrix_p[in] A pointer to the WorldMatrix this path will belong to, used
 *                 for size validation.
 * @return A pointer to the newly allocated Path.
 */
[[nodiscard]] Path *
PATH_initializePath(const size_t pathSize, const WorldMatrix *const matrix_p);

/**
 * @brief Frees the memory allocated for a Path instance.
 *
 * @param path_pp[in,out] Pointer to the pointer of the Path to be freed.
 */
void PATH_freePath(Path **path_pp);

/**
 * @brief Adds a new coordinate to the end of the path.
 *
 * @note This function will cause a fatal error and exit if the path is full.
 *
 * @param path_p[in,out] A pointer to the Path instance.
 * @param row[in] The row of the coordinate to add.
 * @param col[in] The column of the coordinate to add.
 */
void PATH_addCoordinates(Path *path_p, uint16_t row, uint16_t col);

/**
 * @brief Retrieves the last coordinate added to the path without removing it.
 *
 * @param path_p[in] A pointer to the Path instance.
 * @return The last Cords in the path. If the path is empty, returns
 *         Cords with row and col set to UINT16_MAX.
 */
[[nodiscard]] Cords PATH_getLastCoordinates(Path *path_p);

/**
 * @brief Removes and returns the last coordinate from the path.
 *
 * The "popped" coordinate in the internal array is zeroed out.
 *
 * @param path_p[in,out] A pointer to the Path instance.
 * @return The last Cords in the path. If the path is empty, returns
 *         Cords with row and col set to UINT16_MAX.
 */
[[nodiscard]] Cords PATH_popCoordinates(Path *path_p);

/**
 * @brief Checks if the path is contiguous.
 *
 * A contiguous path is one where each coordinate is exactly one Manhattan
 * distance unit away from the previous one.
 *
 * @param path_p[in] A pointer to the Path instance.
 * @return true if the path is contiguous, false otherwise.
 */
[[nodiscard]] bool PATH_isContiguous(Path *path_p);

/**
 * @brief Checks if the path is empty.
 *
 * @param path_p[in] A pointer to the Path instance.
 * @return true if the path contains no coordinates, false otherwise.
 */
[[nodiscard]] bool PATH_isEmpty(Path *path_p);

/**
 * @param path_p[in] A pointer to the Path instance.
 * @return The number of coordinates currently stored in the path.
 */
[[nodiscard]] size_t PATH_getLength(Path *path_p);

/**
 * @brief Clears all coordinates from the path.
 *
 * Resets the path's length to 0. It does not free the path's memory.
 *
 * @param path_p[in,out] A pointer to the Path instance.
 */
void PATH_clearPath(Path *path_p);

/**
 * @brief Prints all coordinates in the path to stdout.
 *
 * @param path_p[in] A pointer to the Path instance.
 */
void PATH_printPath(Path *path_p);

#endif /* PATH_H */