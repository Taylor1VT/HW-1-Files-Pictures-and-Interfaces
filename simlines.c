/*
 * Filename: simlines.c
 * Purpose: Find matching groups of similar lines in a collection of files.
 *
 * Authors: Taylor Ampatiellos and Isaac Rothschild
 * Modified: 4 February 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mem.h"
#include "atom.h"
#include "table.h"
#include "readaline.h"
#include "normalize.h"

void read_file(FILE *input_file, Table_T table, char *filename);
void print_matching_groups(const void *key, void **value, void *cl);
void free_table(const void *key, void **value, void *cl);

/* Each line_data struct is one node in the hash table lists. */
struct line_data {
        char *filename;
        int line_number;

        struct line_data *next;
};

/*
 * Reads input files line by line, normalizing each line and storing each as a
 * line_data struct in a hash table, using chaining for collision handling.
 * Similar lines are printed in matching groups, and then all data is freed.
 */
int main(int argc, char* argv[])
{
        FILE *input_file;
        Table_T table = Table_new(1000, NULL, NULL);
        int count = 0, *cl = &count;

        for (int i = 1; i < argc; i++) {
                input_file = fopen(argv[i], "r");
                assert(input_file); /* Raises CRE if file pointer is NULL */
                read_file(input_file, table, argv[i]);
                fclose(input_file);
        }

        Table_map(table, print_matching_groups, cl);
        Table_map(table, free_table, NULL);
        Table_free(&table);

        return EXIT_SUCCESS;
}

/*
 * Reads a single file line by line using readaline.
 * Normalizes each line and stores non-empty lines in the hash table
 * as line_data structs. The keys to the hash table are Hanson Atoms.
 *
 * Input: FILE *input_file - pointer to the file being read
 *        Table_T table - Hanson Table instance where data is being stored
 *        char *filename - name of the file being read
 */
void read_file(FILE *input_file, Table_T table, char *filename)
{
        char *ptr1, *ptr2;
        char **in_str = &ptr1, **norm_str = &ptr2;
        int n = readaline(input_file, in_str);

        /* Reads and processes non-empty lines, increments line number */
        for (int line = 1; n > 0; n = readaline(input_file, in_str), line++) {
                const char *atm;

                normalize(*in_str, n, norm_str);

                /* Only store non-empty lines */
                if (**norm_str != '\0') {
                        struct line_data *new_data = ALLOC(sizeof *new_data);
                        struct line_data *temp = new_data;
                        atm = Atom_string(*norm_str);
                        *new_data = (struct line_data) { .filename = filename,
                                                         .line_number = line,
                                                         .next = NULL };
                        temp = Table_get(table, atm);

                        /* If collision, append new_data to linked list */
                        if (temp) {
                                while (temp -> next != NULL)
                                        temp = temp -> next;
                                temp -> next = new_data;
                        } else {
                                Table_put(table, atm, new_data);
                        }
                }

                FREE(*norm_str);
                FREE(*in_str);
        }
}

/*
 * Prints groups of similar lines based on the format detailed in the spec.
 *
 * Input: const void *key - index of hash table element being processed
 *        void **value - pointer to hash table element being processed
 *        void *cl - pointer to the closure
 */

void print_matching_groups(const void *key, void **value, void *cl)
{
        int *count = (int *) cl;
        struct line_data *node = *value;
        struct line_data *temp = node;
        if (node -> next != NULL && *((char *) key) != '\n') {
                if (*count > 0) /* Don't print new line before first group */
                        printf("\n");
                (*count)++;
                printf("%s\n", (char *) key);
                while (temp != NULL) {
                        printf("%*s %*d\n", -20, temp -> filename, 7,
                                                 temp -> line_number);
                        temp = temp -> next;
                }
        }
}

/*
 * Frees the elements of the table.
 *
 * Input: const void *key - index of hash table element being freed
 *        void **value - pointer to hash table element being freed
 *        void *cl - pointer to the closure
 */
void free_table(const void *key, void **value, void *cl)
{
        (void) key;
        (void) cl;

        struct line_data *cur = *value;
        struct line_data *nxt = cur;

        while (cur -> next != NULL) {
                nxt = cur -> next;
                FREE(cur);
                cur = nxt;
        }
        FREE(cur);
}

