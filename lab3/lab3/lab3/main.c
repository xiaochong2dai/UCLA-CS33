#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define S       16     //  (16 cache sets)
#define E       4      //  (4-way associative cache)
#define B       32     //  (32 elements in each block)
#define T       7      //  (7 tag bits)
#define M       65536  //  (65536 location memory)
#define READ    1
#define WRITE   0

int s ;
int b ;
int m ;

int rhits = 0 ; int rmiss = 0 ; int whits = 0 ; int wmiss = 0 ; int dwrit = 0 ;

struct cache_t
{
    char dirty ;
    char valid ;
    int  tag   ;
    int 	last  ;
    int  *block ;
} cache[S][E] ;

int *memory ;

int callno = 0 ;

int x  =     0 ;
int y  = 16384 ;
int z  = 32768 ;
int ni =    20 ;
int nj =    20 ;

void stats( char *t )
{
    int i,j,k,A ;
    
    for( i=0; i<S; i++ )
        for( j=0; j<E; j++ )
        {
            if( cache[i][j].valid & cache[i][j].dirty )
            {
                A = cache[i][j].tag*exp2(m-T)+i*B ;
                for( k=0; k<B; k++ )
                    memory[A+k] = cache[i][j].block[k] ;
                dwrit = dwrit+1 ;
            }
            cache[i][j].valid = 0 ;
            cache[i][j].dirty = 0 ;
        }
    
    printf( "%8s y=%5d ni=%5d nj=%5d rh=%5d rm=%5d wh=%5d wm= %5d dw=%5d\n", t, y,ni,nj,
           rhits, rmiss, whits, wmiss, dwrit ) ;
    rhits = 0 ; rmiss = 0 ;
    whits = 0 ; wmiss = 0 ;
    dwrit = 0 ;
    
}

void initcache()
{
    //
    // put code to calculate s, b, m here
    s=log2(S);
    b=log2(B);
    m=s+b+T;
    //
    printf( "S= %d E= %d B= %d T= %d M= %d s= %d b= %d m= %d\n", S,E,B,T,M,s,b,m ) ;
    
    //
    // put code to initialize cache and memory here
    memory=malloc(M*sizeof(int));
    int l;
    for (l=0;l<M;l++)
        memory[l]=l;
    
    int i,j,k ;
    
    for( i=0; i<S; i++ )
        for( j=0; j<E; j++ )
        {
            cache[i][j].valid = 0 ;
            cache[i][j].dirty = 0 ;
            cache[i][j].tag = 0 ;
            cache[i][j].last = 0 ;
            cache[i][j].block=malloc(B*sizeof(int));
            for( k=0; k<B; k++ )
                cache[i][j].block[k] =0;
        }
    
    //
}

void readwritecache( int readwrite, int a, int *value, int *hitmiss, int voice )
{
    
    //
    // readwrite = READ (1) for read, WRITE (0) for write
    // a = binary memory address ( 0 <= a < 65535 ) to read or write
    // *value is memory value returned when readwrite = READ
    //           memory value to store when readwrite = WRITE
    // hitmiss is set to 0 for a miss, 1 for a hit, in either read or write case
    // voice is a debugging switch
    //
    //
    //   compute si, ta, and bo from the address a
    int si, ta, bo, li ;
    unsigned int temp= (unsigned int) a;
    ta=(temp&0xfe00)>>(s+b);
    si=((temp&0x01e0)<<T)>>(T+b);
    bo=temp&0x1f;

    *hitmiss=0;
    int j;
    for(j=0; j<E; j++ )   //hits
    {
        if( cache[si][j].valid && cache[si][j].tag==ta)
        {
            li=j;
            *hitmiss=1;
            callno++;
            cache[si][li].last=callno;
            if (readwrite==1)
            {
                *value=cache[si][li].block[bo];
                rhits++;
            }
            
            else
            {
                cache[si][li].block[bo]=*value;
                cache[si][li].dirty=1;
                whits++;
            }
            break;
        }
    }

    if (*hitmiss==0)   //misss
    {
        int empty=0;
        int i;
        for (i=0;i<E;i++) //miss blank
        {
            li=i;
            if (cache[si][li].valid==0)
            {
                empty=1;
                memcpy(cache[si][li].block,memory+(ta<<(s+b))+(si<<b), B*sizeof(int));
                cache[si][li].valid=1;
                cache[si][li].tag=ta;
                callno++;
                cache[si][li].last=callno;
                if (readwrite==1)
                {
                    *value=cache[si][li].block[bo];
                    rmiss++;
                }
                
                else if( readwrite==0)
                {
                    cache[si][li].block[bo]=*value;
                    cache[si][li].dirty=1;
                    wmiss++;
                }
                break;
            }
        }
        
        if(empty==0) // miss last
        {
            li=0;
            int lru=cache[si][li].last;
            int ckli;
            for (ckli=1;ckli<E;ckli++)
            {
                if(cache[si][ckli].last<lru)
                {
                    lru=cache[si][ckli].last;
                    li=ckli;
                }
            }
            if (cache[si][li].dirty==1)
            {
                memcpy(memory+(cache[si][li].tag<<(s+b))+(si<<b), cache[si][li].block,B*sizeof(int));
                dwrit++;
            }
            memcpy(cache[si][li].block,memory+(ta<<(s+b))+(si<<b), B*sizeof(int));
         
            /*
            int A = a-bo ;
            for( int k=0; k<B; k++ )
                cache[si][li].block[k] = memory[A+k] ;*/
            
            cache[si][li].valid=1;
            cache[si][li].tag=ta;
            cache[si][li].dirty=0;
            callno++;
            cache[si][li].last=callno;
            if (readwrite==1)
            {
                *value=cache[si][li].block[bo];
                rmiss++;
            }
            
            else if( readwrite==0)
            {
                cache[si][li].block[bo]=*value;
                cache[si][li].dirty=1;
                wmiss++;
            }
        }
    }
    //	si = stack, index
    //	ta = tag
    //	bo = block offset
    //      increment callno
    //
    //voice=1;
    if( voice )
        printf( "%5d rw= %d a= %5d bo= %3d si= %3d ta= %3d", callno,readwrite,a,bo,si,ta ) ;
    
    //
    //   check each line of the set:
    //	if( cache[si][line#].valid && cache[si][line#].tag = ta )
    //	to find a hit
    //
    //   if no hit, choose a line (not valid or LRU )
    //
    //   if chosen line dirty, copy to memory
    //
    if( voice )
        printf( "li= %d", li ) ;
    //
    //    copy from memory to chosen line
    //
    //    if write, copy value into line, set dirty
    //
    //    else copy value from line, not dirty
    //
    //    set last for line
    //
    if( voice )
        printf( " %d %d %d\n", *value, cache[si][li].valid, cache[si][li].dirty ) ;
}

