
#include <stdio.h>

#include <math.h>


void to_binary  ( int  n, int w, int *x, int *o ) ;
void from_binary( int *x, int w, int *n ) ;
void adder( int *x, int *y, int *z, int *o, int w ) ;
void mult ( int *x, int *y, int *z, int *o, int w ) ;
void to_complement(int *x, int w);
void shift_left(int * arr, int len, int jiwei, int *temp);

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
        to_binary( d,w,x,&o ) ;
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
            for( b=6; b>-5; b=b-6 )    /* 4, 0, -4 */
            {
                to_binary( a,w,x,&o ) ;
                if ( o )
                    printf( "%d Overflow in to_binary! ",a ) ;
                to_binary( b,w,y,&o ) ;
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

void adder( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first addend */
    /* y is an input array of w ints, either 0 or 1 representing the second addend */
    /* z is an output array of w ints, either 0 or 1 representing the sum of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */
    
    int carry=0;
    int check_carry1,check_carry2;
    for(int i=0; i<w; i++)
    {
        if((x[i]&y[i])||(x[i]&carry)||(y[i]&carry))
        {
            if(x[i]&y[i]&carry)
                z[i]=1;
            else
                z[i]=0;
            carry=1;
        }
        else if((x[i]|y[i])||(x[i]|carry)||(y[i]|carry))
        {
            z[i]=1;
            carry=0;
        }
        else
        {
            z[i]=0;
            carry=0;
        }
        
        if(i==w-2)
            check_carry1=carry;
        if(i==w-1)
            check_carry2=carry;
    }
    if(check_carry1^check_carry2)
        *o=1;
}
void mult ( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first multiplicand */
    /* y is an input array of w ints, either 0 or 1 representing the second multicand */
    /* z is an output array of w ints, either 0 or 1 representing the product of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */

    int init[w];
    int temp[w];
    for (int i=0;i<=w-1;i++)
    {
        init[i]=0;
        temp[i]=0;
        z[i]=0;
    }
    int sign_counter;
    if (x[w-1]&y[w-1])
    {
        sign_counter=2;
        to_complement(x, w);
        to_complement(y, w);
    }
    else if(!(x[w-1]|y[w-1]))
        sign_counter=0;
    else
    {
        sign_counter=1;
        if (x[w-1])
            to_complement(x, w);
        else
            to_complement(y, w);
    }
    int flag=0;

    for (int j=0;j<w;j++)
    {
        if (y[j])
        {
            shift_left(x, w, j,temp);
        }
        if(temp[w-1])
            *o=1;
        adder( init,temp,z,&flag,w );
     /*
        
        printf("\n");
        for (int i=0; i<w;i++)
        {
        printf("%d",temp[i]);
        }
        printf("\n");*/
        
        
        /*for (int i=0;i<=w-1;i++)
        {
            init[i]=z[i];
        }*/
    }
    if(sign_counter==1)
       to_complement(z, w);
}

void to_complement(int *x, int w)
{
    for (int i=0;i<w;i++)
    {
        x[i]=abs(x[i]-1);
    }
    int temp[w];
    for (int i=0;i<w;i++)
        temp[i]=x[i];
    int flag=0;
    int com[w];
    com[0]=1;
    for (int i=1;i<w;i++)
        com[i]=0;
    adder(temp, com, x, &flag, w);
}

void shift_left(int * arr, int len, int jiwei, int *temp)
{
    
    for (int i = len - jiwei-1;  i >= 0;  i--)
    {
        temp[i+jiwei]= arr[i];
    }
    for (int j=0;j<jiwei;j++)
        temp[j] = 0;
}
    
void to_binary( int n, int w, int *x, int *o )
{
    /* n is an input and is the number to be converted to binary */
    /* w is an input = to the width in bits of x */
    /* x is an output array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* o is an output = 1 if an overflow occurred ( n is too large for w bits ) */
    *o=0;
    int MAX=pow(2,w-1)-1;
    int MIN=-pow(2,w-1);
    int k;
    
    if(n>MAX || n<MIN)
        *o=1;
    else
    {
        for (int a = w-1; a >= 0; a--)
        {
            k = n >> a;
            if (k & 1)
            {
                x[a]=1;
            }
            else
            {
                x[a]=0;
            }
        }
    }
}
    
void from_binary( int *x, int w, int *n )
{
    /* x is an input array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* w is an input = to the width in bits of x */
    /* n is an output and will be equal to the value stored in binary in x */
    *n=0;
    for (int i=0; i<w-1;i++)
    {
        *n += x[i]*pow(2,i);
    }
    if(x[w-1])
        *n-=pow(2,w-1);
}