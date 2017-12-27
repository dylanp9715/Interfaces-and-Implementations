/* 
 * removeblackedge.h
 * by Kira Lauring and Dylan Park
 * HW 2 Part D
 * removeblackedge.h is the interface that removes all black edges in a bit2
 */

#include "bit2.h"
#include "stack.h"

Bit2_T remove_blackedges(Bit2_T bit2);
void remove_blackedge_path(Bit2_T bit2, int col, int row);
Stack_T get_edge_neighbor(Stack_T stack, Bit2_T bit2, int col, int row);
Stack_T check_neighbors(Stack_T stack, Bit2_T bit2, int my_col, int my_row);
int is_corner(Bit2_T bit2, int col, int row);
int is_right_edge(Bit2_T bit2, int col, int row);
int is_left_edge(Bit2_T bit2, int col, int row);
int is_bottom_edge(Bit2_T bit2, int col, int row);
int is_top_edge(Bit2_T bit2, int col, int row);
struct bit_info *make_bit_info();