/* 
 * uarray2.c
 * by Kira Lauring and Dylan Park
 * HW 2 Part A
 * The implementation for a two-dimensional array that can hold any type
 */

#include "uarray2.h"

struct UArray2_T {
        int height;
        int width;
        int size;
        UArray_T outer;
};

/* Purpose: Creates a new UArray populated with UArrays of size height by width
 * Arguments: 3 ints: width and height of the 2-D array and the size of each 
 * element in the array
 * Returns: UArray2_T
 */
 UArray2_T UArray2_new(int width, int height, int size) 
 {

        UArray2_T uarray2 = malloc(sizeof(struct UArray2_T));
        uarray2->height = height;
        uarray2->width = width;
        uarray2->size = size;
        uarray2->outer = UArray_new(width, 8);
        for (int i = 0; i < width; ++i) {
                UArray_T *my_uarray = UArray_at(uarray2->outer, i);
                *my_uarray = UArray_new(height, size);
        }
        
        return uarray2; 
}

/* Purpose: Provides the height of the UArray2_T
 * Arguments: UArray2_T
 * Returns: int: height
 */
int UArray2_height(UArray2_T uarray2) 
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/* Purpose: Provides the width of the UArray2_T
 * Arguments: UArray2_T
 * Returns: int: width
 */
int UArray2_width(UArray2_T uarray2) 
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/* Purpose: Provides the size of each element found within UArray2_T
 * Arguments: UArray2_T
 * Returns: int: size of element
 */
int UArray2_size(UArray2_T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

/* Purpose: Provides the element found at (col, row) of the UArray2_T
 * Arguments: UArray2_T, 2 ints: column number and row number
 * Returns: void pointer to the element at (col, row)
 */
void *UArray2_at(UArray2_T uarray2, int col, int row)
{
        assert(uarray2 != NULL);
        assert(row < uarray2->height && row >= 0);
        assert(col < uarray2->width && col >= 0);
        UArray_T *uarray_p = UArray_at((uarray2->outer), col);
        return UArray_at(*uarray_p, row);
}

/* Purpose: Frees all memory used for UArray2_T
 * Arguments: pointer to UArray2_T
 * Returns: int: void
 */
void UArray2_free(UArray2_T *uarray2) 
{
        assert(uarray2 != NULL && *uarray2 != NULL);
        for (int i = 0; i < UArray2_width(*uarray2); ++i) {
                UArray_free((UArray_T *)UArray_at(((*uarray2)->outer), i));
        }
        UArray_free(&((*uarray2)->outer));
        free(*uarray2);
}

/* Purpose: Calls the apply function column by column on the UArray2_T
 * Arguments: UArray2_T, apply function, and void pointer closure
 * Returns: void
 */
void UArray2_map_row_major(UArray2_T uarray2, void apply(int col, int row, 
        UArray2_T uarray2, void *elem, void *cl), void *cl)
{
        assert(uarray2 != NULL);
        for (int row = 0; row < uarray2->height; ++row) {
                for (int col = 0; col < uarray2->width; ++col) {
                        apply(col, row, uarray2, UArray2_at(uarray2, col, row), 
                                cl);
                }
        }
}

/* Purpose: Calls the apply function row by row on the UArray2_T
 * Arguments: UArray2_T, apply function, and void pointer closure
 * Returns: void
 */
void UArray2_map_col_major(UArray2_T uarray2, void apply(int col, int row, 
        UArray2_T uarray2, void *elem, void *cl), void *cl)
{
        assert(uarray2 != NULL);
        for (int col = 0; col < uarray2->width; ++col) {
                for (int row = 0; row < uarray2->height; ++row) {
                        apply(col, row, uarray2, UArray2_at(uarray2, col, row),
                                cl);
                }
        }
}