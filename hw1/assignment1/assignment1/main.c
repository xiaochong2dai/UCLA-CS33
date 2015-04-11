#include <stdio.h>
typedef char *byte_pointer;
int main()
{
    int test_num = 1;
    byte_pointer c = (byte_pointer)&test_num;
    if (*c==1)
        printf("This is a little endian machine.\n");
    else
        printf("This is a big endian machine.\n");
    return 0;
}