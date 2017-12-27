/* 
 * bit2.h
 * by Kira Lauring and Dylan Park
 * HW 2 Part B
 * The implementation for a two-dimensional array of bits
 */

#include "bit2.h"

struct Bit2_T {
        int height;
        int width;
        UArray_T outer;
};

/* Purpose: Creates a new UArray of size width and populates it with Bit 
 * vectors of size height
 * Arguments: 2 ints: width and height of the 2-D array
 * element in the array
 * Returns: Bit2_T
 */
Bit2_T Bit2_new(int width, int height)
{
        Bit2_T bit2 = malloc(sizeof(struct Bit2_T));
        bit2->height = height;
        bit2->width = width;
        bit2->outer = UArray_new(width, 8);
        for (int i = 0; i < width; ++i) {
                Bit_T *my_bit2 = UArray_at(bit2->outer, i);
                *my_bit2 = Bit_new(height);
        }
       
        return bit2; 
}

/* Purpose: Provides the width of the Bit2_T
 * Arguments: Bit2_T
 * Returns: int: width
 */
int Bit2_width(Bit2_T bit2) 
{
        assert(bit2 != NULL);
        return bit2->width;
} 

/* Purpose: Provides the height of the Bit2_T
 * Arguments: Bit2_T
 * Returns: int: height
 */
int Bit2_height(Bit2_T bit2) 
{
        assert(bit2 != NULL);
        return bit2->height;
}

/* Purpose: Frees all memory used for Bit2_T
 * Arguments: pointer to Bit2_T
 * Returns: void
 */
void Bit2_free(Bit2_T *bit2) 
{
        assert(bit2 != NULL && *bit2 != NULL);
        for (int i = 0; i < Bit2_width(*bit2); ++i) {
                Bit_free((Bit_T *)UArray_at(((*bit2)->outer), i));
        }
        UArray_free(&((*bit2)->outer));
        free(*bit2);
}

/* Purpose: Retrieves the bit found at (col, row)
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Returns: int: bit at (col, row)
 */
int Bit2_get(Bit2_T bit2, int col, int row)
{
        assert(bit2 != NULL);
        assert(row < bit2->height && row >= 0);
        assert(col < bit2->width && col >= 0);
        Bit_T *location = UArray_at(bit2->outer, col);
        return Bit_get(*location, row);
}

/* Purpose: Puts the bit passed in to (col, row)
 * Arguments: Bit2_T, 3 ints: column number, row number, and bit value
 * Returns: int: previous value stored at (col, row)
 */
int Bit2_put(Bit2_T bit2, int col, int row, int bit) 
{
        assert(bit2 != NULL);
        assert(row < bit2->height && row >= 0);
        assert(col < bit2->width && col >= 0);
        Bit_T *location = UArray_at(bit2->outer, col);
        return Bit_put(*location, row, bit);
}

/* Purpose: Calls the apply function column by column on the Bit2_T
 * Arguments: Bit2_T, apply function, and void pointer closure
 * Returns: void
 */
void Bit2_map_row_major(Bit2_T bit2, void apply(int col, int row, Bit2_T bit2,
        int elem, void *cl), void *cl)
{
        assert(bit2 != NULL);
        for (int row = 0; row < bit2->height; ++row) {
                for (int col = 0; col < bit2->width; ++col) {
                        apply(col, row, bit2, Bit2_get(bit2, col, row), cl);
                }
        }
}

/* Purpose: Calls the apply function row by row on the Bit2_T
 * Arguments: Bit2_T, apply function, and void pointer closure
 * Returns: void
 */
void Bit2_map_col_major(Bit2_T bit2, void apply(int col, int row, Bit2_T bit2, 
        int elem, void *cl), void *cl)
{
        assert(bit2 != NULL);
        for (int col = 0; col < bit2->width; ++col) {
                for (int row = 0; row < bit2->height; ++row) {
                        apply(col, row, bit2, Bit2_get(bit2, col, row), cl);
                }
        }
}
