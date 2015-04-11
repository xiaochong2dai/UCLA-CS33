#include <stdio.h>
#include <stdlib.h>

#define HEAPSIZE 32768

#define hdr_payload(i)  ((struct HDR *) &heap[i])->payload
#define hdr_freeall(i)  ((struct HDR *) &heap[i])->freeall
#define hdr_succesr(i)  ((struct HDR *) &heap[i])->succesr
#define hdr_previus(i)  ((struct HDR *) &heap[i])->previus

char *heap ;
int  heapsize ;

int sizehdr ;

struct HDR       // free block header/footer/linked list
{
    int  payload ;    // size of block (excluding headers)
    char freeall ;    // is this block allocated? 0=free/1=allocated
    int  succesr ;    // successor free block
    int  previus ;    // previous free block
} anchor ;

void dump_heap( char *t )   // prints all of the headers/footers on the heap
{
    int i ;
    
    int j=0 ;
    printf( "%s anchor= %d\n", t, anchor.succesr ) ;
    for( i=0; i<heapsize; i=i+sizehdr  )
        if ( hdr_payload(i) != 0 )
        {
            printf( "%6d %6d %6d %6d %6d\n", i, hdr_payload(i), hdr_freeall(i), hdr_previus(i), hdr_succesr(i) ) ;
            if ( j++%2==1 ) printf( "\n" ) ;
        }
}

void init_heap()   // initialize the heap
{
    heap = (char *) calloc( HEAPSIZE, 1 ) ;
    sizehdr =  16 ;
    heapsize = HEAPSIZE ;
    
    anchor.payload =  0 ;   // anchor
    anchor.freeall =  1 ;
    anchor.succesr =  0 ;
    anchor.previus = -1 ;
    
    hdr_payload(0) = heapsize-2*sizehdr ;   // header
    hdr_freeall(0) = 0 ;
    hdr_succesr(0) = heapsize ;
    hdr_previus(0) = -1 ;
    hdr_payload(heapsize-sizehdr) = heapsize-2*sizehdr ; // footer
    hdr_freeall(heapsize-sizehdr) = 0 ;
}

/* left free
 right free
 both left & right free
 neither left or right free
 first free spot
 last free spot
 */


