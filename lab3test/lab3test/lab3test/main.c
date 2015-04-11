//
//  main.c
//  lab3test
//
//  Created by Benson on 11/26/14.
//  Copyright (c) 2014 Big Nerd Ranch. All rights reserved.
//

/*
 int A = a-bo ;
 for( int k=0; k<B; k++ )
 cache[si][li].block[k] = memory[A+k] ;*/

#include <stdio.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    int a=25374;
    int bb;
    int si,ta,bo;
    int T=7,b=5,s=4;
    unsigned int temp= (unsigned int) a;
   /*
    ta=(temp&0xfe00)>>(s+b);
    si=((temp&0x01e0)<<T)>>(T+b);
    bo=temp&0x1f;
    */

    
    
    
    si = (temp >> b) & ((1 << s) - 1);
    ta = (temp >> (b + s)) & ((1 << T) - 1);
    bo = temp & ((1 << b) - 1);
    
    //bb=a-bo;
    printf("ta= %d  si= %d  bo= %d  ",ta, si, bo);
    int TA=ta<<(s+b);
    int SI=si<<b;
    printf("\nTA= %d  SI= %d",TA, SI);
    return 0;
}
