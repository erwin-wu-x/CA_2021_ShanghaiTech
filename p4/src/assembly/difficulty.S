.data
    
    # press0: .string "Press one botton to turn left"
    # press1: .string "Press the other botton to turn right"
    # eat: .string "Eat"
    # lengthen: .string "to lengthen"
    # shorten: .string "to shorten"
    # poison: .string "to die"
    easy: .string "Easy"
    normal: .string "Normal"
    hard: .string "Hard"
    hell: .string "Hell"
    
.section .text
.globl difficulty
.type difficulty, @function
difficulty:
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



    # String "Easy: "
    la s1, easy
    li a0, 25 #x
    li a1, 17 #y
    mv a2, s1
    li a3, 0xFF2600
    jal LCD_ShowString 

    # String "Normal: "
    la s1, normal
    li a0, 85 #x
    li a1, 17 #y
    mv a2, s1
    li a3, 0x3700FF
    jal LCD_ShowString 

    # String "Hard: "
    la s1, hard
    li a0, 25 #x
    li a1, 45 #y
    mv a2, s1
    li a3, 0x00EAFF
    jal LCD_ShowString

    # String "Hell: "
    la s1, hell
    li a0, 85 #x
    li a1, 45 #y
    mv a2, s1
    li a3, 0b1111100000000000
    jal LCD_ShowString

    # # Icon "food0: "
    # li a0, 40 #x
    # li a1, 20 #y
    # jal food0

    # # Icon "food1: "
    # li a0, 40 #x
    # li a1, 40 #y
    # jal food1

    # # Icon "food2: "
    # li a0, 40 #x
    # li a1, 60 #y
    # jal food2

    # # lenthen
    # la s1, lengthen
    # li a0, 60 #x
    # li a1, 10 #y
    # mv a2, s1
    # li a3, 0x3700FF  
    # jal LCD_ShowString

    # # shorten
    # la s1, shorten
    # li a0, 60 #x
    # li a1, 30 #y
    # mv a2, s1
    # li a3, 0b1111100000000000  
    # jal LCD_ShowString

    # # poison
    # la s1, poison
    # li a0, 60 #x
    # li a1, 50 #y
    # mv a2, s1
    # li a3, 0xFF2600  
    # jal LCD_ShowString








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
    


