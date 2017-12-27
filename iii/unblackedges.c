/* 
 * unblackedges.c
 * by Kira Lauring and Dylan Park
 * HW 2 Part D
 * unblackedges removes all black edges of a PBM file and prints an updated PBM
 * file to stdout.
 */

#include "pnmrdr.h"
#include "bit2.h"
#include "except.h"
#include "stack.h"
#include "removeblackedge.h"

void raise_exception(char *exception_type);
void check_image(Pnmrdr_mapdata file_data);
Bit2_T pbm_read (FILE *inputfp);
void raise_exception(char *exception_type);
void Bit2_print(int row, int col, Bit2_T bit2, int elem, void *cl);
void pbm_print(Bit2_T bit2);



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

        Bit2_T bit2 = pbm_read(fp);
        bit2 = remove_blackedges(bit2);
        pbm_print(bit2);
        Bit2_free(&bit2);
        fclose(fp);
}

/* Purpose: Prints a bit (apply function)
 * Arguments: Bit2_T, 2 ints: column number and row number
 * Returns: 1 if it is not on right edge of the Bit2_T array and 0 otherwise
 */ 
void Bit2_print(int row, int col, Bit2_T bit2, int elem, void *cl)
{
        (void) row;
        (void) col;
        (void) cl;
        (void) bit2;
        printf("%i ", elem);
}

/* Purpose: Ensures that the file provided is a portable bitmap
 *  Arguments: Pnmrdr_mapdata struct that contains info about file
 *  Returns: void
 */
void check_image(Pnmrdr_mapdata file_data)
{
        if (file_data.type != 1) {
                raise_exception("Not portable bitmap");
        }
}

/* Purpose: Reads all bits from file and stores them into a  Bit2_T
 *  Arguments: FILE pointer to a pbm file
 *  Returns: Bit2_T 
 */
Bit2_T pbm_read(FILE *inputfp)
{
        Pnmrdr_T file_reader = Pnmrdr_new(inputfp);
        Pnmrdr_mapdata file_data = Pnmrdr_data(file_reader);
        check_image(file_data);

        Bit2_T file = Bit2_new(file_data.width, file_data.height);
        for (unsigned row = 0; row < file_data.height; ++row) {
                for (unsigned col = 0; col < file_data.width; ++col) {
                        int curr_value = Pnmrdr_get(file_reader);
                        if (curr_value == 1 || curr_value == 0) {
                                Bit2_put(file, col, row, curr_value);
                        } else {
                                raise_exception("Value is not a bit");
                        }
                }
        }
        Pnmrdr_free(&file_reader);
        return file;
}

/* Purpose: Raises an exception of the type passed in as an argument
 * Arguments: pointer to character array: type of exception to be raised
 * Retuns: void
 */
void raise_exception(char *exception_type)
{
        Except_T Exception = { exception_type };
        RAISE(Exception);
}

/* Purpose: Prints a PBM from a passed Bit2_T
 * Arguments: Bit2_T
 * Returns: void
 */
void pbm_print(Bit2_T bit2)
{
        printf("%s\n", "P1");
        printf("%i %i\n", Bit2_width(bit2), Bit2_height(bit2));
        int counter = 1;
        for (int row = 0; row < Bit2_height(bit2); ++row) {
                for (int col = 0; col < Bit2_width(bit2); ++col) {
                        if (counter > 70) {
                                printf("\n");
                                counter = 0;
                        }
                        printf("%i", Bit2_get(bit2, col, row));
                        counter++;
                }
                counter = 1;
                printf("\n");
        }
}