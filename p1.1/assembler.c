/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhangs and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/utils.h"
#include "src/tables.h"
#include "src/translate_utils.h"
#include "src/translate.h"

#include "assembler.h"


#define MAX_ARGS 3
#define BUF_SIZE 1024
const char *IGNORE_CHARS = " \f\n\r\t\v,()";/**/
/*hexToDec*/
/*

    int64_t i=2;
    int64_t len=strlen(name);
    int64_t dec_num=0;
    int64_t n=0;
    if(name[0] == '0' && name[1] == 'x'){
        for(;i<len;++i){
            if (name[i] >= 97 && name[i] <= 102)
                n = name[i] - 97 + 10;
            
            else if (name[i] >= 65 && name[i] <= 70) 
                n = name[i] - 65 + 10;
            else{
                n = name[i] - 48;
            }
            
            dec_num = dec_num * 16 + n;
        }
        
        if(sprintf(my_return_str,"%ld",dec_num)<0){
            printf("error in assembler.c sprintf");
        }
        
    }else{
        memcpy(my_return_str,name,12*sizeof(char));
    }
    return my_return_str;
}
*/
/*******************************
 * Helper Functions
 *******************************/

/* you should not be calling this function yourself. */
static void raise_label_error(uint32_t input_line, const char *label) {
    write_to_log("Error - invalid label at line %d: %s\n", input_line, label);
}

/* call this function if more than MAX_ARGS arguments are found while parsing
   arguments.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.

   EXTRA_ARG should contain the first extra argument encountered.
 */
static void raise_extra_argument_error(uint32_t input_line, const char *extra_arg) {
    write_to_log("Error - extra argument at line %d: %s\n", input_line, extra_arg);
}

/* You should call this function if write_pass_one() or translate_inst()
   returns -1.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.
 */
static void raise_instruction_error(uint32_t input_line, const char *name, char **args,
                                    int num_args) {

    write_to_log("Error - invalid instruction at line %d: ", input_line);
    log_inst(name, args, num_args);
}

