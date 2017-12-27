/* 
 * uarray2.h
 * by Kira Lauring and Dylan Park
 * HW 2 Part A
 * Uarray2.h is an interface for a 2-D array using UArrays
 */

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"
#include "assert.h"

typedef struct UArray2_T *UArray2_T;

extern UArray2_T UArray2_new(int width, int height, int size); 
extern void UArray2_free(UArray2_T *uarray2); 
extern int UArray2_height(UArray2_T uarray2); 
extern int UArray2_width(UArray2_T uarray2); 
extern int UArray2_size(UArray2_T uarray2); 
extern void *UArray2_at(UArray2_T uarray2, int col, int row); 
extern void UArray2_map_col_major(UArray2_T uarray2, void apply(int col,
        int row, UArray2_T uarray2, void *elem, void *cl), void *cl); 
extern void UArray2_map_row_major(UArray2_T uarray2, void apply(int col, 
        int row, UArray2_T uarray2, void *elem, void *cl), void *cl);

#endif