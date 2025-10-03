#include "startingPointVector.h"
#include "matrixWorld.h"
#include <stdio.h>
#include <assert.h>

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

void test_remove_point() {
    printf("Testing: Remove Point\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    StartingPointVector* vec = STPOINT_createVector(matrix);

    Cords p1 = {1, 1}, p2 = {2, 2}, p3 = {3, 3}, p4_non_existent = {4, 4};

    STPOINT_addPoint(vec, p1);
    STPOINT_addPoint(vec, p2);
    STPOINT_addPoint(vec, p3);
    assert(STPOINT_getSize(vec) == 3);

    // Remove from middle
    STPOINT_removePoint(vec, p2);
    assert(STPOINT_getSize(vec) == 2);
    assert(!STPOINT_containsPoint(vec, p2));
    assert(STPOINT_containsPoint(vec, p1));
    assert(STPOINT_containsPoint(vec, p3));

    // Remove non-existent
    STPOINT_removePoint(vec, p4_non_existent);
    assert(STPOINT_getSize(vec) == 2);

    // Remove first
    STPOINT_removePoint(vec, p1);
    assert(STPOINT_getSize(vec) == 1);
    assert(!STPOINT_containsPoint(vec, p1));
    assert(STPOINT_containsPoint(vec, p3));

    // Remove last
    STPOINT_removePoint(vec, p3);
    assert(STPOINT_getSize(vec) == 0);
    assert(!STPOINT_containsPoint(vec, p3));

    STPOINT_destroyVector(&vec);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Remove Point\n");
}

void test_clear_vector() {
    printf("Testing: Clear Vector\n");
    WorldMatrix* matrix = MATRIXWORLD_matrixInitialization(10, 10);
    StartingPointVector* vec = STPOINT_createVector(matrix);

    STPOINT_addPoint(vec, (Cords){1, 1});
    STPOINT_addPoint(vec, (Cords){2, 2});
    assert(STPOINT_getSize(vec) == 2);

    STPOINT_cleanVector(vec);
    assert(STPOINT_getSize(vec) == 0);
    assert(!STPOINT_containsPoint(vec, (Cords){1, 1}));

    // Test clearing an already empty vector
    STPOINT_cleanVector(vec);
    assert(STPOINT_getSize(vec) == 0);

    STPOINT_destroyVector(&vec);
    MATRIXWORLD_matrixFree(&matrix);
    printf("Passed: Clear Vector\n");
}

int main(void) {
    printf("--- Running StartingPointVector Tests ---\n");
    test_create_and_destroy();
    test_add_and_contains();
    test_add_duplicates();
    test_add_sorted();
    test_remove_point();
    test_clear_vector();
    printf("--- All StartingPointVector Tests Passed ---\n");
    return 0;
}
