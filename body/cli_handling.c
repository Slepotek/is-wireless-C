/**
 * @file cli_handling.c
 * @brief Implements the command-line argument parsing and management logic.
 *
 * This file provides the core functionality for parsing CLI arguments, validating
 * them, and populating the Parameters struct for use by the main application.
 */
#include "cli_handling.h"
#include "utilities.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* > Local Function Declarations ******************************************************************/

/**
 * @brief Parses a string into a 16-bit unsigned integer.
 *
 * @param str[in]   The string to parse.
 * @param value[out] Pointer to store the parsed value.
 * @return           True on success, false on failure.
 */
static bool CLI_HANDLING_internal_parseUint16Arg(const char *str, uint16_t *value);

/**
 * @brief Parses a string into a 32-bit unsigned integer.
 *
 * @param str[in]   The string to parse.
 * @param value[out] Pointer to store the parsed value.
 * @return           True on success, false on failure.
 */
static bool CLI_HANDLING_internal_parseUint32Arg(const char *str, uint32_t *value);

/**
 * @brief Adds a new coordinate to the blockedCells array in the Parameters struct.
 *
 * @param params[in,out] The Parameters struct to modify.
 * @param row[in]        The row of the blocked cell.
 * @param col[in]        The column of the blocked cell.
 * @return               True on success, false on memory allocation failure.
 */
static bool CLI_HANDLING_internal_addBlockedCell(Parameters *params, uint16_t row, uint16_t col);

/**
 * @brief Parses a file containing blocked cell coordinates and adds them to the Parameters.
 *
 * @param filePath[in]   Path to the file to parse.
 * @param params[in,out] The Parameters struct to populate.
 * @return               True on success, false on failure.
 */
static bool CLI_HANDLING_internal_parseBlockedCellsFile(const char *filePath, Parameters *params);

/* > Public Function Definitions ******************************************************************/

void CLI_printHelp(void) {
  printf("(pathFinder - Adaptive Path Finding in NxM Matrix\n\n"
         "USAGE:\n"
         "    pathFinder --rows R --cols C --pathLength N [OPTIONS]\n\n"
         "REQUIRED:\n"
         "    --rows R                Number of matrix rows (e.g., --rows 5)\n"
         "    --cols C                Number of matrix columns (e.g., --cols 5)\n"
         "    --pathLength N          Target path length (e.g., --pathLength 12)\n\n"
         "OPTIONAL:\n"
         "    --blockedCells COORDS   Blocked cell coordinates (e.g., --blockedCells {1,0} {2,1})\n"
         "    --blockedCellsFile FILE Path to file containing blocked cell coordinates\n"
         "    --multithreading        Flag enabling the execution of the program on parallel threads\n"
         "    --help, -h              Show this help message\n\n"
         "EXAMPLES:\n"
         "    pathFinder --rows 5 --cols 5 --pathLength 6\n"
         "    pathFinder --rows 8 --cols 8 --pathLength 12 --blockedCells {1,0} {2,0} {1,1}\n"
         "    pathFinder --rows 100 --cols 100 --pathLength 50 --blockedCellsFile blocked_cells.txt\n\n"
         "BLOCKED CELLS FILE FORMAT:\n"
         "    Each line should contain: row,col\n"
         "    Lines starting with # are treated as comments\n"
         "    Example file content:\n"
         "        # Blocked cells for test matrix\n"
         "        0,1\n"
         "        1,0\n"
         "        2,2\n\n"
         "NOTES:\n"
         "    - Matrix cells are 0-indexed\n"
         "    - Path finds contiguous route through unblocked cells (value 0)\n"
         "    - Blocked cells have value 1 and cannot be traversed\n"
         "    - Algorithm uses DFS with smart starting point selection\n"
         ")\n");
}

