#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateARowOfCell(Image *image, int row, uint32_t rule)
{
    Color * newRowOfColors = (Color*) malloc(image->cols * sizeof(Color));
    if (newRowOfColors == NULL) {
        fprintf(stderr, "Error: Memory Allocation Failure\n");
        return NULL;
    }

    // iterate through the whole row
    for (int thisGrid = 0; thisGrid < image->cols; thisGrid++) {
        // calculate the number of adjacent live cells of this cell
        int liveAdjacent = 0;
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = thisGrid - 1; j <= thisGrid + 1; j++) {
                if (i == row && j == thisGrid) { // Don't forget to exclude the current grid itself!!!
                    continue;
                }
                int r = (i + image->rows) % image->rows;
                int c = (j + image->cols) % image->cols;
                if (image->image[r][c].R == 255) {
                    liveAdjacent++;
                }
            }
        }

        // determine the next state according to the rule
        int key;
        if (image->image[row][thisGrid].R == 255) { // if I was alive
            key = ((1 << (9 + liveAdjacent)) & rule) >> (9 + liveAdjacent);
        } else { // if I was dead
            key = ((1 << liveAdjacent) & rule) >> liveAdjacent;
        }
        if (key == 0) { // I am to die
            newRowOfColors[thisGrid].R = 0;
            newRowOfColors[thisGrid].G = 0;
            newRowOfColors[thisGrid].B = 0;
        } else { // I am to live
            newRowOfColors[thisGrid].R = 255;
            newRowOfColors[thisGrid].G = 255;
            newRowOfColors[thisGrid].B = 255;
        }
    }
    return newRowOfColors;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
        newImage->image[row] = evaluateARowOfCell(image, row, rule);
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
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
    if (argc != 3) {
        return -1;
    }
    Image *original = readData(argv[1]);
    if (original == NULL) {
        return -1;
    }
    uint32_t rule = strtol(argv[2], NULL, 0);
    Image *edited = life(original, rule);
    if (edited == NULL) {
        freeImage(original);
        return -1;
    }
    writeData(edited);
    freeImage(original);
    freeImage(edited);
    return 0;
}

#pragma clang diagnostic pop