void locationexample()
{
    int i,j,k,hm, r;
    
    for( y=16374; y< 16395; y=y+1 )
    {
        for( i=0; i<nj;i++ )
        {
            readwritecache( READ,   x+i, &r, &hm, 0 ) ;
            readwritecache( WRITE,  y+i, &r, &hm, 0 ) ;
        }
        stats( "loc copy" ) ;
        
    }
    
    
    //
    // code for row wise transponse
    //
    for( y=16374; y< 16395; y=y+1 )
    {
        for(i=0;i<ni;i++)
        {
            for (j=0;j<nj;j++)
            {
               /* readwritecache( READ,   y+nj*j+i, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+nj*i+j, &r, &hm, 0 ) ;*/
                readwritecache( READ,   y+nj*i+j, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+ni*j+i, &r, &hm, 0 ) ;
            }
        }
        stats( "loc rows" ) ;
    }
    
    //
    // code for col wise transponse
    //
    for( y=16374; y< 16395; y=y+1 )
    {
        for(j=0;j<nj;j++)
        {
            for (i=0;i<ni;i++)
            {
              /*  readwritecache( READ,   y+nj*j+i, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+nj*i+j, &r, &hm, 0 ) ; */
                readwritecache( READ,   y+ni*i+j, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+ni*j+i, &r, &hm, 0 ) ;
            }
        }
        stats( "loc cols" ) ;
    }
}

void wsexample()
{
    int i,j,ii,jj,hm,r ;
    
    y = 20000 ;
    for( ni=88; ni<128; ni=ni+8 )
    {
        nj = ni;
        //
        // code for row wise transpose
        //
        for(i=0;i<ni;i++)
            for (j=0;j<nj;j++)
            {
              /*  readwritecache( READ,   y+nj*j+i, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+nj*i+j, &r, &hm, 0 ) ; */
                readwritecache( READ,   y+nj*i+j, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+ni*j+i, &r, &hm, 0 ) ;
            }
        stats( "ws rows" ) ;
    }
    
  
    for( ni=88; ni<128; ni=ni+8 )
    {
        nj = ni ;
        //
        // code for row wise transpose with 8x blocking
        //
        
        for( ii=0; ii<ni; ii=ii+8 )
            for( jj=0; jj<nj; jj=jj+8 )
                for( i=ii; i<ii+8; i++ )
                    for( j=jj; j<jj+8; j++ )
                    {
                      /*  readwritecache( READ,   y+nj*j+i, &r, &hm, 0 ) ;
                        readwritecache( WRITE,  x+nj*i+j, &r, &hm, 0 ) ; */
                        readwritecache( READ,   y+nj*i+j, &r, &hm, 0 ) ;
                        readwritecache( WRITE,  x+ni*j+i, &r, &hm, 0 ) ;
                    }
        
        stats( "wsbrows" ) ;
    }
    
    for( ni=88; ni<128; ni=ni+8 )
    {
        nj = ni ;
        //
        // code for col wise transpose 
        //
        for(j=0;j<nj;j++)
            for (i=0;i<ni;i++)
            {
             /*   readwritecache( READ,   y+nj*j+i, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+nj*i+j, &r, &hm, 0 ) ; */
                readwritecache( READ,   y+ni*i+j, &r, &hm, 0 ) ;
                readwritecache( WRITE,  x+ni*j+i, &r, &hm, 0 ) ;
            }
        stats( "ws cols" ) ;
    }
    
    for( ni=88; ni<128; ni=ni+8 )
    {
        nj = ni ;
        //
        // code for col wise transpose with 8x blocking
        //
        for( jj=0; jj<ni; jj=jj+8 )
            for( ii=0; ii<nj; ii=ii+8 )
                for( j=jj; j<jj+8; j++ )
                    for( i=ii; i<ii+8; i++ )
                    {
                      /*  readwritecache( READ,   y+nj*j+i, &r, &hm, 0 ) ;
                        readwritecache( WRITE,  x+nj*i+j, &r, &hm, 0 ) ; */
                        readwritecache( READ,   y+ni*i+j, &r, &hm, 0 ) ;
                        readwritecache( WRITE,  x+ni*j+i, &r, &hm, 0 ) ;
                    }
                    
        stats( "wsbcols" ) ;
    }

}

int main()
{
    
    initcache() ;
    locationexample() ;
    wsexample() ;
    
    return 0 ;
}