Parameters *CLI_parseCliCommands(int argc, char *argv[]) {
    Parameters *params = malloc(sizeof(Parameters));
    if (!params) {
        perror("Failed to allocate memory for parameters");
        return NULL;
    }
    // Set default values
    *params = (Parameters){
                           .rows = 0,
                           .cols = 0,
                           .pathLength = 0,
                           .blockedCells = NULL,
                           .blockedCellsCount = 0,
                           .blockedCellsFile = NULL,
                           .isMultithreading = false
                          };

    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            CLI_printHelp();
            free(params); // Free memory before successful exit
            exit(EXIT_SUCCESS);
        } else if (strcmp(arg, "--rows") == 0) {
            if (++i >= argc || !CLI_HANDLING_internal_parseUint16Arg(argv[i], &params->rows)) {
                fprintf(stderr, "Error: Invalid or missing argument for --rows\n");
                goto error_exit;
            }
        } else if (strcmp(arg, "--cols") == 0) {
            if (++i >= argc || !CLI_HANDLING_internal_parseUint16Arg(argv[i], &params->cols)) {
                fprintf(stderr, "Error: Invalid or missing argument for --cols\n");
                goto error_exit;
            }
        } else if (strcmp(arg, "--pathLength") == 0) {
            if (++i >= argc || !CLI_HANDLING_internal_parseUint32Arg(argv[i], &params->pathLength)) {
                fprintf(stderr, "Error: Invalid or missing argument for --pathLength\n");
                goto error_exit;
            }
        } else if (strcmp(arg, "--blockedCellsFile") == 0) {
          if (++i >= argc) {
            fprintf(stderr,
                    "Error: Missing file path for --blockedCellsFile\n");
            goto error_exit;
          }
          params->blockedCellsFile = argv[i];
        } else if (strcmp(arg, "--multithreading") == 0) {
          params->isMultithreading = true;
        } else if (strcmp(arg, "--blockedCells") == 0) {
          while (i + 1 < argc && argv[i + 1][0] == '{') {
            i++;
            uint16_t row, col;
            if (sscanf(argv[i], "{%hu,%hu}", &row, &col) != 2) {
              fprintf(stderr, "Error: Invalid format for --blockedCells. "
                              "Expected {row,col}.\n");
              goto error_exit;
            }
            if (!CLI_HANDLING_internal_addBlockedCell(params, row, col)) {
              goto error_exit;
            }
          }
        } else {
          fprintf(stderr, "Error: Unknown option '%s'\n", arg);
          goto error_exit;
        }
    }

    if (params->blockedCellsFile && !CLI_HANDLING_internal_parseBlockedCellsFile(params->blockedCellsFile, params)) {
        goto error_exit;
    }

    if (params->rows == 0 || params->cols == 0 || params->pathLength == 0) {
        fprintf(stderr, "Error: Missing required arguments. --rows, --cols, and --pathLength must be provided.\n");
        goto error_exit;
    }

    return params;

error_exit:
    CLI_destroyParameters(params);
    return NULL;
}

void CLI_destroyParameters(Parameters *params) {
    if (params) {
        free(params->blockedCells);
        free(params);
    }
}

/* > Local Function Definitions *******************************************************************/

static bool CLI_HANDLING_internal_parseUint16Arg(const char *str, uint16_t *value) {
    char *end;
    errno = 0;
    long val = strtol(str, &end, 10);
    if (errno || end == str || *end != '\0' || val < 0 || val > UINT16_MAX) {
        return false;
    }
    *value = (uint16_t)val;
    return true;
}

static bool CLI_HANDLING_internal_parseUint32Arg(const char *str, uint32_t *value) {
    char *end;
    errno = 0;
    long val = strtol(str, &end, 10);
    if (errno || end == str || *end != '\0' || val < 0 || val > UINT32_MAX) {
        return false;
    }
    *value = (uint32_t)val;
    return true;
}

static bool CLI_HANDLING_internal_addBlockedCell(Parameters *params, uint16_t row, uint16_t col) {
    uint32_t count = params->blockedCellsCount;
    Cords *new_cells = realloc(params->blockedCells, (count + 1) * sizeof(Cords));
    if (!new_cells) {
        perror("Failed to allocate memory for blocked cells");
        return false;
    }
    params->blockedCells = new_cells;
    params->blockedCells[count] = (Cords){.row = row, .col = col};
    params->blockedCellsCount++;
    return true;
}

static bool CLI_HANDLING_internal_parseBlockedCellsFile(const char *filePath, Parameters *params) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening blocked cells file");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        uint16_t row, col;
        if (sscanf(line, "%hu,%hu", &row, &col) != 2) {
            fprintf(stderr, "Warning: Skipping malformed line in %s: %s", filePath, line);
            continue;
        }
        if (!CLI_HANDLING_internal_addBlockedCell(params, row, col)) {
            fclose(file);
            return false;
        }
    }

    fclose(file);
    return true;
}