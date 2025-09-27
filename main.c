#include <stdio.h>
#include "someLibFile.h"

int main (int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    printf("Hello World\n");
    printLibMessage("This is the way\n");
    return 0;
}