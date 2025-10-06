#include "cli_handling.h"
#include "dfsPathFinding.h"
#include "matrixWorld.h"
#include "path.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // 1. Parse Command-Line Arguments
  Parameters *params = CLI_parseCliCommands(argc, argv);
  if (params == NULL) {
    fprintf(stderr, "Error: Failed to parse command-line arguments.\n");
    return EXIT_FAILURE;
  }

  printf("--- Path Finder Initializing ---\n");
  printf("Matrix Dimensions: %u rows, %u cols\n", params->rows, params->cols);
  printf("Target Path Length: %u\n", params->pathLength);
  if (params->blockedCellsCount > 0) {
    printf("Blocked Cells Provided: %u\n", params->blockedCellsCount);
  }
  printf("--------------------------------\n\n");

  // 2. Initialize World Matrix
  WorldMatrix *world =
      MATRIXWORLD_matrixInitialization(params->rows, params->cols);
  if (world == NULL) {
    fprintf(stderr, "Error: Failed to initialize the world matrix.\n");
    CLI_destroyParameters(params);
    return EXIT_FAILURE;
  }

  // 3. Set Blocked Cells
  if (params->blockedCellsCount > 0) {
    MATRIXWORLD_matrixBlanking(world, params->blockedCells,
                               params->blockedCellsCount);
  }

  // 4. Run Pathfinding Algorithm
  printf("Searching for a path...\n");
  Path *foundPath = DFS_findPath(world, params->pathLength, params->isMultithreading);

  // 5. Report Results
  if (foundPath != NULL) {
    printf("\n--- Path Found!---\n");
    PATH_printPath(foundPath);
    printf("--------------------\n");
  } else {
    printf("\n--- No Valid Path Found ---\n");
  }

  // 6. Clean Up Resources
  printf("\nCleaning up resources...\n");
  PATH_freePath(&foundPath);
  MATRIXWORLD_matrixFree(&world);
  CLI_destroyParameters(params);

  printf("Done.\n");
  return EXIT_SUCCESS;
}