void lfree( int *aa )   // frees the block at address aa
{
    // lfree code here
    int index_aa=*aa;
    if (hdr_freeall(index_aa-sizehdr)!=0)
    {
        int pll;
        int lfinfo;
        if ((index_aa-sizehdr)!=0)
        {
            pll=hdr_payload(index_aa-sizehdr*2);
            lfinfo=hdr_freeall(index_aa-sizehdr*2);
        }
        int plm=hdr_payload(index_aa-sizehdr);
        int plr=hdr_payload(index_aa+plm+sizehdr);
        
        //left block free right block not free or last block
        if ((lfinfo==0)&& ((hdr_freeall(index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)!=0)|| ((index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)==heapsize)))
        {
            /*
             pll=hdr_payload(index_aa-sizehdr*2);
             plm=hdr_payload(index_aa-sizehdr);*/
            
            hdr_payload(index_aa-sizehdr*3-pll)=pll+plm+sizehdr*2;    //new header payload
            hdr_payload(index_aa+plm)=pll+plm+sizehdr*2;    //new footer payload
            
            hdr_freeall(index_aa+plm)=0;  //new footer freeall
            //hdr_freeall(index_aa-sizehdr*3-pll)=0;
            
            hdr_freeall(index_aa-sizehdr)=hdr_freeall(index_aa-sizehdr*2)=0;
            hdr_payload(index_aa-sizehdr)=hdr_payload(index_aa-sizehdr*2)=0;
        }
        
        //left block not free or the first block, and right block free
        else if (((hdr_freeall(index_aa-sizehdr*2)!=0)|| (index_aa-sizehdr==0)) && (hdr_freeall(index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)==0))
        {
            /*
             plm=hdr_payload(index_aa-sizehdr);
             plr=hdr_payload(index_aa+plm+sizehdr);*/
            
            //temp_prev=hdr_previus(index_aa+plm+sizehdr);
            //temp_succ=hdr_succesr(index_aa+plm+sizehdr);
            hdr_payload(index_aa-sizehdr)=plm+sizehdr*2+plr;  //new header payload
            hdr_payload(index_aa+plm+sizehdr*2+plr)=plm+sizehdr*2+plr; //new footer payload
            hdr_freeall(index_aa-sizehdr)=0;    //new header freeall
            hdr_freeall(index_aa+plm+sizehdr*2+plr)=0;
            
            
            hdr_previus(index_aa-sizehdr)=hdr_previus(index_aa+plm+sizehdr);
            if(hdr_previus(index_aa-sizehdr)==-1)
            {
                anchor.succesr=index_aa-sizehdr;
            }
            else
                hdr_succesr(hdr_previus(index_aa+plm+sizehdr))=index_aa-sizehdr;
            
            hdr_succesr(index_aa-sizehdr)=hdr_succesr(index_aa+plm+sizehdr);
            hdr_previus(hdr_succesr(index_aa+plm+sizehdr))=index_aa-sizehdr;
            
            hdr_succesr(index_aa+plm+sizehdr)=hdr_previus(index_aa+plm+sizehdr)=0;
            
            hdr_freeall(index_aa+plm)=hdr_freeall(index_aa+plm+sizehdr)=0;
            hdr_payload(index_aa+plm)=hdr_payload(index_aa+plm+sizehdr)=0;
        }
        
        //both left block and right block free
        else if ((hdr_freeall(index_aa-sizehdr*2)==0) && (hdr_freeall(index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)==0))
        {
            
            hdr_payload(index_aa-pll-sizehdr*3)=pll+plm+plr+sizehdr*4;  //new header payload
            hdr_payload(index_aa+plm+plr+sizehdr*2)=pll+plm+plr+sizehdr*4;  //new footer payload
            
            hdr_freeall(index_aa-sizehdr)=hdr_freeall(index_aa-sizehdr*2)=0;
            hdr_payload(index_aa-sizehdr)=hdr_payload(index_aa-sizehdr*2)=0;
            hdr_freeall(index_aa+plm)=hdr_freeall(index_aa+plm+sizehdr)=0;
            hdr_payload(index_aa+plm)=hdr_payload(index_aa+plm+sizehdr)=0;
            
            hdr_succesr(index_aa-pll-sizehdr*3)=hdr_succesr(index_aa+plm+sizehdr); //consider if succesr is heapsize??????
            hdr_previus(hdr_succesr(index_aa+plm+sizehdr))=index_aa-pll-sizehdr*3;
            
            hdr_previus(index_aa+plm+sizehdr)=hdr_succesr(index_aa+plm+sizehdr)=0;
            
        }
        
        //left block not free or the first block, right block not free as well or last block
        /*     else if (((hdr_freeall(index_aa-sizehdr*2)!=0) && (hdr_freeall(index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)!=0))||((index_aa-sizehdr==0) && (hdr_freeall(index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)!=0))|| ((hdr_freeall(index_aa-sizehdr*2)!=0) &&((index_aa+hdr_payload(index_aa-sizehdr)+sizehdr)==heapsize)))*/
        else
        {
            hdr_freeall(index_aa-sizehdr)=0;
            hdr_freeall(index_aa+hdr_payload(index_aa-sizehdr))=0;
            int i,flag=0;
            
            for (i=index_aa-sizehdr*2;i>=2*sizehdr;i=i-sizehdr)
            {
                if (hdr_freeall(i)==0)
                {
                    hdr_succesr(index_aa-sizehdr)=hdr_succesr(i-hdr_payload(i)-sizehdr);
                    hdr_previus(hdr_succesr(i-hdr_payload(i)-sizehdr))=index_aa-sizehdr;
                    hdr_succesr(i-hdr_payload(i)-sizehdr)=index_aa-sizehdr;
                    hdr_previus(index_aa-sizehdr)=i-hdr_payload(i)-sizehdr;
                    flag=1;
                    break;
                }
                i=i-hdr_payload(i)-sizehdr;
            }
            
            if (flag==0)
            {
                hdr_succesr(index_aa-sizehdr)=anchor.succesr;
                hdr_previus(anchor.succesr)=index_aa-sizehdr;
                hdr_previus(index_aa-sizehdr)=-1;
                anchor.succesr=index_aa-sizehdr;
            }
        }
    }
    else
        return;
}


