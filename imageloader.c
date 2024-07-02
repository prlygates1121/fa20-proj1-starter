/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"



//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable To Open Files\n");
        //fclose(fp); // Don't use fclose() on a NULL pointer!!!
        return NULL;
    }

    char P3[3];
    fscanf(fp, "%s", P3);
    if (P3[0] != 'P' || P3[1] != '3') {
        fprintf(stderr, "Error: Incorrect Image Format\n");
        fclose(fp);
        return NULL;
    }

    Image *im = (Image*) malloc(sizeof(Image));
    if (im == NULL) {
        fprintf(stderr, "Error: Memory Allocation Failure\n");
        return NULL;
    }
    fscanf(fp, "%"PRIu32" %"PRIu32"", &im->cols, &im->rows);

    int maxColors;
    fscanf(fp, "%d", &maxColors);
    if (maxColors != 255) {
        fprintf(stderr, "Error: Incorrect Total Colors (not 255)\n");
        free(im);
        fclose(fp);
        return NULL;
    }

    im->image = (Color **) malloc(im->rows * sizeof(Color *));
    for (uint32_t row = 0; row < im->rows; row++) {
        im->image[row] = (Color *) malloc(im->cols * sizeof(Color));
        if (im->image[row] == NULL) {
            for (uint32_t i = 0; i < row; i++) {
                free(im->image[i]);
            }
            free(im);
            fprintf(stderr, "Error: Memory Allocation Failure\n");
            return NULL;
        }
        for (uint32_t col = 0; col < im->cols; col++) {
            fscanf(fp, "%3hhu %3hhu %3hhu", &(im->image[row][col].R), &(im->image[row][col].G), &(im->image[row][col].B));
        }
    }

    fclose(fp);
    return im;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    printf("P3\n");
    printf("%"PRIu32" %"PRIu32"\n", image->cols, image->rows);
    printf("255\n");
    for (uint32_t row = 0; row < image->rows; row++) {
        for (uint32_t col = 0; col < image->cols; col++) {
            printf("%3hhu %3hhu %3hhu", image->image[row][col].R, image->image[row][col].G, image->image[row][col].B);
            if (col != image->cols - 1) {
                printf("   ");
            }
        }
        printf("\n");
    }

}

//Frees an image
void freeImage(Image *image)
{
    for (uint32_t row = 0; row < image->rows; row++) {
        free(image->image[row]);
    }
    free(image->image);
    free(image);
}