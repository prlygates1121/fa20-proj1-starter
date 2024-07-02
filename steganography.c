/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row)
{
    Color *rowOfColors = (Color*) malloc(image->cols * sizeof(Color));
    if (rowOfColors == NULL) {
        fprintf(stderr, "Error: Memory Allocation Failure\n");
        return NULL;
    }
	for (uint32_t col = 0; col < image->cols; col++) {
        if ((image->image[row][col].B & 1) == 1) {
            rowOfColors[col].R = 255;
            rowOfColors[col].G = 255;
            rowOfColors[col].B = 255;
        } else {
            rowOfColors[col].R = 0;
            rowOfColors[col].G = 0;
            rowOfColors[col].B = 0;
        }
    }
    return rowOfColors;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *newImage = (Image*) malloc(sizeof(Image));
    if (newImage == NULL) {
        fprintf(stderr, "Error: Memory Allocation Failure\n");
        return NULL;
    }
    newImage->rows = image->rows;
    newImage->cols = image->cols;
    newImage->image = (Color**) malloc(newImage->rows * sizeof(Color*));
    if (newImage->image == NULL) {
        free(newImage);
        fprintf(stderr, "Error: Memory Allocation Failure\n");
        return NULL;
    }
    for (uint32_t row = 0; row < newImage->rows; row++) {
        newImage->image[row] = evaluateOnePixel(image, row);
        if (newImage->image[row] == NULL) {
            for (uint32_t i = 0; i < row; i++) {
                free(newImage->image[i]);
            }
            free(newImage);
            return NULL;
        }
    }
    return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
    if (argc != 2) {
        return -1;
    }
    char *filename = argv[1];
	Image *original = readData(filename);
    if (original == NULL) {
        return -1;
    }
    Image *edited = steganography(original);
    if (edited == NULL) {
        freeImage(original);
        return -1;
    }
    writeData(edited);
    freeImage(original);
    freeImage(edited);
    return 0;
}
