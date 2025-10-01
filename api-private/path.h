/* > Description *******************************************************************/
/**
 * @file path.h
 * @brief
 *   This header file defines the public interface for the Path data structure,
 *   which represents a sequence of coordinates. It provides functions for
 *   creating, managing, and querying paths.
 *
 *****************************************************************************/

/* > Multiple Inclusion Protection ****************************************/
#ifndef PATH_H
#define PATH_H

/* > Includes *************************************************************/
#include "matrixWorld.h"
#include "utilities.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* > Defines **************************************************************/

/* > Type Declarations ****************************************************/

/**
 * @brief Opaque pointer to the internal Path structure.
 */
typedef struct Path Path;

/* > Constant Declarations ************************************************/

/* > Variable Declarations ************************************************/

/* > Function Declarations ************************************************/

/**
 * @brief Initializes a new Path instance with a specified capacity.
 * @param pathSize[in] The maximum number of coordinates the path can hold.
 * @param matrix_p[in] A pointer to the WorldMatrix for size validation.
 * @return A pointer to the newly allocated Path.
 */
[[nodiscard]] Path *PATH_initializePath(const size_t pathSize, const WorldMatrix *const matrix_p);

/**
 * @brief Frees the memory allocated for a Path instance.
 * @param path_pp[in,out] Pointer to the pointer of the Path to be freed.
 */
void PATH_freePath(Path **path_pp);

/**
 * @brief Adds a new coordinate to the end of the path.
 * @param path_p[in,out] A pointer to the Path instance.
 * @param row[in] The row of the coordinate to add.
 * @param col[in] The column of the coordinate to add.
 */
void PATH_addCoordinates(Path *path_p, uint16_t row, uint16_t col);

/**
 * @brief Retrieves the last coordinate added to the path without removing it.
 * @param path_p[in] A pointer to the Path instance.
 * @return The last Cords in the path, or Cords with UINT16_MAX if empty.
 */
[[nodiscard]] Cords PATH_getLastCoordinates(Path *path_p);

/**
 * @brief Removes and returns the last coordinate from the path.
 * @param path_p[in,out] A pointer to the Path instance.
 * @return The last Cords in the path, or Cords with UINT16_MAX if empty.
 */
[[nodiscard]] Cords PATH_popCoordinates(Path *path_p);

/**
 * @brief Checks if the path is contiguous.
 * @param path_p[in] A pointer to the Path instance.
 * @return true if the path is contiguous, false otherwise.
 */
[[nodiscard]] bool PATH_isContiguous(Path *path_p);

/**
 * @brief Checks if the path is empty.
 * @param path_p[in] A pointer to the Path instance.
 * @return true if the path contains no coordinates, false otherwise.
 */
[[nodiscard]] bool PATH_isEmpty(Path *path_p);

/**
 * @brief Gets the current number of coordinates in the path.
 * @param path_p[in] A pointer to the Path instance.
 * @return The number of coordinates currently stored in the path.
 */
[[nodiscard]] size_t PATH_getLength(Path *path_p);

/**
 * @brief Clears all coordinates from the path.
 * @param path_p[in,out] A pointer to the Path instance.
 */
void PATH_clearPath(Path *path_p);

/**
 * @brief Prints all coordinates in the path to stdout.
 * @param path_p[in] A pointer to the Path instance.
 */
void PATH_printPath(Path *path_p);

/* > End of Multiple Inclusion Protection *********************************/
#endif /* PATH_H */
