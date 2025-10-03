#ifndef SOME_LIB_FILE_H
#define SOME_LIB_FILE_H

#include <stdlib.h>
#define UNUSED(x) ((void)(x))
#define FOUR_DIRECTIONS 4
#define RANDOM_GEN_SEED 42 //some fairly ordinary number

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

static inline uint16_t UTILITY_generateRandomNumber(uint16_t upperBound)
{
  if (upperBound == 0) {
    return UINT16_MAX; //This should fail further during execution
  }

  // Addressing modulo bias
  // Discard from the max rand range the uneven max deviser of upperBound
  uint16_t validRange = RAND_MAX - (RAND_MAX % upperBound);
  // lower bound for the random generation is 0 as in WorldMatrix first index
  uint32_t returnValue;

  // The loop should have very small failure rate. Usually first pick is enough
  do{
    returnValue = rand();
  }while(returnValue >= validRange);

  return returnValue % upperBound;
}

static inline void UTILITY_seedRandomNumberGen(void)
{
  srand(RANDOM_GEN_SEED);
}
#endif