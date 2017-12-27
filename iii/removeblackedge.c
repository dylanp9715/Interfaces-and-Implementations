/* 
 * removeblackedge.c
 * by Kira Lauring and Dylan Park
 * HW 2 Part D
 * removeblackedge.c is the implementation that removes all black edges in bit2
 */

#include "removeblackedge.h"

struct bit_info {
        int row;
        int col;
};

/* Purpose: Removes all black edges in a pbm
 * Arguments: Bit2_T
 * Returns: Bit2T (updated with all black edges removed)
 */
Bit2_T remove_blackedges(Bit2_T bit2)
{
        for (int i = 0; i < Bit2_width(bit2); ++i) {
                remove_blackedge_path(bit2, i, 0);
        }
        for (int j = 0; j < Bit2_height(bit2); ++j) {
                remove_blackedge_path(bit2, 0, j);
        }
        for (int k = 0; k < Bit2_width(bit2); ++k) {
                remove_blackedge_path(bit2, k, Bit2_height(bit2) - 1);
        }
        for (int l = 0; l < Bit2_height(bit2); ++l) {
                remove_blackedge_path(bit2, Bit2_width(bit2) - 1, l);
        }

        return bit2;
}

/* Purpose: Removes all black edges connected to a single given edge
 *  Arguments: Bit2_T, 2 ints: column number and row number (an edge)
 *  Returns: void
 */ 
void remove_blackedge_path(Bit2_T bit2, int col, int row) 
{
        Stack_T stack = Stack_new();
        int bit = Bit2_get(bit2, col, row);

        if (bit == 0) {
                Stack_free(&stack);
                return;
        }
        Bit2_put(bit2, col, row, 0); 
        stack = get_edge_neighbor(stack, bit2, col, row);

        while (!Stack_empty(stack)) {
                struct bit_info *curr_bit_info;
                curr_bit_info = Stack_pop(stack);
                int my_col = curr_bit_info->col;
                int my_row = curr_bit_info->row;
                free(curr_bit_info);
                Bit2_put(bit2, my_col, my_row, 0); 
                if ((my_col > 0 && my_col < (Bit2_width(bit2) - 1)) && 
                        (my_row > 0 && my_row < (Bit2_height(bit2) - 1))) {
                        stack = check_neighbors(stack, bit2, my_col, my_row);
                }                       
        }

        Stack_free(&stack);
        return;     
}

/* Purpose: Gets the neighbor of a bit on the edge of a Bit2_T array
 * Arguments: Stack_T, Bit2_T, and 2 ints for both columns and rows
 * Returns: Stack_T with neighbor on it
 */
Stack_T get_edge_neighbor(Stack_T stack, Bit2_T bit2, int col, int row)
{
        struct bit_info *new_bit_info = make_bit_info();
        new_bit_info->col = col;
        new_bit_info->row = row;

        if (is_corner(bit2, col, row) == 0) {
                free(new_bit_info);
                return stack;
        } else if (is_top_edge(bit2, col, row) == 0 
                        && Bit2_get(bit2, col, row + 1) == 1) {
                new_bit_info->row = new_bit_info->row + 1;
                Stack_push(stack, (void *)new_bit_info);
                return stack;
        } else if (is_left_edge(bit2, col, row) == 0 
                        && Bit2_get(bit2, col + 1, row) == 1) {
                new_bit_info->col = new_bit_info->col + 1;
                Stack_push(stack, (void *)new_bit_info);
                return stack;
        } else if (is_right_edge(bit2, col, row) == 0 
                        && Bit2_get(bit2, col - 1, row) == 1) {
                new_bit_info->col = new_bit_info->col - 1;
                Stack_push(stack, (void *)new_bit_info);
                return stack;
        } else if (is_bottom_edge(bit2, col, row) == 0 
                        && Bit2_get(bit2, col, row - 1) == 1) {
                new_bit_info->row = new_bit_info->row - 1;
                Stack_push(stack, (void *)new_bit_info);
                return stack;
        }
        else {
                free(new_bit_info);
        }
        return stack;
}

