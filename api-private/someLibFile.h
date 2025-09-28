#ifndef SOME_LIB_FILE_H
#define SOME_LIB_FILE_H

#define UNUSED(x) ((void)(x))
#define FOUR_DIRECTIONS 4

#include <stdint.h>

/**
 * @brief A structure to hold 2D coordinates.
 */
typedef struct {
  uint16_t row; /**< The row coordinate. */
  uint16_t col; /**< The column coordinate. */
} Cords;

typedef struct {
  int8_t row;
  int8_t col;
} fourWayDirections;

static const fourWayDirections directions[FOUR_DIRECTIONS] = 
{
  {0, 1},
  {0, -1},
  {1, 0},
  {-1, 0}
};
void printLibMessage(char* message);

#endif