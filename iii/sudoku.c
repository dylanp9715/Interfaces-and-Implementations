/* 
 * sudoku.c
 * by Kira Lauring and Dylan Park
 * HW 2 Part C
 * sudoku.c returns 0 if a given PGM file is a solved sudoku board, and returns
 * 0 if it is not.
 */

#include "pnmrdr.h"
#include "uarray2.h"
#include "except.h"

void raise_exception(char *exception_type);
int check_image(Pnmrdr_mapdata board_data);
UArray2_T store_board(Pnmrdr_T board_reader);
int is_solved(UArray2_T sudoku_board);
int check_row(UArray2_T sudoku_board);
int check_col(UArray2_T sudoku_board);
int check_block(UArray2_T sudoku_board, int block_num);

/* BLOCK_DIMEN is used for the dimensions of the 3x3 sudoku blocks */
int const BLOCK_DIMEN = 3;

int main(int argc, char **argv) {
        FILE *fp;
        if (argc == 1) {
                fp = stdin;
        } else if (argc == 2) {
                char *file_name = argv[1];
                fp = fopen(file_name, "rb");
        } else {
                raise_exception("Too many arguments");
        }

        if (!fp) {
                raise_exception("File error");
        }

        Pnmrdr_T board_reader = Pnmrdr_new(fp);
        Pnmrdr_mapdata board_data = Pnmrdr_data(board_reader);
        if (check_image(board_data) == 1) {
                return 1;
        }

        UArray2_T sudoku_board = store_board(board_reader);
        if (sudoku_board == NULL) {
                return 1;
        }

        int board_is_solved = is_solved(sudoku_board);
        Pnmrdr_free(&board_reader);
        UArray2_free(&sudoku_board);
        fclose(fp);
        return board_is_solved;
}

/* Purpose: Prints error message of give error type 
 * Arguments: pointer to char array: exception type
 * Returns: void
 */
void raise_exception(char *exception_type)
{
        Except_T Exception = { exception_type };
        RAISE(Exception);
}

/* Purpose: Checks that image has a pixel count > 0 and that the image is a 
 * portable graymap 
 * Arguments: Pnmrdr_mapdata struct
 * Exceptions: if not portable graymap
 * Returns: int: 1 if not a proper sudoku board
 */
int check_image(Pnmrdr_mapdata board_data)
{
        if (board_data.height != 9 || board_data.width != 9) {
                return 1;
        }
        if (board_data.type != 2) {
                raise_exception("Not portable graymap");
        }
        if (board_data.denominator != 9) {
                return 1;
        }
        return 0;
}

/* Purpose: Reads all numbers from the PGM file and stores them into a 
 * Uarray2_T called sudoku_board
 * Arguments: Pnmrdr_T
 * Returns: Uarray2_T populated with elements
 */
UArray2_T store_board(Pnmrdr_T board_reader) 
{
        UArray2_T sudoku_board = UArray2_new(9, 9, 4);
        for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                        unsigned curr_value = Pnmrdr_get(board_reader);
                        if (curr_value > 0 && curr_value <= 9) {
                                *(unsigned *)UArray2_at(sudoku_board, row, col)
                                        = curr_value;
                        } else {
                                sudoku_board = NULL;
                                return sudoku_board;
                        }
                }
        }

        return sudoku_board;
}

/* Purpose: Checks columns, rows, and 3x3 blocks of the UArray2_T sudoku_board
 * for whether it is solved
 * Arguments: UArray2_T sudoku_board
 * Returns: 0 if solved board, 1 if there is a mistake
 */
int is_solved(UArray2_T sudoku_board)
{
        if ((check_row(sudoku_board) == 1)) {
                return 1;
        } 
        if ((check_col(sudoku_board) == 1)) {
                return 1;
        }
        for (int i = 0; i < 9; ++i) {
                if ((check_block(sudoku_board, i) == 1)) {
                        return 1;
                }
        }

        return 0;
}

/* Purpose: Checks for duplicate numbers in all rows of UArray2_T sudoku_board 
 * Arguments: UArray2_T sudoku_board
 * Returns: 0 if all rows are correct, 1 if any row is incorrect
 */
int check_row(UArray2_T sudoku_board)
{
        int count_freq[9] = {0};
        for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                        int *curr_value = UArray2_at(sudoku_board, row, col);
                        if (count_freq[*curr_value - 1] == 0) {
                                count_freq[*curr_value - 1] = 1;
                        } else {
                                return 1;
                        }
                }
                for (int i = 0; i < 9; ++i) {
                        count_freq[i] = 0;
                }
        }
        return 0;
}

/* Purpose: Checks for duplicate numbers in all columns of UArray2_T 
 * sudoku_board 
 * Arguments: UArray2_T sudoku_board
 * Returns: 0 if all columns are correct, 1 if any column is incorrect
 */
int check_col(UArray2_T sudoku_board) 
{
        int count_freq[9] = {0};
        for (int col = 0; col < 9; ++col) {
                for (int row = 0; row < 9; ++row) {
                        int *curr_value = UArray2_at(sudoku_board, col, row);
                        if (count_freq[*curr_value - 1] == 0) {
                                count_freq[*curr_value - 1] = 1;
                        } else {
                                return 1;
                        }
                } 
                for (int i = 0; i < 9; ++i) {
                        count_freq[i] = 0;
                }
        }
        return 0;
}

/* Purpose: Checks for duplicate numbers in all blocks of UArray2_T 
 * sudoku_board 
 * Arguments: UArray2_T sudoku_board
 * Returns: 0 if all blocks are correct, 1 if any block is incorrect
 */
int check_block(UArray2_T sudoku_board, int block_num)
{
        int count_freq[9] = {0};
        int row_start = (block_num / BLOCK_DIMEN) * BLOCK_DIMEN;
        int col_start = (block_num % BLOCK_DIMEN) * BLOCK_DIMEN;

        for (int row = 0; row < BLOCK_DIMEN; ++row) {
                for (int col = 0; col < BLOCK_DIMEN; ++col) {
                        int *curr_value = UArray2_at(sudoku_board, 
                                row_start + row, col_start + col);
                        if (count_freq[*curr_value - 1] == 0) { 
                                count_freq[*curr_value - 1] = 1;
                        } else {
                                return 1;
                        }
                }
                for (int i = 0; i < 9; ++i) {
                        count_freq[i] = 0;
                }
        }
        return 0;
}