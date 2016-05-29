/*
 * Filename: normalize.c
 * Purpose: Normalizes a given line.
 *
 * Authors: Taylor Ampatiellos and Isaac Rothschild
 * Modified: 4 February 2016
 */

#include <stdio.h>
#include "mem.h"
#include "normalize.h"

/*
 * Removes all non-word characters from a given line, separating each group of
 * word characters (words) with a space.
 * Input with no word characters is output as the empty string ("\0").
 *
 * Word characters are all alpha-numerals (0-9, a-z, A-Z).
 * Non-word characters are any other ASCII characters.
 *
 * Input: char *input - the original line to be normalized
 *        size_t len - the length of the original line
 *        char **output - pointer to where the normalized line will be stored
 */
void normalize(char *input, size_t len, char **output)
{
        /* Normalized line will not be longer than the original */
        char *normal = ALLOC(len);
        size_t pos = 0, itr = 0;

        while (itr < len) {
                while (input[itr] != '\0' && !is_word_char(input[itr]))
                        itr++;

                if (input[itr] != '\0' && pos > 0)
                        normal[pos++] = ' ';

                while (input[itr] != '\0' && is_word_char(input[itr]))
                        normal[pos++] = input[itr++];
                
                itr++;
        }

        normal[pos] = '\0';
        *output = normal;
}

/*
 * Checks if given character is a word character or not, as defined above.
 *
 * Input: char c - character being checked
 */
int is_word_char(char c)
{
        int value = c;
        return ((value >= 48 && value <= 57)  ||
                (value >= 65 && value <= 90)  ||
                (value >= 97 && value <= 122) ||
                (value == 95));
}