int lalloc( int amt )   // allocates a block of size int
{
    // lalloc code here
    int round_amt=((amt+15)/16)*16;
    int i=0,loc,j=1;
    int flag=0;
    int first=anchor.succesr;
    
    while (flag==0) //no free block larger than the requested block (plus header/footer), adding HEAPSIZE additional space
    {
        for (i=first;i<heapsize;i=hdr_succesr(i))
        {
            if (hdr_payload(i)>=round_amt)
            {
                if (hdr_payload(i)>=round_amt+sizehdr*3)   //splitting
                {
                    int temp=hdr_payload(i);
                    
                    hdr_payload(i)=round_amt;   //allocated header
                    hdr_freeall(i)=1;   //allocated header
                    //hdr_previus(i)=hdr_succesr(i)=0;
                    
                    hdr_payload(i+round_amt+sizehdr)=round_amt;  //allocated footer
                    hdr_freeall(i+round_amt+sizehdr)=1;  //allocated footer
                    //hdr_previus(i+round_amt+sizehdr)=hdr_succesr(i+round_amt+sizehdr)=0;
                    
                    hdr_payload(i+round_amt+sizehdr*2)=temp-(round_amt+sizehdr*2);  //free header
                    hdr_freeall(i+round_amt+sizehdr*2)=0;  //free header
                    hdr_previus(i+round_amt+sizehdr*2)=hdr_previus(i); //free header
                    //hdr_succesr(hdr_previus(i))=i+round_amt+sizehdr*2;
                    hdr_succesr(i+round_amt+sizehdr*2)=hdr_succesr(i); //free header
                    
                    if (hdr_succesr(i)!=heapsize)
                        hdr_previus(hdr_succesr(i))=i+round_amt+sizehdr*2;
                    
                    hdr_payload(i+temp+sizehdr)=temp-(round_amt+sizehdr*2);  //free footer
                    hdr_freeall(i+temp+sizehdr)=0;  //free footer
                    
                    if(hdr_previus(i)==-1)
                        anchor.succesr=i+round_amt+sizehdr*2;
                    else
                        hdr_succesr(hdr_previus(i))=i+round_amt+sizehdr*2;
                    
                    hdr_previus(i)=hdr_succesr(i)=0;
                }
                else    //no splitting
                {
                    hdr_freeall(i)=1;   //allocated header
                    hdr_freeall(i+hdr_payload(i)+sizehdr)=1; //allocated footer
                    
                    hdr_previus(hdr_succesr(i))=hdr_previus(i);
                    
                    if(hdr_previus(i)==-1)
                        anchor.succesr=hdr_succesr(i);
                    else
                        hdr_succesr(hdr_previus(i))=hdr_succesr(i);
                    
                    hdr_previus(i)=hdr_succesr(i)=0;
                }
                flag=1;
                loc=i;  //address returned by lalloc will be the integer location of the payload in the heap (not the header)
                break;  //jump out of the loop
            }
        }
        
        if(flag==0)
        {
            j++;
            int prev_heapsize=heapsize;
            heapsize=HEAPSIZE*j;
            
            //printf("new heap size is %d\n",heapsize);
            
            heap = (char *) realloc( heap, heapsize) ;
            
            
            int last_pl=hdr_payload(prev_heapsize-sizehdr);
            
            if (hdr_freeall(prev_heapsize-sizehdr)==0)
            {
                hdr_payload(prev_heapsize-last_pl-sizehdr*2)=last_pl+HEAPSIZE;
                hdr_payload(heapsize-sizehdr)=last_pl+HEAPSIZE;
                hdr_freeall(heapsize-sizehdr)=0;
                hdr_payload(prev_heapsize-sizehdr)=0;
                hdr_succesr(prev_heapsize-last_pl-sizehdr*2)=heapsize;
            }
            else
            {
                hdr_payload(prev_heapsize) = HEAPSIZE-2*sizehdr ;   // header
                hdr_freeall(prev_heapsize) = 0 ;
                hdr_succesr(prev_heapsize) = heapsize ;
                
                for (i=first;i<heapsize;i=hdr_succesr(i))
                {
                    if (hdr_succesr(i)==prev_heapsize)
                        hdr_previus(prev_heapsize) = i;
                }
                
                hdr_payload(heapsize-sizehdr) = HEAPSIZE-2*sizehdr ; // footer
                hdr_freeall(heapsize-sizehdr) = 0 ;
            }
            //dump_heap( "test heap" ) ;
            
        }
    }
    return loc+sizehdr;
}


int main()
{
    int i ;
    int j = 0  ;
    int blks[100] ;
    
    init_heap() ;
    dump_heap( "init heap" ) ;
    
    for( i=0; i<20; i++ )
        blks[j++] = lalloc( rand()%500 ) ;
    dump_heap( "first allocs" ) ;
    
    lfree( &blks[10] ) ;
    lfree( &blks[11] ) ;
    dump_heap( "coalesce with upper" ) ;
    
    lfree( &blks[14] ) ;
    lfree( &blks[13] ) ;
    dump_heap( "coalesce with lower" ) ;
    
    lfree( &blks[5] ) ;
    lfree( &blks[7] ) ;
    lfree( &blks[6] ) ;
    dump_heap( "coalesce with both" ) ;
    
    //printf("%d\n",heapsize);
    
    blks[5] = lalloc( 80000 ) ;
    //printf("%d\n",heapsize);
    dump_heap( "blew the top off" ) ;
    
    for( i=0; i<20; i++ )
        if (blks[i] != 0 )
        {
            lfree( &blks[i] ) ;
        }
    dump_heap( "free everything " ) ;
    
    return 0 ;
}