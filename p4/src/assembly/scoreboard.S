.data
    finish: .string "SCORE: "
    Retry: .string "Retry"
    Quit: .string "Quit"
.section .text
.globl scoreboard
.type scoreboard, @function
scoreboard:
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
    mv s0, a0 #s0: Total score

    # Set blackground color
    li a0, 0 # 0x4F0D5C #blackground color: Black
    # li a0, 0
    jal LCD_Clear

    # String "Score: "
    la s1, finish
    li a0, 40 # x
    li a1, 15 # y
    mv a2, s1
    li a3, 0x3700FF  
    jal LCD_ShowString

    # Score(A num)
    li a0, 90      
    li a1, 15       
    mv a2, s0
    li a3, 3
    li a4, 0xf800
    jal LCD_ShowNum

    # Retry

    la s1, Retry
    li a0, 33 # x
    li a1, 40 # y
    mv a2, s1
    li a3, 0x4F0D5C  
    jal LCD_ShowString

    # Quit

    la s1, Quit
    li a0, 100 # x
    li a1, 40 # y
    mv a2, s1
    li a3, 0xFF2600
    jal LCD_ShowString


    # frame
    li a0, 2        
    li a1, 2        
    li a2, 157      
    li a3, 77       
    li a4, 0xFFFE   
    jal LCD_DrawRectangle

    li a0, 6        
    li a1, 6        
    li a2, 153      
    li a3, 73       
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
    