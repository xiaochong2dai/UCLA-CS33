#include <stdio.h>
void main()
{
    //    int d = 16;
    unsigned char* c;
    int n = 8;
    unsigned* u_c = (unsigned*) c;
    //rounding down the address
    unsigned u_x, result;
    //  Clear the least significant 4 bits of p
    u_x &= ~((unsigned)0xf);
    result = (unsigned*)u_x;
    u_c = &result;
    for (int j = 0; j < n ; j++) {
        printf("\n%p    ", u_c);
        for (int i = 0; i < 4; i++) {
            printf("%08x    ", u_c[i]);
        }
        u_c = ((unsigned int*)u_c+16);
    }
}