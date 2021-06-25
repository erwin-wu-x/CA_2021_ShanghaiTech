
.data
    eat: .string "Eat" # Only to fill the space
.section .text
.globl otherchoice
.type otherchoice, @function
# main:
otherchoice:
    addi sp, sp, -52
    # Save S-reg
    sw s0, 0(sp) #save reg
    sw s1, 4(sp) #save reg
    sw s2, 8(sp) #save reg
    sw s3, 12(sp) #save reg
    sw s4, 16(sp) #save reg #save reg
    sw s5, 20(sp) #save reg #save reg
    sw s6, 24(sp) #save reg
    sw s7, 28(sp) #save reg #save reg
    sw s8, 32(sp) #save reg
    sw s9, 36(sp) #save reg
    sw s10, 40(sp) #save reg
    sw s11, 44(sp) #save reg
    sw ra, 48(sp) #save reg
    # #######################################
    
    mv s0, a0 # s0 left or right
    mv s1, a1 # s1 set or clear

    #########################
    # li s0,1
    # li s1,1
    #########################
    li s5, 0 # constant
    li s6, 1 # constant
    li s7, 2 # constant
    li s8, 3 # constant
    
    # li t1, 38 # y1
    # li t3, 60 # y2
    beq s0, s5, label_a0 # easy
    beq s0, s6, label_a1 # normal
    beq s0, s7, label_a2 # hard
    beq s0, s8, label_a3 # hell
label_c:
    beq s1, s5, label_d0 # judge set or clear
    beq s1, s6, label_d1 # judge set or clear
label_f:
    mv a0, t0 # set arg
    mv a1, t1 # set arg
    mv a2, t2 # set arg
    mv a3, t3 # set arg
    mv a4, t4 # set arg
    jal LCD_DrawRectangle # call func

end:
    # #######################################
    # Load S-reg
    lw s0, 0(sp) # load reg
    lw s1, 4(sp) # load reg
    lw s2, 8(sp) # load reg
    lw s3, 12(sp) # load reg
    lw s4, 16(sp) # load reg # load reg
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

# botton0_set:
#     # x: 31 - 81
#     # y: 20 - 60
#     li a0, 26 # x1
#     li a1, 38 # y1
#     li a2, 71 # x2
#     li a3, 60 # y2
#     li a4, 0xFFFF
    
#     jal LCD_DrawRectangle
#     j end
# botton0_clear:
#     li a0, 26 # x1
#     li a1, 38 # y1
#     li a2, 71 # x2
#     li a3, 60 # y2
#     li a4, 0x0
    
#     jal LCD_DrawRectangle
#     j end
# botton1_set:
#     # x: 31 - 81
#     # y: 20 - 60
#     li a0, 90 # x1
#     li a1, 38 # y1
#     li a2, 138# x2
#     li a3, 60 # y2
#     li a4, 0xFFFF 
    
#     jal LCD_DrawRectangle
        
#     j end
# botton1_clear:
#     li a0, 90 # x1
#     li a1, 38 # y1
#     li a2, 138# x2
#     li a3, 60 # y2
#     li a4, 0x0
    
#     jal LCD_DrawRectangle
#     j end

label_a0: # easy
    # li t0, 90
    # li t2, 138
    li t0, 21# x1
    li t1, 15# y1
    li t2, 63# x2
    li t3, 37# y2
    j label_c
label_a1: # normal
    li t0, 83# x1
    li t1, 15# y1
    li t2, 133# x2
    li t3, 37# y2
    j label_c
label_a2:
    li t0, 21# x1
    li t1, 43# y1
    li t2, 63# x2
    li t3, 63# y2
    j label_c
label_a3:
    # li t0, 26
    # li t2, 75
    li t0, 83# x1
    li t1, 43# y1
    li t2, 128# x2
    li t3, 63# y2
    j label_c

label_d0:
    li t4, 0x0000
    j label_f
label_d1:
    li t4, 0xFFFFFF
    j label_f
