/*
 * Filename: normalize.h
 *
 * Authors: Taylor Ampatiellos and Isaac Rothschild
 * Modified: 4 February 2016
 */

#ifndef NORMALIZE_INCLUDED
#define NORMALIZE_INCLUDED

#include <stdio.h>

/* normalizes the line by removing non-word characters */
void normalize(char *input, size_t len, char **output);

/* checks if a given character is a word or non-word character */
int is_word_char(char c);

#endif /* normalize.h */