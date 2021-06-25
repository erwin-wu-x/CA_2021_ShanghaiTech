/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;



/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {/*addr_alignment_incorrect*/
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {/*name_already_exists*/
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_sym(FILE* output, uint32_t addr, const char* name) {/*write_sym*/
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    /* YOUR CODE HERE */
    SymbolTable* my_SymbolTable = malloc(sizeof(SymbolTable));
    if(my_SymbolTable == NULL){/*judge the my_SymbolTable*/
        allocation_failed();
    }
    my_SymbolTable->data = (Symbol*)malloc(sizeof(Symbol));
    if(my_SymbolTable->data == NULL){/*judge the data*/
        allocation_failed();
    }
    my_SymbolTable->len=0;
    my_SymbolTable->cap=1;/*inits*/
    my_SymbolTable->mode = mode;
    return my_SymbolTable;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    unsigned int i=0;/*init*/
    if(table == NULL){
        /*printf("freeSymbolTable error\n");*/
        return;/*judge NULL poiter*/
    }
    for(;i<table->len;++i){/*free name(string)*/
        free(table->data[i].name);
    }
    free(table->data);
    free(table);/*Notice !!!!whether I should free this table*/
    return;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
   1. ADDR is given as the byte offset from the first instruction.
   2. The SymbolTable must be able to resize itself as more elements are added.

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.OK

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect()
   and return -1.OK

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
   in the table, you should call name_already_exists() and return -1.OK

   6.If memory allocation fails, you should call allocation_failed().OK

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    if(addr%4!=0){/*If ADDR is not word-aligned, you should call addr_alignment_incorrect()and return -1*/
        addr_alignment_incorrect();
        return -1;
    }
    /* table null condition*/
    if(table == NULL){
        /*printf("push_back error\n");*/
        return -2;/*judge NUll poiter*/
    }
    if(table->mode == SYMBOLTBL_UNIQUE_NAME){/*5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already existsin the table, you should call name_already_exists() and return -1.*/
        if(get_addr_for_symbol(table,name) != -1){
            name_already_exists(name);
            return -1;
        }
    }


    if (table->len >= table->cap) {/*resize*/
        table->cap *= 2;
        table->data = realloc(table->data,(table->cap)*sizeof(Symbol));
        /* table null condition*/
        if(table->data == NULL){
            allocation_failed();
        }
    }
    /* table->data[table->len] = para;*/
    table->data[table->len].addr = addr;/*init*/
    table->data[table->len].name_len = strlen(name)+1;
    table->data[table->len].name = (char*)malloc(sizeof(char)*table->data[table->len].name_len);
    /*wrong condition*/
    if(table->data[table->len].name==NULL){
        allocation_failed();
    }
    strcpy(table->data[table->len].name,name);/*contribute space for name and copy*/
    ++table->len;
    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    /*init*/
    unsigned int i=0;
    unsigned int name_len;
    /*NULL condition*/
    if(table == NULL || name == NULL){
        /*printf("get_addr_for_symbol error");judge NULL*/
    }
    /*about name*/
    name_len=strlen(name)+1;
    for(;i<table->len;++i){
        if(table->data[i].name_len == name_len){
            /*strcmp*/
            if(strcmp(table->data[i].name,name)==0){/*two strings are equal*/
                return table->data[i].addr;/*found*/
            }
        }
    }
    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    unsigned int i=0;
    if(table==NULL || output==NULL){
        /*printf("write_table error");judge NULL poiter*/
    }
    
    for(;i<table->len;++i){
        write_sym(output,table->data[i].addr,table->data[i].name);
    }
}

/* Reads the symbol table file written by the `write_table` function, and
   restore the symbol table.
 */
SymbolTable* create_table_from_file(int mode, FILE* file) {
    /* YOUR CODE HERE */
    SymbolTable * mytable;
    char name[100];/*symbol longer than 100????*/
    uint32_t addr;
    if(file == NULL){
        /*printf("create_table_from_file error");*/
    }
    mytable = create_table(mode);
    while(1){/*read and write*/
        if(fscanf(file, "%u", &addr)==-1){/*EOF*/
            break;
        }
        fscanf(file, "%s",name);
        add_to_table(mytable,name,addr);
    }
    return mytable;
}
