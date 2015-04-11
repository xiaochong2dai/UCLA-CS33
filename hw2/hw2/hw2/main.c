#include <stdio.h>
#include <math.h>
void to_binary( short n, int w, int *x )
{
    int i ;
    long m,y ;
    
    if ( n >= 0 )
        m = n ;
    else
        m = exp2(w)+n ; /* so that mod works properly */
    
    for( i=0; i<w; i++ )
    {
        x[i] = m%2 ;
        y = m ;
        m = m/2 ;
    }
}
void main()
{
    short a, b, MIN_INT, MAX_INT ;
    int W, w, x[20], y[20], z[20] ;
    
    w = sizeof(a)*8 ;
    W = w-1 ;
    MAX_INT =  exp2( W )-1 ;
    MIN_INT = -exp2( W )   ;
    
    //
    //  put your test code here
    //
    for (a=5;a>=-5;a--)
    {
        printf("\nThe value of a is: %d",a);
        printf("\nThe value of a/4 is: %d",a/4);
        printf("\nThe value of a>>2 is: %d",a>>2);
        printf("\nThe value of ((a<0?(a+3):a)>>2 is: %d",((a<0?(a+3):a)>>2));
    }
    to_binary(MIN_INT+MAX_INT,w,x);
    printf("\nThe binary value of MIN_INT+MAX_INT is:");
    for(int i=w-1;i>=0;i--)
    {
         printf("%d",x[i]);
    }
    for (a=5;a>=-5;a--)
    {
        to_binary(a, w,x);
        printf("\nThe binary value of %d is:",a);
        for(int i=w-1;i>=0;i--)
        {
            printf("%d",x[i]);
        }
    }
    for (a=5;a>=-5;a--)
    {
        to_binary(a^(MIN_INT+MAX_INT),w,z);
        printf("\nThe one's complement of %d is:",a);
        for(int i=w-1;i>=0;i--)
        {
            printf("%d",z[i]);
        }
    }
    
    a=3;
    to_binary(~a,w,x);
    printf("\nThe binary value of ~%d is:",a);
    for(int i=w-1;i>=0;i--)
    {
        printf("%d",x[i]);
    }
    
    for (b=5;b>=-5;b--)
    {
        to_binary(b,w,x);
        printf("\nThe binary value of %d is:",b);
        for(int i=w-1;i>=0;i--)
        {
            printf("%d",x[i]);
        }
    }
    for (b=5;b>=-5;b--)
    {
        to_binary(~b, w,x);
        printf("\nThe binary value of ~%d is:",b);
        for(int i=w-1;i>=0;i--)
        {
            printf("%d",x[i]);
        }
    }
    for (b=5;b>=-5;b--)
    {
        to_binary(~(~a|~b), w,x);
        printf("\nThe binary value of ~(~%d|~%d) is:",a, b);
        for(int i=w-1;i>=0;i--)
        {
            printf("%d",x[i]);
        }
    }
}