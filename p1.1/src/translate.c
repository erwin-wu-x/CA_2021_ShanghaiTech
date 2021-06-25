/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* Writes instruction during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and mv pseudoinstructions. Your pseudoinstruction
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addi instruction. Otherwise, expand it into
        a lui-addi pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   venus has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if venus behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */

static unsigned
get_bit(unsigned x, unsigned n) {
    /*
       Returning -1 is a placeholder (it makes
       no sense, because get_bit only returns 
       0 or 1) */
    x = x >> n;/**/
    x = x & 1;
    return x;
    /*return -1;*/
}

unsigned write_pass_one(FILE *output, const char *name, char **args, int num_args) {
    /*NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.*/
    signed long int long_imm=0;
    int imm;/**/

    if (!output || !name || !args)/**/
        return 0;/**/

    /*on 2021.3.22  xuyt1 modified*/
    if(strcmp(name, "li") == 0){
        if (num_args != 2) {
            return 0; /**/
        }
        long_imm = strtol(args[1], NULL, 0);
        if(long_imm > 4294967295){
            return 0;
        }
        /*!!!!目前我只考虑16进制的情况;*/
        if(args[1][0]=='0' && (args[1][1]=='X' || args[1][1]=='x')){
            imm = strtol(args[1], NULL, 16);
            
            if(imm>=-2048 && imm<=2047){/*output the immediate as is*/
                fprintf(output, "%s ", "addi");/**/
                fprintf(output, "%s ", args[0]); /**/
                fprintf(output, "%s ", "x0");/**/
                fprintf(output, "%s\n", args[1]);/**/
                return 1;/**/
            }else{
                char buffer[20]={0};/**/
                char new_arg_1[20]={0};/**/
                unsigned upper_20_bits=0;/**/
                int lower_12_bits=0;/**/
                int lower_12_bits_len=0;
                int youyi=0;
                int i=0;

                sprintf(new_arg_1+2,"%X",imm);/**/
                new_arg_1[0]='0';/**/
                new_arg_1[1]='x';/**/
                lower_12_bits_len = strlen(new_arg_1);/**/
                youyi = 10 - lower_12_bits_len;/**/

                for(i=lower_12_bits_len-1;i>=2;--i){/**/
                    new_arg_1[i+youyi] = new_arg_1[i];/**/
                }/**/

                for(i=2;i<2+youyi;++i){/**/
                    new_arg_1[i]='0';/**/
                }


                strcpy(buffer,new_arg_1);/**/
                buffer[7]='\0';/**/
                upper_20_bits = strtol(buffer, NULL, 16); /**/
                if (get_bit(imm, 11) == 1){
                    upper_20_bits += 1;/**/
                    strcpy(buffer,new_arg_1);/**/
                    buffer[2] = 'F';/**/
                    buffer[3] = 'F';/**/
                    buffer[4] = 'F';/**/
                    buffer[5] = 'F';/**/
                    buffer[6] = 'F';/**/
                    lower_12_bits = strtol(buffer, NULL, 16); /**/
                }else{
                    strcpy(buffer,new_arg_1);/**/
                    buffer[2] = '0';/**/
                    buffer[3] = '0';/**/
                    buffer[4] = '0';/**/
                    buffer[5] = '0';/**/
                    buffer[6] = '0';/**/
                    lower_12_bits = strtol(buffer, NULL, 16); /**/
                }
                fprintf(output, "%s ", "lui");/**/
                fprintf(output, "%s ", args[0]);/**/
                fprintf(output, "%d\n", upper_20_bits);/**/
                
                fprintf(output, "%s ", "addi");/**/
                fprintf(output, "%s ", args[0]);/**/
                fprintf(output, "%s ", args[0]);/**/
                fprintf(output, "%d\n", lower_12_bits);/**/
                return 2;
            }
        }else{/*dec*/
            /* unsigned unsigned_imm = strtol(args[1], NULL, 10); */
            char buffer[20]={0};/**/
            char new_arg_1[20]={0};/**/
            unsigned upper_20_bits=0;/**/
            int lower_12_bits=0;/**/
            int lower_12_bits_len=0;/**/
            int youyi=0;/**/
            int i=0;/**/
            imm = strtol(args[1], NULL, 10);/**/
            /* if(unsigned_imm > 2047){
                goto Ugly;
            } */
            
            
            if(imm>=-2048 && imm<=2047){/*output the immediate as is*/
                fprintf(output, "%s ", "addi");/**/
                fprintf(output, "%s ", args[0]); /**/
                fprintf(output, "%s ", "x0");/**/
                /* fprintf(output, "%s\n", args[1]); *//*我们改动了这里！！！！！！*/
                fprintf(output, "%d\n", imm);
                return 1;/**/
            }else{                
                /* Ugly:; */
                
                

                sprintf(new_arg_1+2,"%X",imm);/**/
                new_arg_1[0]='0';/**/
                new_arg_1[1]='x';/**/
                lower_12_bits_len = strlen(new_arg_1);/**/
                youyi = 10 - lower_12_bits_len;/**/

                for(i=lower_12_bits_len-1;i>=2;--i){
                    new_arg_1[i+youyi] = new_arg_1[i];/**/
                }

                for(i=2;i<2+youyi;++i){
                    new_arg_1[i]='0';/**/
                }

                strcpy(buffer,new_arg_1);/**/
                buffer[7]='\0';/**/
                upper_20_bits = strtol(buffer, NULL, 16); /**/
                if (get_bit(imm, 11) == 1){
                    upper_20_bits += 1;/**/
                    strcpy(buffer,new_arg_1);/**/
                    buffer[2] = 'F';/**/
                    buffer[3] = 'F';/**/
                    buffer[4] = 'F';/**/
                    buffer[5] = 'F';/**/
                    buffer[6] = 'F';/**/
                    lower_12_bits = strtol(buffer, NULL, 16); /**/
                }else{
                    strcpy(buffer,new_arg_1);/**/
                    buffer[2] = '0';/**/
                    buffer[3] = '0';/**/
                    buffer[4] = '0';/**/
                    buffer[5] = '0';/**/
                    buffer[6] = '0';/**/
                    lower_12_bits = strtol(buffer, NULL, 16); /**/
                }
                fprintf(output, "%s ", "lui");/**/
                fprintf(output, "%s ", args[0]);/**/
                fprintf(output, "%d\n", upper_20_bits);/**/
                
                fprintf(output, "%s ", "addi");/**/
                fprintf(output, "%s ", args[0]);/**/
                fprintf(output, "%s ", args[0]);/**/
                fprintf(output, "%d\n", lower_12_bits);/**/
                return 2;
            }
        }
        
    }

    
    /*on 2021.3.22 wushx modified*/
    /*
    if (strcmp(name, "li") == 0) {
        
        if (num_args != 2) {
            return 0; 
        }
        imm = strtol(args[1], NULL, 0);
        if (imm < 0 || imm > 4294967295)
            return 0;
        if (((imm >> 31) & 1) == 1)
            imm = -((0xFFFFFFFF - imm) + 1);

        imm = strtol(args[1], NULL, 0);
        if (imm < 0 || imm > 4294967295) 
            return 0;
        if (((imm >> 31) & 1) == 1)
            imm = -((0xFFFFFFFF - imm) + 1);
        if (imm >= -2048 && imm <= 2047) {
            fprintf(output, "%s ", "addi");
            fprintf(output, "%s ", args[0]); 
            fprintf(output, "%s ", "x0");
            fprintf(output, "%s\n", args[1]);
            return 1;
        } else {
            int upper_20_bits = imm >> 12; 
            int lower_12_bits = 0;
            char buffer[20] = {0};
            lower_12_bits = imm & 0x00000FFF;
            if (get_bit(lower_12_bits, 11) == 1) {
                fprintf(output, "%s ", "lui");
                fprintf(output, "%s ", args[0]);
                upper_20_bits += 1;
                lower_12_bits = imm - (1 << 12);
                
                sprintf(buffer, "%d", upper_20_bits);
                fprintf(output, "%s\n", buffer);

            } else {
                fprintf(output, "%s ", "lui");
                fprintf(output, "%s ", args[0]);

                
                sprintf(buffer, "%d", upper_20_bits);
                fprintf(output, "%s\n", buffer);

            }
            fprintf(output, "%s ", "addi");
            fprintf(output, "%s ", args[0]);
            fprintf(output, "%s ", args[0]);
            
            sprintf(buffer, "%d", lower_12_bits);
            fprintf(output, "%s\n", buffer);
            return 2;
        }

        */

        /*before 2021.3.22*/
        /*
        imm = atoi(args[1]);

        if (imm > 4294967295 || imm < -2147483648)
        {             
            return 0; 
        } else if (imm < 4096 || imm >= -4096) {
            fprintf(output, "%s ", "addi");
            fprintf(output, "%s ", args[0]); 
            
            fprintf(output, "%s ", "x0");
            fprintf(output, "%s\n", args[1]);
            return 1;
        } else {
            int upper_20_bits = imm >> 12; 
            int lower_12_bits;
            int aa;
            fprintf(output, "%s ", "lui");
            fprintf(output, "%s ", args[0]);
            fprintf(output, "%d\n", upper_20_bits);
            lower_12_bits = imm & 0x00000FFF;
            if (get_bit(lower_12_bits, 11) == 1) {
                fprintf(output, "%s ", "lui");
                aa = atoi(args[0]);
                aa -= 1;
                
                fprintf(output, "%d ", aa);
                fprintf(output, "%d\n", upper_20_bits);
            } else {
                
                fprintf(output, "%s ", "lui");
                fprintf(output, "%s ", args[0]);
                fprintf(output, "%d\n", upper_20_bits);
            }


            
            fprintf(output, "%s ", "addi");
            fprintf(output, "%s ", args[0]);
            fprintf(output, "%s ", args[0]);
            fprintf(output, "%d\n", lower_12_bits);
            
            return 2;
        }*/

        /* YOUR CODE HERE */
    else if (strcmp(name, "bgez") == 0) {
        if (num_args != 2) {
            return 0; /*the number of args(num_args) error*/
        }
        fprintf(output, "%s ", "bge");
        fprintf(output, "%s ", args[0]);
        /*fprintf(output, "%s ", "x0,");*/
        fprintf(output, "%s ", "x0");
        fprintf(output, "%s\n", args[1]);
        return 1;
        /* YOUR CODE HERE */
    } else if (strcmp(name, "beqz") == 0) {
        if (num_args != 2) {
            return 0; /*the number of args(num_args) error*/
        }
        fprintf(output, "%s ", "beq");
        fprintf(output, "%s ", args[0]);
        /*fprintf(output, "%s ", "x0,");*/
        fprintf(output, "%s ", "x0");
        fprintf(output, "%s\n", args[1]);
        return 1;
        /* else  condition*/
    } else if (strcmp(name, "mv") == 0) {
        if (num_args != 2) {
            return 0; /*the number of args(num_args) error*/
        }
        fprintf(output, "%s ", "addi");/**/
        fprintf(output, "%s ", args[0]);
        fprintf(output, "%s ", args[1]);/**/
        fprintf(output, "0\n");
        return 1;
        /* else  condition*/
    } else if (strcmp(name, "jal") == 0) {
        if (num_args == 2) {
            write_inst_string(output, name, args, num_args);
            return 1;
            /* else  condition*/
        } else if (num_args == 1) {
            fprintf(output, "%s ", "jal");
            /*fprintf(output, "%s ", "ra,");*/
            fprintf(output, "%s ", "ra");
            fprintf(output, "%s\n", args[0]);
            return 1;
            /* else  condition*/
        } else {
            return 0;
        }
        /* else  condition*/
    } else if (strcmp(name, "jalr") == 0) {
        if (num_args == 3) {
            write_inst_string(output, name, args, num_args);
            return 1;
            /* else  condition*/
        } else if (num_args == 2) {
            return 0; /*wrong number of args*/
            /* else  condition*/
        } else if (num_args == 1) {
            fprintf(output, "%s ", "jal");
            /*fprint*/
            fprintf(output, "%s ", "ra"); /*bug*/
            fprintf(output, "%s ", args[0]);
            fprintf(output, "%s\n", "0");
            /* else  condition*/
        } else {
            return 0;
        }
        /* else  condition*/
    } else if (strcmp(name, "j") == 0) {
        if (num_args != 1) {
            return 0; /*the number of args(num_args) error*/
        }
        fprintf(output, "%s ", "jal");
        /*fprintf(output, "%s ", "zero,");*/
        fprintf(output, "%s ", "zero");
        fprintf(output, "%s\n", args[0]);
        return 1;
        /* else  condition*/
    } else if (strcmp(name, "jr") == 0) {
        if (num_args != 1) {
            return 0; /*the number of args(num_args) error*/
        }

        fprintf(output, "%s ", "jalr");
        /*fprintf(output, "%s ", "zero,");*/
        fprintf(output, "%s ", "zero");
        fprintf(output, "%s ", args[0]);
        fprintf(output, "%s\n", "0");
        /*return*/
        return 1;/**/
    } else {/**/
        /* else  condition*/
        write_inst_string(output, name, args, num_args);/**/
        return 1;
    }
    return 0;
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step.

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write
   anything to OUTPUT but simply return -1. venus may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error.
 */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args, uint32_t addr, SymbolTable *symtbl) {
    /* r type*/
    if (strcmp(name, "add") == 0)/*add*/
        return write_rtype(0x33, 0x00, 0x00, output, args, num_args);
    else if (strcmp(name, "or") == 0)
        return write_rtype(0x33, 0x06, 0x00, output, args, num_args);
    else if (strcmp(name, "sll") == 0)/*sll*/
        return write_rtype(0x33, 0x01, 0x00, output, args, num_args);
    else if (strcmp(name, "slt") == 0)
        return write_rtype(0x33, 0x02, 0x00, output, args, num_args);
    else if (strcmp(name, "sltu") == 0)/*sltu*/
        return write_rtype(0x33, 0x03, 0x00, output, args, num_args);
        /* i type*/
    else if (strcmp(name, "addi") == 0)
        return write_itype(0x13, 0x00, output, args, num_args);
    else if (strcmp(name, "ori") == 0)/*ori*/
        return write_itype(0x13, 0x06, output, args, num_args);
    else if (strcmp(name, "lb") == 0)
        return write_itype(0x03, 0x00, output, args, num_args);
    else if (strcmp(name, "lbu") == 0)/*lbu*/
        return write_itype(0x03, 0x04, output, args, num_args);
    else if (strcmp(name, "lw") == 0)
        return write_itype(0x03, 0x02, output, args, num_args);
    else if (strcmp(name, "jalr") == 0)/*lbu*/
        return write_itype(0x67, 0x00, output, args, num_args);
        /* s type*/
    else if (strcmp(name, "sb") == 0)
        return write_stype(0x23, 0x00, output, args, num_args);
    else if (strcmp(name, "sw") == 0)
        return write_stype(0x23, 0x02, output, args, num_args);
        /* u type*/
    else if (strcmp(name, "lui") == 0)
        return write_utype(0x37, output, args, num_args);
        /* b type*/
    else if (strcmp(name, "beq") == 0)
        return write_sbtype(0x63, 0x00, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bne") == 0)
        return write_sbtype(0x63, 0x01, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "blt") == 0)/*blt*/
        return write_sbtype(0x63, 0x04, output, args, num_args, addr, symtbl);
    else if (strcmp(name, "bge") == 0)
        return write_sbtype(0x63, 0x05, output, args, num_args, addr, symtbl);
        /* j type*/
    else if (strcmp(name, "jal") == 0)
        return write_ujtype(0x6F, output, args, num_args, addr, symtbl);
    else
        return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE *output, char **args, size_t num_args) {
    /* init variables */
    int rd, rs, rt;
    uint32_t instruction = 0;
    /* some error checking */
    if (num_args != 3)
        return -1;
    if (!args[0] && args[1] && args[2])
        return -1;
    /* init variables */
    rd = translate_reg(args[0]);
    rs = translate_reg(args[1]);
    rt = translate_reg(args[2]);

    /* error checking for register ids */
    if (rd == -1 || rt == -1 || rs == -1)
        return -1;
    /* generate instruction */
    instruction = opcode + (rd << 7) + (funct3 << 12) + (rs << 15) + (rt << 20) + (funct7 << 25);
    write_inst_hex(output, instruction);
    return 0;
}

