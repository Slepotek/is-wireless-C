/**
 * @file cli_handling.h
 * @brief Defines the interface for parsing and managing command-line arguments.
 *
 * This file contains the structures and function declarations necessary to handle
 * CLI input, populate a parameters structure, and clean up allocated resources.
 */

#ifndef CLI_HANDLING_H
#define CLI_HANDLING_H

#include "utilities.h"
#include <stdbool.h>
#include <stdint.h>

/* > Typedefs *************************************************************************************/

/**
 * @brief Holds all runtime parameters parsed from the command-line interface.
 */
typedef struct {
  uint16_t rows;              /**< Number of rows in the matrix. */
  uint16_t cols;              /**< Number of columns in the matrix. */
  uint32_t pathLength;        /**< The target length of the path to find. */
  Cords *blockedCells;        /**< Dynamic array of coordinates for blocked cells. */
  uint32_t blockedCellsCount; /**< Number of elements in the blockedCells array. */
  const char *blockedCellsFile; /**< Path to a file containing blocked cell coordinates. */
} Parameters;

/* > Function Declarations ************************************************************************/

/**
 * @brief Parses command-line arguments and populates a Parameters struct.
 *
 * @param argc[in] The argument count.
 * @param argv[in] The argument vector.
 * @return         A pointer to a dynamically allocated Parameters struct on success,
 *                 or NULL on failure (e.g., invalid argument, missing required option).
 *                 The caller is responsible for freeing the struct using CLI_destroyParameters.
 */
Parameters *CLI_parseCliCommands(int argc, char *argv[]);

/**
 * @brief Frees the memory allocated for a Parameters struct and its members.
 *
 * @param params[in,out] A pointer to the Parameters struct to be destroyed.
 */
void CLI_destroyParameters(Parameters *params);

/**
 * @brief Prints the help and usage message to the console.
 */
void CLI_printHelp(void);

#endif // CLI_HANDLING_H