/* Purpose: Checks every neighbor of the bit at (my_col, my_row)
*  Arguments: Stack_T, Bit2_T, and pointers to ints for both columns and rows
*  Returns: Stack_T with neighbors on it
*/
Stack_T check_neighbors(Stack_T stack, Bit2_T bit2, int my_col, int my_row)
{
        if (Bit2_get(bit2, my_col - 1, my_row) == 1 
                && is_left_edge(bit2, my_col - 1, my_row) == 1) {
                struct bit_info *new_bit_info = make_bit_info();
                new_bit_info->row = my_row;
                new_bit_info->col = my_col - 1;
                Stack_push(stack, (void *)new_bit_info);
        } if (Bit2_get(bit2, my_col + 1, my_row) == 1 
                && is_right_edge(bit2, my_col + 1, my_row) == 1) {
                struct bit_info *new_bit_info = make_bit_info();
                new_bit_info->row = my_row;
                new_bit_info->col = my_col + 1;
                Stack_push(stack, (void *)new_bit_info);
        } if (Bit2_get(bit2, my_col, my_row - 1) == 1 
                && is_top_edge(bit2, my_col, my_row - 1) == 1) {
                struct bit_info *new_bit_info = make_bit_info();
                new_bit_info->col = my_col;
                new_bit_info->row = my_row - 1;
                Stack_push(stack, (void *)new_bit_info);
        } if (Bit2_get(bit2, my_col, my_row  + 1) == 1 
                && is_bottom_edge(bit2, my_col, my_row + 1) == 1) {
                struct bit_info *new_bit_info = make_bit_info();
                new_bit_info->col = my_col;
                new_bit_info->row = my_row + 1;
                Stack_push(stack, (void *)new_bit_info);
        }
        return stack;
}

/* Purpose: Checks if the bit found at (col, row) is in a corner of the 
 * Bit2_T array
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Returns: 1 if it is not in corner and 0 otherwise
 */
int is_corner(Bit2_T bit2, int col, int row)
{
        if ((col == 0 && row == 0) || (col == Bit2_width(bit2)-1 && row == 0)
                || (col == 0 && row == Bit2_height(bit2)-1) 
                || (col == Bit2_width(bit2)-1 && row == Bit2_height(bit2)-1)) {
                return 0;
        }
        return 1;
}

/* Purpose: Checks if bit found at (col, row) is on the right edge of the 
 * Bit2_T array
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Retuns: 1 if it is not on right edge of the Bit2_T array and 0 otherwise
 */ 
int is_right_edge(Bit2_T bit2, int col, int row)
{
        if (col == Bit2_width(bit2) - 1 && row < Bit2_height(bit2)) {    
                return 0;
        }
        return 1;
}

/* Purpose: Checks if bit found at (col, row) is on the left edge of the 
 * Bit2_T array
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Retuns: 1 if it is not on left edge of the Bit2_T array and 0 otherwise
 */ 
int is_left_edge(Bit2_T bit2, int col, int row)
{
        if (col == 0 && row < Bit2_height(bit2)) {
                return 0;
        }
        return 1;
}

/* Purpose: Checks if bit found at (col, row) is on the bottom edge of the 
 * Bit2_T array
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Retuns: 1 if it is not on bottom edge of the Bit2_T array and 0 otherwise
 */ 
int is_bottom_edge(Bit2_T bit2, int col, int row)
{
        if (row == Bit2_height(bit2) - 1 && col < Bit2_width(bit2)) {
                return 0;
        }
        return 1;
}

/* Purpose: Checks if bit found at (col, row) is on the top edge of the 
 * Bit2_T array
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Retuns: 1 if it is not on top edge of the Bit2_T array and 0 otherwise
 */ 
int is_top_edge(Bit2_T bit2, int col, int row)
{
        if (row == 0 && col < Bit2_width(bit2)) {
                return 0;
        }
        return 1;
}

/* Purpose: Initializes the bit info struct
 *  Arguments: none
 *  Returns: initialized struct
 */ 
struct bit_info *make_bit_info()
{
        struct bit_info new_bit_info;
        new_bit_info.col = 0;
        new_bit_info.row = 0;
        struct bit_info *new_bit_info_p = &new_bit_info;
        new_bit_info_p = malloc(sizeof(struct bit_info));
        return new_bit_info_p;
}