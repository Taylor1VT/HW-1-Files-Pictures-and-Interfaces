/*
 * Filename: readaline.c
 * Purpose: Read in exactly one line of input from a provided file pointer.
 *
 * Authors: Taylor Ampatiellos and Isaac Rothschild
 * Modified: 4 February 2016 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"
#include "readaline.h"

#define INIT_LEN 200

size_t readaline(FILE *inputfp, char **datap)
{
        assert(inputfp && datap); /* Raises CRE on NULL input */
        char *line = ALLOC(INIT_LEN);
        size_t capacity = INIT_LEN;
        size_t len = 0;
        char c;
        
        for (c = fgetc(inputfp); c != '\n' && c != EOF; c = fgetc(inputfp)) {
                line[len++] = c;
                if (len == capacity) {
                        capacity *= 2;
                        RESIZE(line, capacity);
                }
        }

        /* If EOF (c == EOF), don't append newline char */
        if (feof(inputfp)) {
                if (len == 0) { /* EOF with len == 0 means empty line */ 
                        *datap = NULL;
                        FREE(line);
                        return len;
                }  
        } else {
                line[len++] = c;
        }
        
        assert(!ferror(inputfp)); /* Raises CRE if fgetc encountered error */

        line[len] = '\0';
        *datap = line;

        return len;
}

