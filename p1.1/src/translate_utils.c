/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "translate_utils.h"

/* Writes the instructions as a string to OUTPUT. NAME is the name of the
   instruction, and its arguments are in ARGS. NUM_ARGS is the length of
   the array.
 */
void write_inst_string(FILE *output, const char *name, char **args, int num_args) {
    int i;

    fprintf(output, "%s", name);
    /* for loop */
    for (i = 0; i < num_args; i++)
        fprintf(output, " %s", args[i]);
    fprintf(output, "\n");
}

/* Writes the instruction to OUTPUT in hexadecimal format. */
void write_inst_hex(FILE *output, uint32_t instruction) {
    fprintf(output, "%08x\n", instruction);
}

/* Returns 1 if the label is valid and 0 if it is invalid. A valid label is one
   where the first character is a character or underscore and the remaining
   characters are either characters, digits, or underscores.
 */
int is_valid_label(const char *str) {
    int first = 1;
    if (!str)
        return 0;
    /*while*/
    while (*str) {
        if (first) {
            if (!isalpha((int) *str) && *str != '_')
                return 0; /* does not start with letter or underscore */
            else
                first = 0;
        } else if (!isalnum((int) *str) && *str != '_')
            return 0; /* subsequent characters not alphanumeric */
        str++;
    }
    return first ? 0 : 1; /* empty string is invalid  */
}

/* Translate the input string into a signed number. The number is then
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND.

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants.

   You should store the result into the location that OUTPUT points to. The
   function returns 0 if the conversion proceeded without errors, or -1 if an
   error occurred.
 */
int translate_num(long int *output, const char *str, long int lower_bound,
                  long int upper_bound) {
    /*define variable*/
    long result;
    char *endptr;
    /*long range = *output;*/
    /*hex*/
    char *h1 = "0x";
    char *h2 = "0X";
    if (!str)
        return -1;

    /*hex condition*/
    if (strstr(str, h1) || strstr(str, h2)) {
        result = strtol(str, &endptr, 16);
        /*error checking*/
        if (*endptr != '\0')
            return -1;
        /*31 bits condition*/
        if (((result >> 31) & 1) == 1)
            result = -((0xFFFFFFFF - result) + 1);
        /*20 bits condition*/
        else if (((result >> (20 - 1)) & 1) == 1)
            result = -((0xFFFFF - result) + 1);
        /*12 bits condition*/
        else if (((result >> (12 - 1)) & 1) == 1)
            result = -((0xFFF - result) + 1);
    /*dec condition*/
    } else {
        result = strtol(str, &endptr, 10);
        /*error checking*/
        if (*endptr != '\0')
            return -1;
        /*20 bits condition*/
        if (*output == 20 && ((result >> (20 - 1)) & 1) == 1)
            result = -((0xFFFFF - result) + 1);
    }
    if (result < lower_bound || result > upper_bound) {
        return -1;
    }
    *output = result; /*may change to strtol(str, &endptr, 0)*/
    return 0;
}
/* Translates the register name to the corresponding register number. Please
   see the RISC-V Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char *str) {
    if (strcmp(str, "zero") == 0 || strcmp(str, "x0") == 0)
        return 0;
    else if (strcmp(str, "ra") == 0 || strcmp(str, "x1") == 0) /*ra*/
        return 1;
    else if (strcmp(str, "sp") == 0 || strcmp(str, "x2") == 0) /*following are pointers register*/
        return 2;
    else if (strcmp(str, "gp") == 0 || strcmp(str, "x3") == 0)
        return 3;
    else if (strcmp(str, "tp") == 0 || strcmp(str, "x4") == 0)/*tp*/
        return 4;
    else if (strcmp(str, "t0") == 0 || strcmp(str, "x5") == 0) /*temp register*/
        return 5;
    else if (strcmp(str, "t1") == 0 || strcmp(str, "x6") == 0)
        return 6;
    else if (strcmp(str, "t2") == 0 || strcmp(str, "x7") == 0)/*t2*/
        return 7;
    else if (strcmp(str, "s0") == 0 || strcmp(str, "fp") == 0 || strcmp(str, "x8") == 0) /*s register*/
        return 8;
    else if (strcmp(str, "s1") == 0 || strcmp(str, "x9") == 0)
        return 9;
    else if (strcmp(str, "a0") == 0 || strcmp(str, "x10") == 0)/*a*/
        return 10;
    else if (strcmp(str, "a1") == 0 || strcmp(str, "x11") == 0)
        return 11;
    else if (strcmp(str, "a2") == 0 || strcmp(str, "x12") == 0)/*a2*/
        return 12;
    else if (strcmp(str, "a3") == 0 || strcmp(str, "x13") == 0)
        return 13;
    else if (strcmp(str, "a4") == 0 || strcmp(str, "x14") == 0)/*a4*/
        return 14;
    else if (strcmp(str, "a5") == 0 || strcmp(str, "x15") == 0)
        return 15;
    else if (strcmp(str, "a6") == 0 || strcmp(str, "x16") == 0)/*a6*/
        return 16;
    else if (strcmp(str, "a7") == 0 || strcmp(str, "x17") == 0)
        return 17;
    else if (strcmp(str, "s2") == 0 || strcmp(str, "x18") == 0)/*s2*/
        return 18;
    else if (strcmp(str, "s3") == 0 || strcmp(str, "x19") == 0)
        return 19;
    else if (strcmp(str, "s4") == 0 || strcmp(str, "x20") == 0)/*s4*/
        return 20;
    else if (strcmp(str, "s5") == 0 || strcmp(str, "x21") == 0)
        return 21;
    else if (strcmp(str, "s6") == 0 || strcmp(str, "x22") == 0)/*s6*/
        return 22;
    else if (strcmp(str, "s7") == 0 || strcmp(str, "x23") == 0)
        return 23;
    else if (strcmp(str, "s8") == 0 || strcmp(str, "x24") == 0)/*s8*/
        return 24;
    else if (strcmp(str, "s9") == 0 || strcmp(str, "x25") == 0)
        return 25;
    else if (strcmp(str, "s10") == 0 || strcmp(str, "x26") == 0)/*s10*/
        return 26;
    else if (strcmp(str, "s11") == 0 || strcmp(str, "x27") == 0)
        return 27;
    else if (strcmp(str, "t3") == 0 || strcmp(str, "x28") == 0)/*st3*/
        return 28;
    else if (strcmp(str, "t4") == 0 || strcmp(str, "x29") == 0)
        return 29;
    else if (strcmp(str, "t5") == 0 || strcmp(str, "x30") == 0)/*t5*/
        return 30;
    else if (strcmp(str, "t6") == 0 || strcmp(str, "x31") == 0)
        return 31;
        /*return*/
    else
        return -1;
}