/* Truncates the string at the first occurrence of the '#' character. */
static void skip_comments(char *str) {
    char *comment_start = strchr(str, '#');
    if (comment_start) {
        *comment_start = '\0';
    }
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
   whether it is a valid label, and then tries to add it to the symbol table.

   INPUT_LINE is which line of the input file we are currently processing. Note
   that the first line is line 1 and that empty lines are included in this count.

   BYTE_OFFSET is the offset of the NEXT instruction (should it exist).

   Four scenarios can happen:
    1. STR is not a label (does not end in ':'). Returns 0.
    2. STR ends in ':', but is not a valid label. Returns -1.
    3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
        Returns -1.
    3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
        Returns 1.
 */
static int add_if_label(uint32_t input_line, char *str, uint32_t byte_offset,
                        SymbolTable *symtbl) {

    size_t len = strlen(str);
    /*judge*/
    if (str[len - 1] == ':') {
        str[len - 1] = '\0';
        if (is_valid_label(str)) {
            /*add table*/
            if (add_to_table(symtbl, str, byte_offset) == 0) {
                return 1;
            } else {
                return -1;
            }
            /*else condition*/
        } else {
            raise_label_error(input_line, str);
            return -1;
        }
        /*else condition*/
    } else {
        return 0;
    }
}

/*******************************
 * Implement the Following
 *******************************/

/* First pass of the assembler. You should implement pass_two() first.

   This function should read each line, strip all comments, scan for labels,
   and pass instructions to write_pass_one(). The symbol table should also
   been built and written to specified file. The input file may or may not
   be valid. Here are some guidelines:

    1. Only one label may be present per line. It must be the first token present.
        Once you see a label, regardless of whether it is a valid label or invalid
        label, treat the NEXT token as the beginning of an instruction.
    2. If the first token is not a label, treat it as the name of an instruction.
        DO NOT try to check it is a valid instruction in this pass.
    3. Everything after the instruction name should be treated as arguments to
        that instruction. If there are more than MAX_ARGS arguments, call
        raise_extra_argument_error() and pass in the first extra argument. Do not
        write that instruction to the output file (eg. don't call write_pass_one())
    4. Only one instruction should be present per line. You do not need to do
        anything extra to detect this - it should be handled by guideline 3.
    5. A line containing only a label is valid. The address of the label should
        be the byte offset of the next instruction, regardless of whether there
        is a next instruction or not.
    6. If an instruction contains an immediate, you should output it AS IS.
    7. Comments should always be skipped before any further process.

   Just like in pass_two(), if the function encounters an error it should NOT
   exit, but process the entire file and return -1. If no errors were encountered,
   it should return 0.
 */
int pass_one(FILE *input, FILE *inter, FILE *symtbl) {
    /* YOUR CODE HERE */
    
    /* A buffer for line parsing. */
    char buf[BUF_SIZE];

    /* Variables for argument parsing. */
    char *args[MAX_ARGS];
    int num_args;
    /* For each line, there are some hints of what you should do:
        1. Skip all comments
        2. Use `strtok()` to read next token
        3. Deal with labels
        4. Parse the instruction
     */
    char *name;
    char * instruction_name;
    /*init*/
    uint32_t num_instr = 0;
    uint32_t byte_offset = 0;
    int myreturn_value = 0;
    unsigned int lines_written=0;
    /*table*/
    SymbolTable *myTable;
    if (!input || !inter || !symtbl)
        return -1;
    myTable = create_table_from_file(1, symtbl);/*suppose mode =1*/
    while (fgets(buf, BUF_SIZE, input)) {
        int result;
        num_instr++;
        /*comments*/
        skip_comments(buf);
        name = strtok(buf, IGNORE_CHARS);
        if (!name) {
            continue;
        }
        /*result*/
        result = add_if_label(num_instr, name, byte_offset, myTable);
        if(result == 0){/*1. STR is not a label (does not end in ':').*/
            instruction_name=name;
            /*instruction_name = (char*)malloc((strlen(name)+1) *sizeof(char) );
            memcpy(instruction_name,name,(strlen(name)+1) *sizeof(char));*/
            /*args*/
            num_args = 0;

            name=strtok(NULL, IGNORE_CHARS);
            /*while*/
            while(name){
                num_args++;
                if(num_args>3){
                    raise_extra_argument_error(num_instr,name);/**/
                    break;
                }
                /*update name*/
                /*name = hexToDec(name);*/
                args[num_args-1] = name;/*Convert to decimal*/
                name=strtok(NULL, IGNORE_CHARS);
            }
            /*Do not write that instruction to the output file (eg. don't call write_pass_one())*/
            if(num_args<=3){
                lines_written = write_pass_one(inter, instruction_name, args, num_args);/**/
                /*error*/
                if (lines_written == 0) {/**/
                    raise_instruction_error(num_instr, instruction_name, args, num_args);/**/
                    myreturn_value = -1;/**/
                } 
            }else{/*num_args>3*/
                myreturn_value=-1;
            }
            
            byte_offset += lines_written * 4; 
            lines_written = 0;
            /*else condition*/
        }else if(result == -1){
            myreturn_value=-1;
            continue;
            /*
            name = strtok(NULL, IGNORE_CHARS);
            if(name == NULL){
                continue;
            }else{
                
                instruction_name=name;
                
                num_args=0;
                name=strtok(NULL, IGNORE_CHARS);
                
                while(name){
                    num_args++;
                    if(num_args>3){
                        raise_extra_argument_error(num_instr,name);
                        break;
                    }
                    
                    
                    args[num_args-1] = name;
                    name=strtok(NULL, IGNORE_CHARS);
                }
                
                if(num_args<=3){
                    lines_written = write_pass_one(inter, instruction_name, args, num_args);
                    
                    if (lines_written == 0) {
                        raise_instruction_error(num_instr, instruction_name, args, num_args);
                        myreturn_value = -1;
                    } 
                }else{
                    myreturn_value=-1;
                }
                
                byte_offset += lines_written * 4;
                

            }
            */
        }else if(result == 1){/*STR ends in ':' and is a valid label. Addition to symbol table succeeds.*/
            /*1. Only one label may be present per line. It must be the first token present.
        Once you see a label, regardless of whether it is a valid label or invalid
        label, treat the NEXT token as the beginning of an instruction.*/
            name = strtok(NULL, IGNORE_CHARS);
            if(name == NULL){/*There is nothing behind this invalid Labal*/
                continue;
            }else{/*Once you see a label, regardless of whether it is a valid label or invalid
                label, treat the NEXT token as the beginning of an instruction.*/
                instruction_name=name;
                /*instruction_name = (char*)malloc((strlen(name)+1) *sizeof(char) );
                memcpy(instruction_name,name,(strlen(name)+1) *sizeof(char));*/
                /*args*/
                num_args=0;
                name=strtok(NULL, IGNORE_CHARS);
                /*while*/
                while(name){
                    num_args++;/**/
                    if(num_args>3){
                        raise_extra_argument_error(num_instr,name);/**/
                        break;
                    }
                    /*update name*/
                    /*name = hexToDec(name);*/
                    args[num_args-1] = name;/*Convert to decimal*/
                    name=strtok(NULL, IGNORE_CHARS);
                }
                /*Do not write that instruction to the output file (eg. don't call write_pass_one())*/
                if(num_args<=3){
                    lines_written = write_pass_one(inter, instruction_name, args, num_args);
                    /*error*/
                    if (lines_written == 0) {
                        raise_instruction_error(num_instr, instruction_name, args, num_args);
                        myreturn_value = -1;
                    } 
                }else{/*num_args>3*/
                    myreturn_value=-1;
                }
                byte_offset += lines_written * 4;
                lines_written = 0;
            }
        }
    }
    /*end*/
    write_table(myTable,symtbl);
    free_table(myTable);
    return myreturn_value;
}

/* Second pass of the assembler.

   This function should read an intermediate file and the corresponding symbol table
   file, translates it into machine code. You may assume:
    1. The input file contains no comments
    2. The input file contains no labels
    3. The input file contains at maximum one instruction per line
    4. All instructions have at maximum MAX_ARGS arguments
    5. The symbol table file is well formatted
    6. The symbol table file contains all the symbol required for translation
   If an error is reached, DO NOT EXIT the function. Keep translating the rest of
   the document, and at the end, return -1. Return 0 if no errors were encountered. */
int pass_two(FILE *inter, FILE *symtbl, FILE *output) {
    /* A buffer for line parsing. */
    char buf[BUF_SIZE];
    /* Variables for argument parsing. */
    char *args[MAX_ARGS];
    char *temp_pch;
    char *name;

    /* Variables for counting. */
    int num_args;
    int return_value = 0;
    int num_instr = -1;

    /*create symbol table*/
    SymbolTable *myTable = create_table_from_file(1, symtbl);/*suppose mode =1*/
    /* error checking */
    if (!inter || !symtbl || !output)
        return -1;

    /* Write text segment. */
    fprintf(output, ".text\n");
    /*while*/
    while (fgets(buf, BUF_SIZE, inter)) {
        /*define variables*/
        name = strtok(buf, IGNORE_CHARS);
        num_instr++;
        temp_pch = strtok(NULL, IGNORE_CHARS);
        num_args = 0;
        /*error checking*/
        if (!name || !temp_pch)
            return_value = -1;
        /*while*/
        while (temp_pch) {
            args[num_args] = temp_pch;
            num_args++;
            temp_pch = strtok(NULL, IGNORE_CHARS);
        }
        /*inst wrong*/
        if (translate_inst(output, name, args, num_args, 4 * num_instr, myTable) == -1) {
            raise_instruction_error(num_instr + 1, name, args, num_args);
            return_value = -1;
        }
    }
    /* Write symbol segment. */
    fprintf(output, "\n.symbol\n");
    /* Write symbols here by calling `write_table()` */
    write_table(myTable, output);
    free_table(myTable);
    return return_value;
}

/*******************************
 * Do Not Modify Code Below
 *******************************/

static int open_files_pass_one(FILE **input, FILE **inter, FILE **symtbl,
                               const char *input_name, const char *inter_name, const char *symtbl_name) {
    /*some meaningful comments in given functions*/
    *input = fopen(input_name, "r");
    if (!*input) {
        write_to_log("Error: unable to open input file: %s\n", input_name);
        return -1;
    }
    /*some meaningful comments in given functions*/
    *inter = fopen(inter_name, "w");
    if (!*inter) {
        write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
        fclose(*input);
        /*some meaningful comments in given functions*/
        return -1;
    }
    *symtbl = fopen(symtbl_name, "w");
    if (!*symtbl) {
        /*some meaningful comments in given functions*/
        write_to_log("Error: unable to open symbol table file: %s\n", symtbl_name);
        fclose(*input);
        fclose(*inter);
        return -1;
    }
    /*some meaningful comments in given functions*/
    return 0;
}

static int open_files_pass_two(FILE **inter, FILE **symtbl, FILE **output,
                               const char *inter_name, const char *symtbl_name, const char *output_name) {

    /*some meaningful comments in given functions*/
    *inter = fopen(inter_name, "r");
    if (!*inter) {
        write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
        return -1;
    }
    /*some meaningful comments in given functions*/
    *symtbl = fopen(symtbl_name, "r");
    if (!*symtbl) {
        write_to_log("Error: unable to open symbol table file: %s\n", inter_name);
        fclose(*inter);
        /*some meaningful comments in given functions*/
        return -1;
    }
    *output = fopen(output_name, "w");
    if (!*output) {
        /*some meaningful comments in given functions*/
        write_to_log("Error: unable to open output file: %s\n", output_name);
        fclose(*inter);
        fclose(*symtbl);
        return -1;
    }
    /*some meaningful comments in given functions*/
    return 0;
}

/*some meaningful comments in given functions*/
static void close_files(FILE *file1, FILE *file2, FILE *file3) {
    fclose(file1);
    fclose(file2);
    fclose(file3);
}

/* Runs the two-pass assembler. Most of the actual work is done in pass_one()
   and pass_two().
 */
int assemble(const char *in, const char *out, const char *int_, const char *sym) {
    FILE *input, *inter, *symtbl, *output;
    int err = 0;

    /*some meaningful comments in given functions*/
    if (in) {
        /*some meaningful comments in given functions*/
        printf("Running pass one: %s -> %s, %s\n", in, int_, sym);
        if (open_files_pass_one(&input, &inter, &symtbl, in, int_, sym) != 0) {
            exit(1);
        }
        /*some meaningful comments in given functions*/

        if (pass_one(input, inter, symtbl) != 0) {
            err = 1;
        }
        close_files(input, inter, symtbl);
    }
    /*some meaningful comments in given functions*/

    if (out) {
        /*some meaningful comments in given functions*/
        printf("Running pass two: %s, %s -> %s\n", int_, sym, out);
        if (open_files_pass_two(&inter, &symtbl, &output, int_, sym, out) != 0) {
            exit(1);
        }

        /*some meaningful comments in given functions*/
        if (pass_two(inter, symtbl, output) != 0) {
            err = 1;
        }

        close_files(inter, symtbl, output);
    }

    /*some meaningful comments in given functions*/
    return err;
}

/*some meaningful comments in given functions*/
static void print_usage_and_exit() {
    /*some meaningful comments in given functions*/
    printf("Usage:\n");
    printf("  Runs both passes: assembler <input file> <intermediate file> <symbol table file> <output file>\n");
    printf("  Run pass #1:      assembler -p1 <input file> <intermediate file> <symbol table file>\n");
/*some meaningful comments in given functions*/
    printf("  Run pass #2:      assembler -p2 <intermediate file> <symbol table> <output file>\n");
    printf("Append -log <file name> after any option to save log files to a text file.\n");
    exit(0);
}
/*some meaningful comments in given functions*/
int main(int argc, char **argv) {
    int mode = 0;
    char *input, *inter, *output, *symtbl;
    int err;
/*some meaningful comments in given functions*/
    if (argc != 5 && argc != 7) {
        print_usage_and_exit();
    }
/*some meaningful comments in given functions*/
    if (strcmp(argv[1], "-p1") == 0) {
        mode = 1;
/*some meaningful comments in given functions*/
    } else if (strcmp(argv[1], "-p2") == 0) {
        mode = 2;
    }
/*some meaningful comments in given functions*/
    if (mode == 1) {
        input = argv[2];
        inter = argv[3];
/*some meaningful comments in given functions*/
        symtbl = argv[4];
        output = NULL;
    } else if (mode == 2) {
/*some meaningful comments in given functions*/
        input = NULL;
        inter = argv[2];
/*some meaningful comments in given functions*/
        symtbl = argv[3];
        output = argv[4];
    } else {
        input = argv[1];
/*some meaningful comments in given functions*/
        inter = argv[2];
        symtbl = argv[3];
/*some meaningful comments in given functions*/
        output = argv[4];
    }

/*some meaningful comments in given functions*/
    if (argc == 7) {
        if (strcmp(argv[5], "-log") == 0) {
/*some meaningful comments in given functions*/
            set_log_file(argv[6]);
        } else {
            print_usage_and_exit();
        }
    }
/*some meaningful comments in given functions*/

    err = assemble(input, output, inter, symtbl);

    if (err) {
/*some meaningful comments in given functions*/
        write_to_log("One or more errors encountered during assembly operation.\n");
    } else {
/*some meaningful comments in given functions*/
        write_to_log("Assembly operation completed successfully!\n");
    }
/*some meaningful comments in given functions*/

    if (is_log_file_set()) {
        printf("Results saved to %s\n", argv[5]);
    }

    return err;
}
