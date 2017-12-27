/*
 *                      useuarray2.c
 *
 *         This program illustrates the use of the uarray2 interface.
 *
 *         Although it will catch some errors in some uarray2 implementations
 *         it is NOT a thorough test program.
 *
 *         NOTE: this program is commented sparsely, as figuring out
 *         what this program does and why the tests it makes matter is
 *         part of the homework assignment.
 *
 *         Author: Noah Mendelsohn
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <uarray2.h>

typedef long number; // type of element stored at (5,7)


const int DIM1 = 5; // number of cols
const int DIM2 = 7; // number of rows
const int ELEMENT_SIZE = sizeof(number); // number of bits for number
const int MARKER = 99; 

void
check_and_print(int i, int j, UArray2_T a, void *p1, void *p2) 
{
        number *entry_p = p1;
        
        // if entry_p = 
        
        *((bool *)p2) &= UArray2_at(a, i, j) == entry_p;
        //printf("entry: %ld" *entry_p);
        //(long *)element = UArray2_at(a,i,j);
        //printf("%ld\n", element);
        //printf("%s", p2 ? "true" : "false");

        if ( (i == (DIM1 - 1) ) && (j == (DIM2 - 1) ) ) {
                /* we got the corner */
                *((bool *)p2) &= (*entry_p == MARKER);
        }

        printf("ar[%d,%d]\n", i, j);
}

int
main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        UArray2_T test_array;
        bool OK = true;

        test_array = UArray2_new(DIM1, DIM2, ELEMENT_SIZE);

        // conditions we want OK to have in test_array
        OK = (UArray2_width(test_array) == DIM1) &&
	     (UArray2_height(test_array) == DIM2) &&
             (UArray2_size(test_array) == ELEMENT_SIZE);


        /* Note: we are only setting a value on the corner of the array */
        // returns pointer to element number i
        // set MARKER to whatever's at (5,7) which is type number
        *((number *)UArray2_at(test_array, DIM1 - 1, DIM2 - 1)) = MARKER;
        //long value = *((long *)UArray2_at(test_array, DIM1 - 1, DIM2 - 1));
        //printf("%lu\n", value);

        printf("Trying column major\n");
        UArray2_map_col_major(test_array, check_and_print, &OK);
        //fprintf(stderr, "%i\n", UArray2_width(test_array));
        //fprintf(stderr, "%i\n", UArray2_height(test_array));
        //fprintf(stderr, "%i\n", UArray2_size(test_array));

        printf("Trying row major\n");
        UArray2_map_row_major(test_array, check_and_print, &OK);
        //fprintf(stderr, "%i\n", UArray2_width(test_array));
        //fprintf(stderr, "%i\n", UArray2_height(test_array));
        //fprintf(stderr, "%i\n", UArray2_size(test_array));

        UArray2_free(&test_array);
        if (!OK) {
                    printf("Not ok\n");

        }
        printf("The array is %sOK!\n", (OK ? "" : "NOT "));

}
