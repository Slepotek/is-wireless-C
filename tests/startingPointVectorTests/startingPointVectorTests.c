#include "startingPointVector.h"
#include "matrixWorld.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_create_and_destroy() {
    printf("Testing: Create and Destroy\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    StartingPointVector* vec = STPOINT_createVector(matrix);
    assert(vec != NULL);
    STPOINT_destroyVector(&vec);
    assert(vec == NULL);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Create and Destroy\n");
}

void test_add_and_contains() {
    printf("Testing: Add and Contains\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    StartingPointVector* vec = STPOINT_createVector(matrix);

    Cords p1 = {1, 1};
    Cords p2 = {2, 2};
    Cords p3 = {0, 5};

    // Test contains on empty vector
    assert(!STPOINT_containsPoint(vec, p1));

    // Add points
    STPOINT_addPoint(vec, p1);
    STPOINT_addPoint(vec, p2);
    STPOINT_addPoint(vec, p3);

    // Test contains for existing points
    assert(STPOINT_containsPoint(vec, p1));
    assert(STPOINT_containsPoint(vec, p2));
    assert(STPOINT_containsPoint(vec, p3));

    // Test contains for non-existing point
    Cords p4 = {4, 4};
    assert(!STPOINT_containsPoint(vec, p4));

    STPOINT_destroyVector(&vec);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Add and Contains\n");
}

void test_add_duplicates() {
    printf("Testing: Add Duplicates\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(5, 5);
    StartingPointVector* vec = STPOINT_createVector(matrix);

    Cords p1 = {3, 3};
    STPOINT_addPoint(vec, p1);
    STPOINT_addPoint(vec, p1); // Add duplicate
    STPOINT_addPoint(vec, p1); // Add duplicate

    // We can't directly check the size, but contains should be true
    assert(STPOINT_containsPoint(vec, p1));
    // A more complex test could check the size if we had a getter, 
    // but for now we rely on the memory check to ensure no weird allocations happen.

    STPOINT_destroyVector(&vec);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Add Duplicates\n");
}

void test_add_sorted() {
    printf("Testing: Add in Sorted Order\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(20, 20);
    StartingPointVector* vec = STPOINT_createVector(matrix);

    Cords p1 = {1, 1};
    Cords p2 = {1, 5};
    Cords p3 = {5, 1};
    Cords p4 = {5, 5};

    // Add out of order
    STPOINT_addPoint(vec, p3);
    STPOINT_addPoint(vec, p1);
    STPOINT_addPoint(vec, p4);
    STPOINT_addPoint(vec, p2);

    // Check that all points are found (implies sorting is not breaking contains)
    assert(STPOINT_containsPoint(vec, p1));
    assert(STPOINT_containsPoint(vec, p2));
    assert(STPOINT_containsPoint(vec, p3));
    assert(STPOINT_containsPoint(vec, p4));

    STPOINT_destroyVector(&vec);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Add in Sorted Order\n");
}

int main(void) {
    printf("--- Running StartingPointVector Tests ---\n");
    test_create_and_destroy();
    test_add_and_contains();
    test_add_duplicates();
    test_add_sorted();
    printf("--- All StartingPointVector Tests Passed ---\n");
    return 0;
}
