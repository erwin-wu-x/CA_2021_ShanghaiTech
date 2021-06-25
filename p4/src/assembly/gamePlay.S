.data
    # finish: .string "SCORE: "
    press0: .string "Press one botton to turn left"
    press1: .string "Press the other botton to turn right"
    eat: .string "Eat"
    lengthen: .string "to lengthen"
    shorten: .string "to shorten"
    poison: .string "to die"
    
.section .text
.globl gamePlay
.type gamePlay, @function
gamePlay:
    addi sp, sp, -52
    # Save S-reg
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw s9, 36(sp)
    sw s10, 40(sp)
    sw s11, 44(sp)
    sw ra, 48(sp)
    # #######################################
    
    # Set blackground color
    li a0, 0 # 0x4F0D5C #blackground color: Black
    # li a0, 0
    jal LCD_Clear



    # frame
    li a0, 0        
    li a1, 0        
    li a2, 159      
    li a3, 79       
    li a4, 0xFFFE   
    jal LCD_DrawRectangle

    

    








    # #######################################
    # Load S-reg
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw s10, 40(sp)
    lw s11, 44(sp)
    lw ra, 48(sp)
    addi sp, sp, 52
    jr ra
    


