#include <stdio.h>
#include <assert.h>


int main(void)
{
    printf("Testing the test\n");
    assert(1);
    printf("First assert is ok\n");
    // assert(0);
    printf("Second assert is not ok\n (should not print)");
    return 0;
}