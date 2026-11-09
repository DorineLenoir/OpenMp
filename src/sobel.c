/*
	============================================================================
	Name        : sobel.c
	Author      : kdesnos
	Version     : 1.1
	Copyright   : CECILL-C
	Description :
	============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sobel.h"
#include <omp.h>
#include <stdio.h>

#define VERBOSE 1

void sobel(int width, int height, unsigned char *input, unsigned char *output){
    int i,j;
    #pragma omp parallel num_threads(5) 
    {
        #pragma omp for schedule(dynamic,2 )
        for(j=1; j<height-1; j++){

            #ifdef VERBOSE
                printf("Thread %d/%d traite la ligne %d \n", omp_get_thread_num(), omp_get_num_threads(), j);
            #endif

            for(i=1; i<width-1; i++){
                int gx = -input[(j-1)*width + i-1] -2*input[  j*width + i-1] -input[(j+1)*width + i-1]
                        +input[(j-1)*width + i+1] +2*input[  j*width + i+1] +input[(j+1)*width + i+1];
                int gy = -input[(j-1)*width + i-1] -2*input[(j-1)*width + i] -input[(j-1)*width + i+1]
                        +input[(j+1)*width + i-1] +2*input[(j+1)*width + i] +input[(j+1)*width + i+1];

                output[j*width + i] = (unsigned char)sqrt(gx*gx + gy*gy);
            }
            output[j*width] = 255;
            output[(j+1)*width-1] = 255;
        }

        // Border
        memset(output, 255, width);
        memset(output + (height - 1)*width, 255, width);
    }
}
