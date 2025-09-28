#include "path.h"
#include "matrixWorld.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

void test_initialization_and_length() {
    printf("Testing: Path Initialization and Length\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    Path* path = PATH_initializePath(12, matrix);
    assert(path != NULL);
    assert(PATH_getLength(path) == 0);
    assert(PATH_isEmpty(path));
    PATH_freePath(&path);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Path Initialization and Length\n");
}

void test_add_and_get_length() {
    printf("Testing: Add Coordinates and Get Length\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    Path* path = PATH_initializePath(12, matrix);
    PATH_addCoordinates(path, 1, 1);
    PATH_addCoordinates(path, 1, 2);
    assert(PATH_getLength(path) == 2);
    assert(!PATH_isEmpty(path));
    PATH_freePath(&path);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Add Coordinates and Get Length\n");
}

void test_get_pop_and_clear() {
    printf("Testing: Get, Pop, and Clear Coordinates\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    Path* path = PATH_initializePath(12, matrix);
    PATH_addCoordinates(path, 1, 1);
    PATH_addCoordinates(path, 2, 2);
    PATH_addCoordinates(path, 3, 3);

    Cords last = PATH_getLastCoordinates(path);
    assert(last.row == 3 && last.col == 3);
    assert(PATH_getLength(path) == 3);

    Cords popped = PATH_popCoordinates(path);
    assert(popped.row == 3 && popped.col == 3);
    assert(PATH_getLength(path) == 2);

    last = PATH_getLastCoordinates(path);
    assert(last.row == 2 && last.col == 2);

    PATH_clearPath(path);
    assert(PATH_getLength(path) == 0);
    assert(PATH_isEmpty(path));

    // Test popping from an empty path
    Cords empty_pop = PATH_popCoordinates(path);
    assert(empty_pop.row == UINT16_MAX && empty_pop.col == UINT16_MAX);

    PATH_freePath(&path);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Get, Pop, and Clear Coordinates\n");
}

void test_is_contiguous() {
    printf("Testing: Path Contiguity\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    Path* path = PATH_initializePath(12, matrix);

    // Test empty and single-element paths
    assert(PATH_isContiguous(path));
    PATH_addCoordinates(path, 5, 5);
    assert(PATH_isContiguous(path));

    // Test a valid contiguous path
    PATH_addCoordinates(path, 5, 6);
    PATH_addCoordinates(path, 6, 6);
    PATH_addCoordinates(path, 6, 5);
    assert(PATH_isContiguous(path));

    // Test a non-contiguous path (diagonal)
    PATH_addCoordinates(path, 7, 6);
    assert(!PATH_isContiguous(path));

    // Test a non-contiguous path (jump)
    PATH_clearPath(path);
    PATH_addCoordinates(path, 1, 1);
    PATH_addCoordinates(path, 3, 3);
    assert(!PATH_isContiguous(path));

    PATH_freePath(&path);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Path Contiguity\n");
}

int main(void) {
    printf("--- Running Path Tests ---\n");
    test_initialization_and_length();
    test_add_and_get_length();
    test_get_pop_and_clear();
    test_is_contiguous();
    printf("--- All Path Tests Passed ---\n");
    return 0;
}
