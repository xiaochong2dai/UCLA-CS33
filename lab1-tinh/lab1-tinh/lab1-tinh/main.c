//
//  main.c
//  cs33lab1
//
//  Created by Tinh Lac on 10/15/14.
//  Copyright (c) 2014 Tinh Lac. All rights reserved.
//

#include <stdio.h>

#include <math.h>


void to_binary  ( int  n, int w, int *x, int *o ) ;
void from_binary( int *x, int w, int *n ) ;
void adder( int *x, int *y, int *z, int *o, int w ) ;
void mult ( int *x, int *y, int *z, int *o, int w ) ;
void complement(int *x, int* compx, int w);
void shift_left (int *x, int w, int time, int *result);
void main()
{
    int a,b,c,d,w,n,o ;
    int x[32], y[32], z[32] ;
    
    o = 0 ;
    w = 9 ; /* word size in bits */
    /* note that x, y, and z must be at least as large as w and may be larger */
    b = 1 ; /* add one to each */
    for( c=250;c<260;c++ )
    {
        if ( c > 255 )  /* exceeds 8 bits */
            d = c-512 ;
        else
            d =  c ;
        printf( "%d %d ", d,b ) ;
        to_binary( d,w,x,&o );
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        to_binary( b,w,y,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",b ) ;
        adder( x,y,z,&o,w ) ;
        from_binary( z,w,&a ) ;
        printf( "%d ", a ) ;
        if ( o )
            printf( "Overflow in adder! " ) ;
        printf( "\n" ) ;
    }
    
    b = -1 ; /* add -1 to each */
    for( c=250;c<260;c++ )
    {
        if ( c > 255 )
            d = c-512 ;
        else
            d =  c ;
        printf( "%d %d ", d,b ) ;
        to_binary( d,w,x,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        to_binary( b,w,y,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        adder( x,y,z,&o,w ) ;
        from_binary( z,w,&a ) ;
        printf( "%d ", a ) ;
        if ( o )
            printf( "Overflow in adder! " ) ;
        printf( "\n" ) ;
    }
    
    
    for( w=8; w<13; w=w+4 )          /* word sizes 8 and 12 bits */
        for( a=98; a >-100; a=a-98 )  /* 99, 0, -99 */
            for( b=3; b>-5; b=b-3 )    /* 4, 0, -4 */
            {
                to_binary( a,w,x,&o ) ;
                //                for (int i = w-1; i >=0 ; i--) {
                //                    printf("%d", x[i]);
                //                }
                //                printf("\n");
                if ( o )
                    printf( "%d Overflow in to_binary! ",a ) ;
                to_binary( b,w,y,&o ) ;
                //                for (int i = w-1; i >=0 ; i--) {
                //                    printf("%d", y[i]);
                //                }
                //                printf("\n");
                if ( o )
                    printf( "%d Overflow in to_binary! ",b ) ;
                mult( x,y,z,&o,w ) ;
                printf( "%d %d ", a,b ) ;
                from_binary( z,w,&n ) ;
                printf( "%d ", n ) ;
                if ( o )
                    printf( "Overflow in multiplier! " ) ;
                printf( "\n" ) ;
            }
}
//void adder( int *x, int *y, int *z, int *o, int w )
//{
//    int cin = 0;
//    int c[32] ={0};
//    for (int i = 0; i < w; i++) {
//        c[i] = (x[i] ^ y[i] ^ cin);
//        if ((x[i]&y[i])||(x[i]&cin)||(y[i]&cin))
//            cin = 1;
//        else
//            cin = 0;
//    }
//    int j = w-1;
//    if ((y[j]&x[j])&!c[j]) {
//        *o = 1;
//    }
//    else if ((!y[j]&!x[j])&c[j]) {
//        *o = 1;
//    }
//    else
//        *o = 0;
//    for (int i = 0; i < w; i++) {
//        z[i] = c[i];
//    }
//}
void mult ( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first multiplicand */
    /* y is an input array of w ints, either 0 or 1 representing the second multicand */
    /* z is an output array of w ints, either 0 or 1 representing the product of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */
    int x_temp[32] = {0};
    int y_temp[32] = {0};
    int temp3[32]  ={0};
    for (int i = w-1; i >= 0; i --) {
        z[i] = 0;
    }
    //int result[32] = {0};
    int of = 0;
    int shifted1 [32] = {0};
    if (x[w-1]&y[w-1]) {
        complement(x, x_temp, w);
        complement(y, y_temp, w);
        for (int i = 0; i < w; i++) {
            if (y_temp[i]) {
                shift_left(x_temp, w, i, shifted1);
                // print the  shifted1
                for (int i = 0; i >=0; i--) {
                    printf("%d", shifted1[i]);
                }
                printf("\n");
                if (shifted1[w-1]) {
                    *o =1;
                }
            }
            adder(shifted1, z, temp3, &of, w);
            for (int i = 0; i < w; i ++) {
                shifted1[i] = 0;
            }
            for (int i = w-1; i >=0; i--) {
                z[i] = temp3[i];
            }
        }
    }
    if (x[w-1] & !y[w-1]) {
        complement(x, x_temp, w);
        for (int i = 0; i <w; i++) {
            if (y[i]) {
                shift_left(x_temp, w, i, shifted1);
                if (shifted1[w-1]) {
                    *o = 1;
                }
            }
            adder(shifted1, z, temp3, &of, w);
            for (int i = 0; i < w; i ++) {
                shifted1[i] = 0;
            }
            for (int i = w-1; i >=0; i--) {
                z[i] = temp3[i];
            }
        }
        complement(z, z, w);
    }
    if (!x[w-1] & y[w-1]) {
        complement(y, x_temp, w);
        for (int i = 0; i <w; i ++) {
            if (x_temp[i]) {
                shift_left(x, w, i, shifted1);
                if (shifted1[w-1]) {
                    *o = 1;
                }
            }
            adder(shifted1, z, temp3, &of, w);
            for (int i = 0; i < w; i ++) {
                shifted1[i] = 0;
            }
            for (int i = w-1; i >=0; i--) {
                z[i] = temp3[i];
            }
        }
        complement(z, z, w);
    }
    if (!x[w-1] & !y[w-1]) {
        for (int i = 0; i < w; i++) {
            if (y[i]) {
                shift_left(x, w, i, shifted1);
                if (shifted1[w-1]) {
                    *o = 1;
                }
            }
            else{
                for (int i = 0; i < w; i ++) {
                    shifted1[i] = 0;
                }
            }
            //            for (int k = w-1; k >=0; k--) {
            //                printf("%d", shifted1[k]);
            //            }
            //            printf("\n");
            adder(shifted1, z, temp3, &of, w);
            for (int i = w-1; i >=0; i--) {
                z[i] = temp3[i];
            }
        }
    }
    
}
void to_binary( int n, int w, int *x, int* o )
{
    /* n is an input and is the number to be converted to binary */
    /* w is an input = to the width in bits of x */
    /* x is an output array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* o is an output = 1 if an overflow occurred ( n is too large for w bits ) */
    int max, min;
    max = pow(2,w-1)-1;
    min = -pow(2,w-1);
    int decimal = n;
    if (decimal > max || decimal < min) {
        *o = 1;
    }
    if (decimal <= max || decimal >= min) {
        *o = 0;
    }
    int k;
    for (int i = w-1; i >= 0; i--)
    {
        k = n >> i;
        x[i] = k&1;
    }
}
void from_binary( int *x, int w, int* n )
{
    /* x is an input array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* w is an input = to the width in bits of x */
    /* n is an output and will be equal to the value stored in binary in x */
    int result = 0;
    int i = 0;
    for (i = 0; i < w-1 ; i++) {
        result =  result + x[i]*pow(2,i);
    }
    if (i == w-1 && x[i] == 1) {
        result = -pow(2,i)+ result;
    }
    *n = result;
}
void shift_left (int *x, int w, int time, int *result)
{
    for (int i = w-1; i >= 0; i--) {
        result[i] = x[i];
    }
    if (time == 0) {
        for (int i = 0; i < w; i++) {
            result[i] = x[i];
        }
    }
    else
    {
        for (int j = 0; j < time; j++) {
            for (int k = w-1; k >= 0; k--) {
                if (!k) {
                    result[k] = 0;
                }
                else
                    result[k] = result[k-1];
            }
        }
    }
}
void complement(int *x, int* compx, int w)
{
    int temp[32] = {0};
    int one[32] = {1,0};
    //    for (int i = w-1; i >=0 ; i--) {
    //        printf("%d",one[i]);
    //    }
    //    printf("\n");
    for (int i = 0; i < w; i++) {
        if (x[i] == 1) {
            temp[i] = 0;
        }
        else
            temp[i] = 1;
    }
    int o = 0;
    adder(temp, one, compx, &o, w);
}
void adder( int *x, int *y, int *z, int *o, int w )
{
    int cin = 0;
    int c[32] ={0};
    for (int i = 0; i < w; i++) {
        c[i] = (x[i] ^ y[i] ^ cin);
        if ((x[i]&y[i])||(x[i]&cin)||(y[i]&cin))
            cin = 1;
        else
            cin = 0;
    }
    int j = w-1;
    if ((y[j]&x[j])&!c[j]) {
        *o = 1;
    }
    else if ((!y[j]&!x[j])&c[j]) {
        *o = 1;
    }
    else
        *o = 0;
    for (int i = w-1; i >= 0; i--) {
        z[i] = c[i];
    }
}