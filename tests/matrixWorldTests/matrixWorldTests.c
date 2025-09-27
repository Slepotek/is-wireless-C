#include "matrixWorld.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_initialization_and_sizes() {
    printf("Testing: Initialization and Sizes\n");
    worldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 20);
    assert(matrix != NULL);
    assert(MATRIXWORLD_getRowSize(matrix) == 10);
    assert(MATRIXWORLD_getColSize(matrix) == 20);
    assert(MATRIXWORLD_getSize(matrix) == 200);
    assert(!MATRIXWORLD_isBlocked(matrix, 0, 0)); // A new cell is not blocked
    assert(MATRIXWORLD_getNoOfBlockedCells(matrix) == 0);
    assert(MATRIXWORLD_getNoOfUnblockedCells(matrix) == 200);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Initialization and Sizes\n");
}

void test_set_and_get_cell() {
    printf("Testing: Set and Get Cell\n");
    worldMatrix* matrix = MATRIXWORLD_matrixInitialization(5, 5);
    assert(!MATRIXWORLD_isBlocked(matrix, 2, 2)); // A new cell is not blocked
    MATRIXWORLD_setCell(matrix, 2, 2, true);
    assert(MATRIXWORLD_isBlocked(matrix, 2, 2));    // The cell is now blocked
    assert(MATRIXWORLD_getNoOfBlockedCells(matrix) == 1);
    assert(MATRIXWORLD_getNoOfUnblockedCells(matrix) == 24);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Set and Get Cell\n");
}

void test_clear_matrix() {
    printf("Testing: Clear Matrix\n");
    worldMatrix* matrix = MATRIXWORLD_matrixInitialization(5, 5);
    MATRIXWORLD_setCell(matrix, 1, 1, true);
    MATRIXWORLD_setCell(matrix, 2, 2, true);
    assert(MATRIXWORLD_getNoOfBlockedCells(matrix) == 2);
    MATRIXWORLD_clearMatrix(matrix);
    assert(MATRIXWORLD_getNoOfBlockedCells(matrix) == 0);
    assert(MATRIXWORLD_getNoOfUnblockedCells(matrix) == 25);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Clear Matrix\n");
}

void test_blanking() {
    printf("Testing: Blanking\n");
    worldMatrix* matrix = MATRIXWORLD_matrixInitialization(4, 4);
    Cords coords[] = {{0,0}, {1,1}, {2,2}};
    MATRIXWORLD_matrixBlanking(matrix, coords, 3);
    assert(MATRIXWORLD_getNoOfBlockedCells(matrix) == 3);
    assert(MATRIXWORLD_isBlocked(matrix, 0, 0));
    assert(MATRIXWORLD_isBlocked(matrix, 1, 1));
    assert(MATRIXWORLD_isBlocked(matrix, 2, 2));
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Blanking\n");
}

void test_neighbors() {
    printf("Testing: Neighbors Count\n");
    worldMatrix* matrix = MATRIXWORLD_matrixInitialization(3, 3);
    // All unblocked, so 4 neighbors
    assert(MATRIXWORLD_countUnblockedNeighbors(matrix, 1, 1) == 4);
    // Block some neighbors
    MATRIXWORLD_setCell(matrix, 0, 1, true);
    MATRIXWORLD_setCell(matrix, 1, 0, true);
    assert(MATRIXWORLD_countUnblockedNeighbors(matrix, 1, 1) == 2);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Neighbors Count\n");
}


int main(void) {
    printf("--- Running MatrixWorld Tests ---\n");
    test_initialization_and_sizes();
    test_set_and_get_cell();
    test_clear_matrix();
    test_blanking();
    test_neighbors();
    printf("--- All MatrixWorld Tests Passed ---\n");
    return 0;
}
