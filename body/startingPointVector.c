/* > Description ****************************************************************/
/**
 * @file startingPointVector.c
 * @brief This is the file for handling startingPoint vector that will retain
 *        coordinates from the worldMatrix of starting points that have already
 *        been visited by the algorithm.
 */

/* > Includes ****************************************************************/
#include "startingPointVector.h"
#include "utilities.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* > Defines *****************************************************************/

/* > Type Declarations *******************************************************/

/**
 * @brief Internal structure for the StartingPointVector.
 * @details This struct uses a flexible array member `points` to store all
 *          coordinate data within a single contiguous block of memory.
 */
struct StartingPointVector {
    uint32_t currentSize;    ///< The current number of points in the vector.
    const uint32_t capacity; ///< The maximum number of points the vector can hold.
    Cords points[];          ///< Flexible array member for storing coordinates.
};

/* > Global Constant Definitions *********************************************/

/* > Global Variable Definitions *********************************************/

/* > Local Constant Definitions **********************************************/

/* > Local Variable Definitions **********************************************/

/* > Local Function Declarations *********************************************/

/**
 * @brief Comparison function for two Cords, used for sorting and searching.
 * @details Sorts by row first, then by column.
 * @param aPoint[in] Pointer to the first coordinate.
 * @param bPoint[in] Pointer to the second coordinate.
 * @return An integer -1, 0, or 1.
 */
static int STPOINT_internal_compareCords(const void *aPoint, const void *bPoint);

/* > Global Function Definitions *********************************************/

StartingPointVector *STPOINT_createVector(const WorldMatrix *const matrix_p) {
    if (matrix_p == NULL) {
        fprintf(stderr, "FATAL ERROR: WorldMatrix supplied to STPOINT_createVector is NULL!\n");
        exit(EXIT_FAILURE);
    }

    const size_t capacity = MATRIXWORLD_getSize(matrix_p);
    if (capacity == 0) {
        fprintf(stderr, "ERROR: Cannot create StartingPointVector with zero capacity.\n");
        return NULL;
    }

    StartingPointVector *vector_p = malloc(sizeof(StartingPointVector) + sizeof(Cords) * capacity);
    if (vector_p == NULL) {
        fprintf(stderr, "FATAL ERROR: Could not allocate memory for StartingPointVector!\n");
        exit(EXIT_FAILURE);
    }

    *(uint32_t *)&vector_p->capacity = capacity;
    vector_p->currentSize = 0;

    return vector_p;
}

void STPOINT_destroyVector(StartingPointVector **const vector_pp) {
    if (vector_pp != NULL && *vector_pp != NULL) {
        free(*vector_pp);
        *vector_pp = NULL;
    }
}

void STPOINT_cleanVector(StartingPointVector *const vector_p) {
    if (vector_p != NULL) {
        vector_p->currentSize = 0;
    }
}

void STPOINT_addPoint(StartingPointVector *const vector_p, Cords point) {
    if (vector_p == NULL || vector_p->currentSize >= vector_p->capacity) {
        return;
    }

    // If the vector is empty, just insert at the beginning.
    if (vector_p->currentSize == 0) {
        vector_p->points[0] = point;
        vector_p->currentSize++;
        return;
    }

    int32_t lowerIdx = 0;
    int32_t higherIdx = vector_p->currentSize - 1;
    uint32_t insertionIdx = 0;

    while (lowerIdx <= higherIdx) {
        uint32_t middleIdx = lowerIdx + (higherIdx - lowerIdx) / 2;
        int cmp = STPOINT_internal_compareCords(&vector_p->points[middleIdx], &point);

        if (cmp == 0) {
            return;
        }

        if (cmp > 0) {
            higherIdx = middleIdx - 1;
        } else {
            lowerIdx = middleIdx + 1;
        }
    }

    insertionIdx = lowerIdx;

    if (insertionIdx < vector_p->currentSize) {
        memmove(&vector_p->points[insertionIdx + 1], &vector_p->points[insertionIdx],
                (vector_p->currentSize - insertionIdx) * sizeof(Cords));
    }

    vector_p->points[insertionIdx] = point;
    vector_p->currentSize++;
}

void STPOINT_removePoint(StartingPointVector *const vector_p, Cords point) {
    if (vector_p == NULL || vector_p->currentSize >= vector_p->capacity) {
        return;
    }

    // If the vector is empty, just insert at the beginning.
    if (vector_p->currentSize == 0) {
        fprintf(stderr, "FATAL ERROR: Trying to remove element from empty vector!");
        exit(EXIT_FAILURE);
    }

    Cords *pointToDelete_p =
        bsearch(&point, vector_p->points, vector_p->currentSize, sizeof(Cords),
                STPOINT_internal_compareCords);
    if (pointToDelete_p != NULL) {
      size_t pointIndex = pointToDelete_p - vector_p->points;
      size_t noOfElementsToMove = (vector_p->currentSize - 1) - pointIndex;
      if (noOfElementsToMove > 0)
        memmove(pointToDelete_p, pointToDelete_p + 1,
                sizeof(Cords) * noOfElementsToMove);
      vector_p->currentSize--;
    }
}

size_t STPOINT_getSize(const StartingPointVector *const vector_p) {
    if (vector_p == NULL) {
        return 0;
    }
    return vector_p->currentSize;
}

bool STPOINT_containsPoint(const StartingPointVector *const vector_p, Cords point) {
    if (vector_p == NULL || vector_p->currentSize == 0) {
        return false;
    }

    return bsearch(&point, vector_p->points, vector_p->currentSize, sizeof(Cords),
                   STPOINT_internal_compareCords) != NULL;
}

/* > Local Function Definitions **********************************************/

static int STPOINT_internal_compareCords(const void *aPoint, const void *bPoint) {
    const Cords *cordA = (const Cords *)aPoint;
    const Cords *cordB = (const Cords *)bPoint;

    if (cordA->row < cordB->row)
        return -1;
    if (cordA->row > cordB->row)
        return 1;

    if (cordA->col < cordB->col)
        return -1;
    if (cordA->col > cordB->col)
        return 1;

    return 0;
}