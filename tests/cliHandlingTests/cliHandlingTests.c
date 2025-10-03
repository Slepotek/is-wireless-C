#include "cli_handling.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test function declarations
void test_basic_parsing();
void test_missing_required_args();
void test_invalid_arg_values();
void test_unknown_argument();
void test_blocked_cells_cli();
void test_blocked_cells_file();
void test_combined_args();

int main(void) {
  printf("--- Running cliHandling Tests ---\n");
  test_basic_parsing();
  test_missing_required_args();
  test_invalid_arg_values();
  test_unknown_argument();
  test_blocked_cells_cli();
  test_blocked_cells_file();
  test_combined_args();
  printf("--- All cliHandling Tests Passed ---\n");
  return 0;
}

void test_basic_parsing() {
  printf("Testing: Basic required arguments\n");
  char *argv[] = {"pathFinder", "--rows", "10", "--cols", "20", "--pathLength", "100"};
  int argc = sizeof(argv) / sizeof(char *);
  Parameters *params = CLI_parseCliCommands(argc, argv);
  assert(params != NULL);
  assert(params->rows == 10);
  assert(params->cols == 20);
  assert(params->pathLength == 100);
  assert(params->blockedCellsCount == 0);
  assert(params->blockedCellsFile == NULL);
  CLI_destroyParameters(params);
  printf("Passed: Basic required arguments\n");
}

void test_missing_required_args() {
  printf("Testing: Missing required arguments\n");
  char *argv1[] = {"pathFinder", "--cols", "20", "--pathLength", "100"};
  Parameters *params1 = CLI_parseCliCommands(sizeof(argv1) / sizeof(char *), argv1);
  assert(params1 == NULL);

  char *argv2[] = {"pathFinder", "--rows", "10", "--pathLength", "100"};
  Parameters *params2 = CLI_parseCliCommands(sizeof(argv2) / sizeof(char *), argv2);
  assert(params2 == NULL);

  char *argv3[] = {"pathFinder", "--rows", "10", "--cols", "20"};
  Parameters *params3 = CLI_parseCliCommands(sizeof(argv3) / sizeof(char *), argv3);
  assert(params3 == NULL);
  
  printf("Passed: Missing required arguments\n");
}

void test_invalid_arg_values() {
    printf("Testing: Invalid argument values\n");
    char *argv1[] = {"pathFinder", "--rows", "foo", "--cols", "10", "--pathLength", "10"};
    Parameters *params1 = CLI_parseCliCommands(sizeof(argv1) / sizeof(char *), argv1);
    assert(params1 == NULL);

    char *argv2[] = {"pathFinder", "--rows", "10", "--cols", "-5", "--pathLength", "10"};
    Parameters *params2 = CLI_parseCliCommands(sizeof(argv2) / sizeof(char *), argv2);
    assert(params2 == NULL);

    printf("Passed: Invalid argument values\n");
}

void test_unknown_argument() {
    printf("Testing: Unknown argument\n");
    char *argv[] = {"pathFinder", "--rows", "5", "--cols", "5", "--pathLength", "10", "--foo", "bar"};
    Parameters *params = CLI_parseCliCommands(sizeof(argv) / sizeof(char *), argv);
    assert(params == NULL);
    printf("Passed: Unknown argument\n");
}

void test_blocked_cells_cli() {
    printf("Testing: Blocked cells from CLI\n");
    char *argv[] = {"pathFinder", "--rows", "5", "--cols", "5", "--pathLength", "10", "--blockedCells", "{1,2}", "{3,4}"};
    int argc = sizeof(argv) / sizeof(char *);
    Parameters *params = CLI_parseCliCommands(argc, argv);
    assert(params != NULL);
    assert(params->blockedCellsCount == 2);
    assert(params->blockedCells[0].row == 1 && params->blockedCells[0].col == 2);
    assert(params->blockedCells[1].row == 3 && params->blockedCells[1].col == 4);
    CLI_destroyParameters(params);
    printf("Passed: Blocked cells from CLI\n");
}

void test_blocked_cells_file() {
    printf("Testing: Blocked cells from file\n");
    const char* filename = "test_blocked_cells.txt";
    FILE* file = fopen(filename, "w");
    assert(file != NULL);
    fprintf(file, "# Comment\n1,1\n2,2\n");
    fclose(file);

    char *argv[] = {"pathFinder", "--rows", "5", "--cols", "5", "--pathLength", "10", "--blockedCellsFile", (char*)filename};
    int argc = sizeof(argv) / sizeof(char *);
    Parameters *params = CLI_parseCliCommands(argc, argv);
    
    assert(params != NULL);
    assert(params->blockedCellsCount == 2);
    assert(params->blockedCells[0].row == 1 && params->blockedCells[0].col == 1);
    assert(params->blockedCells[1].row == 2 && params->blockedCells[1].col == 2);
    
    CLI_destroyParameters(params);
    remove(filename);
    printf("Passed: Blocked cells from file\n");
}

void test_combined_args() {
    printf("Testing: Combined CLI and file blocked cells\n");
    const char* filename = "test_blocked_cells_combined.txt";
    FILE* file = fopen(filename, "w");
    assert(file != NULL);
    fprintf(file, "3,3\n4,4\n");
    fclose(file);

    char *argv[] = {"pathFinder", "--rows", "10", "--cols", "10", "--pathLength", "20", "--blockedCells", "{1,1}", "{2,2}", "--blockedCellsFile", (char*)filename};
    int argc = sizeof(argv) / sizeof(char *);
    Parameters *params = CLI_parseCliCommands(argc, argv);

    assert(params != NULL);
    assert(params->blockedCellsCount == 4);
    assert(params->blockedCells[0].row == 1 && params->blockedCells[0].col == 1);
    assert(params->blockedCells[1].row == 2 && params->blockedCells[1].col == 2);
    assert(params->blockedCells[2].row == 3 && params->blockedCells[2].col == 3);
    assert(params->blockedCells[3].row == 4 && params->blockedCells[3].col == 4);

    CLI_destroyParameters(params);
    remove(filename);
    printf("Passed: Combined CLI and file blocked cells\n");
}
