/*
 * Filename: brightness.c
 * Purpose: Find the average brightness of a PNM greyscale image
 *
 * Authors: Taylor Ampatiellos and Isaac Rothschild
 * Modified: 4 February 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pnmrdr.h"

int main(int argc, char *argv[])
{
        char filename[50];
        FILE *file;
        unsigned num_pixels, sum = 0;
        float avg = 0.0;

        /* Raises CRE if more than 1 file specified */
        assert(argc < 3);

        if (argc == 1)
                scanf("%s", filename);
        else /* argc == 2 */
                strcpy(filename, argv[1]);

        file = fopen(filename, "rb");
        Pnmrdr_T reader = Pnmrdr_new(file); /* Raises CRE if file is NULL */
        Pnmrdr_mapdata data = Pnmrdr_data(reader);

        /* Raises CRE if the file is of an incorrect type */
        assert(data.type == Pnmrdr_gray);

        num_pixels = data.width * data.height;  
        for (unsigned i = 0; i < num_pixels; i++)
                sum += Pnmrdr_get(reader);
        avg = sum / (float)(num_pixels * data.denominator);
        printf("%0.3f\n", avg);
        
        Pnmrdr_free(&reader);
        fclose(file);

        exit(EXIT_SUCCESS);
}
