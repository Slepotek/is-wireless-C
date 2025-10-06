#include "dfsPathFinding.h"
#include "matrixWorld.h"
#include "path.h"
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

void test_finds_valid_path_in_open_matrix() {
    printf("Testing: Find Valid Path in Open Matrix\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    const uint32_t path_length = 12;

    Path* path = DFS_findPath(matrix, path_length, false);
    assert(path != NULL);
    assert(PATH_getLength(path) == path_length);
    assert(PATH_isContiguous(path));

    PATH_freePath(&path);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Find Valid Path in Open Matrix\n");
}

void test_returns_null_for_impossible_length() {
    printf("Testing: Return NULL for Impossible Length\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(5, 5);
    const uint32_t impossible_length = 30; // More than 25 cells

    Path* path = DFS_findPath(matrix, impossible_length, false);
    assert(path == NULL);

    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Return NULL for Impossible Length\n");
}

void test_returns_null_for_fully_blocked_matrix() {
    printf("Testing: Return NULL for Fully Blocked Matrix\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(8, 8);
    for (uint16_t r = 0; r < 8; ++r) {
        for (uint16_t c = 0; c < 8; ++c) {
            MATRIXWORLD_setCell(matrix, r, c, true);
        }
    }

    Path* path = DFS_findPath(matrix, 5, false);
    assert(path == NULL);

    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Return NULL for Fully Blocked Matrix\n");
}

void test_finds_valid_path_in_open_matrix_multithreaded() {
    printf("Testing: Find Valid Path in Open Matrix (Multithreaded)\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    const uint32_t path_length = 12;

    // The only change is this 'true' parameter
    Path* path = DFS_findPath(matrix, path_length, true);

    assert(path != NULL);
    assert(PATH_getLength(path) == path_length);
    assert(PATH_isContiguous(path));

    PATH_freePath(&path);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Find Valid Path in Open Matrix (Multithreaded)\n");
}

int main(void) {
    printf("--- Running DfsPathFinding Tests ---\n");
    test_finds_valid_path_in_open_matrix();
    test_returns_null_for_impossible_length();
    test_returns_null_for_fully_blocked_matrix();
    test_finds_valid_path_in_open_matrix_multithreaded();
    printf("--- All DfsPathFinding Tests Passed ---\n");
    return 0;
}
