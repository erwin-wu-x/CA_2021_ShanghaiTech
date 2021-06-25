# food.S
# a0: x
# a1: y

.data
    eat: .string "Eat" # Only to fill the space
.section .text
.globl food0
.type food0, @function

food0:
    addi sp, sp, -52
    # Save S-reg
    sw s0, 0(sp) # save reg
    sw s1, 4(sp) # save reg
    sw s2, 8(sp) # save reg
    sw s3, 12(sp) # save reg
    sw s4, 16(sp) # save reg
    sw s5, 20(sp) # save reg
    sw s6, 24(sp) # save reg
    sw s7, 28(sp) # save reg
    sw s8, 32(sp) # save reg
    sw s9, 36(sp) # save reg
    sw s10, 40(sp) # save reg
    sw s11, 44(sp) # save reg
    sw ra, 48(sp) # save reg
    # #######################################
    
    mv s0, a0 #s0 x
    mv s1, a1 #s1 y

    li a2, 2
    li a3, 0xFF2600
    jal LCD_DrawCircle
    
    # #######################################
    # Load S-reg
    lw s0, 0(sp) # load reg
    lw s1, 4(sp) # load reg
    lw s2, 8(sp) # load reg
    lw s3, 12(sp) # load reg
    lw s4, 16(sp) # load reg
    lw s5, 20(sp) # load reg
    lw s6, 24(sp) # load reg
    lw s7, 28(sp) # load reg
    lw s8, 32(sp) # load reg
    lw s9, 36(sp) # load reg
    lw s10, 40(sp) # load reg
    lw s11, 44(sp) # load reg
    lw ra, 48(sp) # load reg
    addi sp, sp, 52
    jr ra

