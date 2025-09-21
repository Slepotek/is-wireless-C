#include "someLibFile.h"
#include <stdio.h>
#include <stdlib.h>

void printLibMessage(char* message)
{
    printf("%s", message);
    uint16_t random_value = rand();
    printf("Random Value: %u\n", random_value);
}