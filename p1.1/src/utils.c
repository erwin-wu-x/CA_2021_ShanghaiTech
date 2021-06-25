/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang ands Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

/*******************************
 * Do Not Modify Code Below 
 *******************************/

static const char* output_file = NULL;

int is_log_file_set() {
    return output_file != NULL;
}
/*set*/
void set_log_file(const char* filename) {
    if (filename) {
        output_file = filename;
        unlink(filename);
     /*else condition*/
    } else {
        output_file = NULL;
    }
}
/*log*/
void write_to_log(char* fmt, ...) {
    va_list args;

    if (output_file) {
        FILE* f = fopen(output_file, "a");
        /*error*/
        if (!f) {
            return;
        }
        /*va*/
        va_start(args, fmt);
        vfprintf(f, fmt, args);
        va_end(args);
        fclose(f);
        /*elsecondition*/
    } else {
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
}
/*inst*/
void log_inst(const char* name, char** args, int num_args) {
    int i;

    if (output_file) {
        FILE* f = fopen(output_file, "a");
        /*error*/
        if (!f) {
            return;
        }
        /*fprintf*/
        fprintf(f, "%s", name);
        for (i = 0; i < num_args; i++) {
            fprintf(f, " %s", args[i]);
        }
        /*fprintf*/
        fprintf(f, "\n");
        fclose(f);
    } else {
        /*fprintf*/
        fprintf(stderr, "%s", name);
        for (i = 0; i < num_args; i++) {
            fprintf(stderr, " %s", args[i]);
        }
        fprintf(stderr, "\n");
    }
}
