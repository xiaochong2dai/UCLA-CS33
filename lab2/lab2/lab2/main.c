#include <stdio.h>
#include <stdlib.h>

char *min_ptr = NULL , *max_ptr ;
char *z;
int m;

void spray_paint( char *x, int sz, char tok, char *t )
{
    printf( "%p %3d %x %s\n", x,sz,(unsigned char) tok,t ) ;
    
    //
    // spray paint code
    //
    
    if (min_ptr==NULL)
    {
        min_ptr=x;
        max_ptr=(x+sz-1);
    }
    else if (min_ptr>x)
        min_ptr=x;
    else if(max_ptr<(x+sz-1))
        max_ptr=(x+sz-1);
    for (int i=0;i<sz;i++)
    {
        *(x+i)=tok;
    }
}

void dumper( char *x, int n )
{
    unsigned char* roundDown = (unsigned char*)(x - ((long)x % 16));
    
    for (int j = 0; j < n ; j++) {
        printf("\n%p    ", roundDown);
        for (int k=0;k<4;k++)
        {
            for (int l=3;l>=0;l--)
                printf("%02x", *(roundDown+4*k+l));
            printf("   ");
            
        }
         roundDown = &(*(roundDown+16));
        }
}

void sub2()
{
    int i ;
    char* a ;
    char x[20] ;
    
    spray_paint( (char *) &i, sizeof(i), 0xf1, "sub2.i" ) ;
    spray_paint( (char *) &a, sizeof(a), 0xf2, "sub2.a" ) ;
    spray_paint( (char *) &x, sizeof(x), 0xf3, "sub2.x" ) ;
    printf ( "Min= %p Max= %p\n", min_ptr, max_ptr ) ;
    dumper( min_ptr,(int) (max_ptr-min_ptr)/16+1 ) ;
    //
    //   save/destroy the stack here (don't forget to use an external save area)
    //
    z=(char *) malloc(max_ptr-min_ptr+1);
    for (m=0;m<max_ptr-min_ptr+1;m++)
        *(z+m)=*(min_ptr+m);

    for (m=0;m<max_ptr-min_ptr+1;m++)
        *(min_ptr+m)=00;
    
    printf( " destroyed stack\n" ) ;
    dumper( min_ptr,(int) (max_ptr-min_ptr)/16+1 ) ;
    //
    //    restore the stack here
    //
    for (m=0;m<max_ptr-min_ptr+1;m++)
        *(min_ptr+m)=*(z+m);

    
    printf( " restored stack\n" ) ;
    dumper( min_ptr,(int) (max_ptr-min_ptr)/16+1 ) ;
    
    free(z);
}

void sub1()
{
    float i ;
    short a ;
    int x[20] ;
    
    spray_paint( (char *) &i, sizeof(i), 0xe1, "sub1.i" ) ;
    spray_paint( (char *) &a, sizeof(a), 0xe2, "sub1.a" ) ;
    spray_paint( (char *) &x, sizeof(x), 0xe3, "sub1.x" ) ;
    sub2() ;
}
int main()
{
    struct mine {
        char* a ;
        char x[20] ;
        float z ;
    } i;
    
    union crazy {
        float a ;
        char b ;
        int s ;
    } a ;
    
    char x[50] ;
    
    spray_paint( (char *) &i, sizeof(i), 0xd1, "main.i" ) ;
    spray_paint( (char *) &a, sizeof(a), 0xd2, "main.a" ) ;
    spray_paint( (char *) &x, sizeof(x), 0xd3, "main.x" ) ;
    
    sub1() ;
    return 0 ;
}