/*i type*/
int write_itype(uint8_t opcode, uint8_t funct3, FILE *output, char **args, size_t num_args) {
    /* init variables */
    int rd, rs, immCheck;
    long imm = 12;
    uint32_t instruction = 0;
    /* some error checking */
    if (num_args != 3)
        return -1;
    if (!args[0] && args[1] && args[2])
        return -1;
/* init variables */
    rd = translate_reg(args[0]);
    rs = translate_reg(args[1]);
    /*imm in wrong position check*/
    if (rs == -1) {
        rs = translate_reg(args[2]);
        immCheck = translate_num(&imm, args[1], -(1 << 11), (1 << 11) - 1);
        /*else condition*/
    } else
        immCheck = translate_num(&imm, args[2], -(1 << 11), (1 << 11) - 1);

    /* error checking for register ids */
    if (rd == -1 || immCheck == -1 || rs == -1)
        return -1;
    /* generate instruction */
    instruction = opcode + (rd << 7) + (funct3 << 12) + (rs << 15) + (imm << 20);
    write_inst_hex(output, instruction);
    return 0;
}

/*s type*/
int write_stype(uint8_t opcode, uint8_t funct3, FILE *output, char **args, size_t num_args) {
    /* init variables */
    int rs1, rs2, immCheck;
    long imm = 12;
    uint32_t instruction = 0;
    /* some error checking */
    if (num_args != 3)
        return -1;
    if (!args[0] && args[1] && args[2])
        return -1;
/* init variables */
    rs1 = translate_reg(args[0]);
    rs2 = translate_reg(args[1]);
    /*imm in wrong position check*/
    if (rs2 == -1) {
        rs2 = translate_reg(args[2]);
        immCheck = translate_num(&imm, args[1], -(1 << 11), (1 << 11) - 1);
        /*else condition*/
    } else
        immCheck = translate_num(&imm, args[2], -(1 << 11), (1 << 11) - 1);

    /* error checking for register ids */
    if (rs1 == -1 || immCheck == -1 || rs2 == -1)
        return -1;
    /* generate instruction */
    instruction =
            opcode + ((imm & ((1 << 5) - 1)) << 7) + (funct3 << 12) + (rs2 << 15) + (rs1 << 20) + ((imm >> 5) << 25);
    write_inst_hex(output, instruction);
    return 0;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne rs rt label
   assume the byte_offset(addr) of label is L,
   current instruction byte_offset(addr) is A
   the relative address I for label satisfy:
     L = A + I
   so the relative address is
     I = L - A              */
int write_sbtype(uint8_t opcode, uint8_t funct3, FILE *output, char **args, size_t num_args,
                 uint32_t addr, SymbolTable *symtbl) {
    /* init variables */
    int rs1, rs2, L;
    uint32_t instruction = 0;
    uint32_t offset = 0;
    /* some error checking */
    if (num_args != 3)
        return -1;
    if (!args[0] && args[1] && args[2])
        return -1;
    /* init variables */
    rs1 = translate_reg(args[0]);
    rs2 = translate_reg(args[1]);
    L = get_addr_for_symbol(symtbl, args[2]);

    /* error checking for register ids */
    if (rs1 == -1 || L == -1 || rs2 == -1)
        return -1;
    /* generate instruction */
    offset = (L - addr);/*may (L-addr-4)/4*/
    instruction =
            opcode + (((offset >> 11) & 1) << 7) + (((offset >> 1) & ((1 << 4) - 1)) << 8) + (funct3 << 12) +
            (rs1 << 15) + (rs2 << 20) + (((offset >> 5) & ((1 << 6) - 1)) << 25) + (((offset >> 12) & 1) << 31);
    write_inst_hex(output, instruction);
    /*return*/
    return 0;
}

int write_utype(uint8_t opcode, FILE *output, char **args, size_t num_args) {
    /* init variables */
    int rd, immCheck;
    long imm = 20;
    uint32_t instruction = 0;
    /* some error checking */
    if (num_args != 2)
        return -1;
    if (!args[0] && args[1])
        return -1;
    /* init variables */
    rd = translate_reg(args[0]);
    immCheck = translate_num(&imm, args[1], -(1 << 19), (1 << 19) - 1);

    /* error checking for register ids */
    if (rd == -1 || immCheck == -1)
        return -1;
    /* generate instructions */
    instruction = opcode + (rd << 7) + (imm << 12);
    write_inst_hex(output, instruction);
    return 0;
}

/* In this project there is no need to relocate labels,
   you may think about the reasons. */
int write_ujtype(uint8_t opcode, FILE *output, char **args, size_t num_args,
                 uint32_t addr, SymbolTable *symtbl) {
    /* init variables */
    int rd, L;
    uint32_t instruction = 0;
    uint32_t offset = 0;
    /* some error checking */
    if (num_args != 2)
        return -1;
    if (!args[0] && args[1])
        return -1;
    /* init variables */
    rd = translate_reg(args[0]);
    L = get_addr_for_symbol(symtbl, args[1]);

    /* error checking for register ids */
    if (rd == -1 || L == -1)
        return -1;
    /* generate instruction */
    offset = (L - addr);/*may (L-addr-4)/4*/
    instruction = opcode + (rd << 7) + (((offset >> 12) & ((1 << 8) - 1)) << 12) + (((offset >> 11) & 1) << 20) +
                  (((offset >> 1) & ((1 << 10) - 1)) << 21) + (((offset >> 20) & 1) << 31);
    write_inst_hex(output, instruction);
    /*return*/
    return 0;
}