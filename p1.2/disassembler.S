
#                               Project 1.2 RISC-V disassembler

#   You can suppose you will get the number of machine codes you should disassemble from 
#   "lines_of_machine_codes" and get the 32 bits-machine code from "machine_codes". 
#   Both of them are stored in file "input.S"

#   When we test your project, we will change the input files content.

# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.data

# Constant integer specifying the lines of machine codes

# DO NOT MODIFY THIS VARIABLE
.globl lines_of_machine_codes
lines_of_machine_codes:
    .word 8


# 32-bits machine codes
# A 32-bits hexadecimal number represents one line of machine code.
# You can suppose all of the input machine codes are valid. 

# DO NOT MODIFY THIS VARIABLE
.globl machine_codes
machine_codes:
    .word 0x000502B3    # add  t0, a0, x0
    .word 0x00100313    # addi t1, x0, 1
    .word 0x00028863    # beq  t0, x0, 16
    .word 0x01DE13B3    # sll  t2, t3, t4
    .word 0xFFF28293    # addi t0, t0, -1
    .word 0xFF5FF06F    # jal  x0, -12
    .word 0x00600533    # add  a0, x0, t1
    .word 0x00008067    # jalr x0, ra, 0


.data
# You can define your own data such as ".word", ".asciiz", ".byte"... here.
# register
register_x0: .asciiz "x0" # x0
register_x1: .asciiz "ra" # ra
register_x2: .asciiz "sp" # sp
register_x3: .asciiz "gp" # gp
register_x4: .asciiz "tp" # tp
register_x5: .asciiz "t0" # t0
register_x6: .asciiz "t1" # t1
register_x7: .asciiz "t2" # t2
register_x8: .asciiz "s0" # s0
register_x9: .asciiz "s1" # s1
register_x10: .asciiz "a0" # a0
register_x11: .asciiz "a1" # a1
register_x12: .asciiz "a2" # a2
register_x13: .asciiz "a3" # a3
register_x14: .asciiz "a4" # a4
register_x15: .asciiz "a5" # a5
register_x16: .asciiz "a6" # a6
register_x17: .asciiz "a7" # a7
register_x18: .asciiz "s2" # s2
register_x19: .asciiz "s3" # s3
register_x20: .asciiz "s4" # s4
register_x21: .asciiz "s5" # s5
register_x22: .asciiz "s6" # s6
register_x23: .asciiz "s7" # s7
register_x24: .asciiz "s8" # s8
register_x25: .asciiz "s9" # s9
register_x26: .asciiz "s10" # 10
register_x27: .asciiz "s11" # 11
register_x28: .asciiz "t3" # t3
register_x29: .asciiz "t4" # t4
register_x30: .asciiz "t5" # t5
register_x31: .asciiz "t6" # t6

# instruction_name
# r type
instruction_add: .asciiz "add"
instruction_or: .asciiz "or"
instruction_slt: .asciiz "slt"
instruction_sltu: .asciiz "sltu"
instruction_sll: .asciiz "sll"
# i type(jalr)
instruction_jalr: .asciiz "jalr"
# i type(imm)
instruction_addi: .asciiz "addi"
instruction_ori: .asciiz "ori"
# i type(load)
instruction_lb: .asciiz "lb"
instruction_lbu: .asciiz "lbu"
instruction_lw: .asciiz "lw"
# s type
instruction_sb: .asciiz "sb"
instruction_sw: .asciiz "sw"
# b type
instruction_beq: .asciiz "beq"
instruction_bne: .asciiz "bne"
instruction_blt: .asciiz "blt"
instruction_bge: .asciiz "bge"
# j type
instruction_jal: .asciiz "jal"
# u type
instruction_lui: .asciiz "lui"

.text
# print functions
print:
	print_assist_functions: # whitespace, commas, newline, brackets.
		# print whitespace
		print_whitespace:
			li	a1, ' '		# whitespace
			li	a0, 11		# prepare for print ascll
			ecall
			ret
		# print commas
		print_commas:
			li	a1, ','		# commas
			li	a0, 11		# prepare for print ascll
			ecall
			ret
		# print newline
		print_newline:
			li	a1, '\n'	# newline
			li	a0, 11		# prepare for print ascll
			ecall
			ret
		# print left bracket
		print_left_bracket:
			li	a1, 40		# left bracket
			li	a0, 11		# prepare for print ascll
			ecall
			ret
		# print right bracket
		print_right_bracket:
			li	a1, ')'		# right bracket
			li	a0, 11		# prepare for print ascll
			ecall
			ret
	# print instruction in r type like: name rd rs1 rs2
	print_r:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s3 = the imm
		mv a1 s0
		li a0 4
		ecall # print "name"
		jal print_whitespace
		mv a1 s1
		li a0 4
		ecall # print the rd
		jal print_commas
		jal print_whitespace
		mv a1 s2
		li a0 4
		ecall # print the rs1
		jal print_commas
		jal print_whitespace
		mv a1 s3
		li a0 4
		ecall # print the rs2
		jal print_newline

		# ra from stack
		lw ra 0(sp)
		addi sp sp 4
		ret # return to main_while_loop
	# print instruction jal: jal rd, imm
	print_uj_u:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s3 = the imm
		mv a1 s0
		li a0 4
		ecall # print "jal"
		jal print_whitespace
		mv a1 s1
		li a0 4
		ecall # print the first register
		jal print_commas
		jal print_whitespace
		mv a1 s3
		li a0 1
		ecall # print the imm
		jal print_newline

		# ra from stack
		lw ra 0(sp)
		addi sp sp 4
		ret # return to main_while_loop
	# print instruction like lb, lbu or sw: lw x14, 8(x2)
	print_i_load_s:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s2 = the second register(rs1)
		# s3 = the imm
		mv a1 s0
		li a0 4
		ecall # print instruction name
		jal print_whitespace

		mv a1 s1
		li a0 4
		ecall # print the first register(rd)
		jal print_commas
		jal print_whitespace

		mv a1 s3
		li a0 1
		ecall # print the imm
		jal print_left_bracket

		mv a1 s2
		li a0 4
		ecall # print the second register(rs1)
		jal print_right_bracket
		jal print_newline

		# ra from stack
		lw ra 0(sp)
		addi sp sp 4
		ret # return to main_while_loop
	# print instruction like addi, jalr or beq: name rs rs imm
	print_i_normal_b: 
		# s0 = instruction name
		# s1 = the first register
		# s2 = the second register
		# s3 = the imm
		mv a1 s0
		li a0 4
		ecall # print instruction name
		jal print_whitespace
		mv a1 s1
		li a0 4
		ecall # print the first register
		jal print_commas
		jal print_whitespace
		mv a1 s2
		li a0 4
		ecall # print the second register
		jal print_commas
		jal print_whitespace
		mv a1 s3
		li a0 1
		ecall # print the imm
		jal print_newline

		# ra from stack
		lw ra 0(sp)
		addi sp sp 4
		ret # return to main_while_loop

assitant_functions: # translate_register, load_segment_with_position, return_0, trans_to_signed.
	# return 0 function 
	return_0:
		
		# DO NOT EDIT THIS FUNCTION
		add	   a1, x0, x0
		addi   a0, x0, 17
		ecall 
	# translateregister
	translate_register: # arguments: a0 = bin(register)
		# save name in a1
		register_0: # x0
			li t0 0
			bne a0 t0 register_1 # switch
			la a1 register_x0
		register_1: # x1
			li t0 1
			bne a0 t0 register_2 # switch
			la a1 register_x1
		register_2: # x2
			li t0 2
			bne a0 t0 register_3 # switch
			la a1 register_x2
		register_3: # x3
			li t0 3
			bne a0 t0 register_4 # switch
			la a1 register_x3
		register_4: # x4
			li t0 4
			bne a0 t0 register_5 # switch
			la a1 register_x4
		register_5: # x5
			li t0 5
			bne a0 t0 register_6 # switch
			la a1 register_x5
		register_6: # x6
			li t0 6
			bne a0 t0 register_7 # switch
			la a1 register_x6
		register_7: # x7
			li t0 7
			bne a0 t0 register_8 # switch
			la a1 register_x7
		register_8: # x8
			li t0 8
			bne a0 t0 register_9 # switch
			la a1 register_x8
		register_9: # x9
			li t0 9
			bne a0 t0 register_10 # switch
			la a1 register_x9
		register_10: # x10
			li t0 10
			bne a0 t0 register_11 # switch
			la a1 register_x10
		register_11: # x11
			li t0 11
			bne a0 t0 register_12 # switch
			la a1 register_x11
		register_12: # x12
			li t0 12
			bne a0 t0 register_13 # switch
			la a1 register_x12
		register_13: # x13
			li t0 13
			bne a0 t0 register_14 # switch
			la a1 register_x13
		register_14: # x14
			li t0 14
			bne a0 t0 register_15 # switch
			la a1 register_x14
		register_15: # x15
			li t0 15
			bne a0 t0 register_16 # switch
			la a1 register_x15
		register_16: # x16
			li t0 16
			bne a0 t0 register_17 # switch
			la a1 register_x16
		register_17: # x17
			li t0 17
			bne a0 t0 register_18 # switch
			la a1 register_x17
		register_18: # x18
			li t0 18
			bne a0 t0 register_19 # switch
			la a1 register_x18
		register_19: # x19
			li t0 19
			bne a0 t0 register_20 # switch
			la a1 register_x19
		register_20: # x20
			li t0 20
			bne a0 t0 register_21 # switch
			la a1 register_x20
		register_21: # x21
			li t0 21
			bne a0 t0 register_22 # switch
			la a1 register_x21
		register_22: # x22
			li t0 22
			bne a0 t0 register_23 # switch
			la a1 register_x22
		register_23: # x23
			li t0 23
			bne a0 t0 register_24 # switch
			la a1 register_x23
		register_24: # x24
			li t0 24
			bne a0 t0 register_25 # switch
			la a1 register_x24
		register_25: # x25
			li t0 25
			bne a0 t0 register_26 # switch
			la a1 register_x25
		register_26: # x26
			li t0 26
			bne a0 t0 register_27 # switch
			la a1 register_x26
		register_27: # x27
			li t0 27
			bne a0 t0 register_28 # switch
			la a1 register_x27
		register_28: # x28
			li t0 28
			bne a0 t0 register_29 # switch
			la a1 register_x28
		register_29: # x29
			li t0 29
			bne a0 t0 register_30 # switch
			la a1 register_x29
		register_30: # x30
			li t0 30
			bne a0 t0 register_31 # switch
			la a1 register_x30
		register_31: # x31
			li t0 31
			bne a0 t0 register_end
			la a1 register_x31
		register_end:
			ret # return
	# load segment with position
	load_segment_with_position: # arguments: a0 = instruction
		# arguments: a1 = length of segment, like len([4:0])=5
		# arguments: a2 = [:a2], like [4:0]
		# DO NOT EDIT THIS FUNCTION
		srl t0 a0 a2 # offset >> a2
		li t1 1
		sll t1 t1 a1 # 1 << a1
		addi t1 t1 -1
		and a1 t0 t1 # result in a1
		ret # return 
	# trans_to_signed
	trans_to_signed: # arguments: a0 = bits, a1 = number
		# DO NOT EDIT THIS FUNCTION
		addi a0 a0 -1 # 32 -1
		srl t0 a1 a0 # number >> 31
		li t1 1
		and t0 t0 t1 # number >> 31 & 1
		bne t0 t1 signed_end 
		signed:
			addi a0 a0 1
			sll t1 t1 a0 
			addi t1 t1 -1
			xor t0 a1 t1
			addi t0 t0 1 # ~number+1
			li t1 -1
			mul a0 t0 t1 # -(~number+1)
			mv a1 a0
		signed_end:
			mv a0 a1
			ret # return signed in a0

# format type choose
format_type_choose: # arguments: a0 = instruction
	# judge which type is.
	# u should managed i u j type, if this week has enough time ,you may manage it by yourself.
	
	# ra to stack
	addi sp sp -4
	sw ra 0(sp)

	li a1 7 # length of opcode
	li a2 0 # start position of opcode
	jal load_segment_with_position # return opcode in a1

	li t0 0x33 # r type opcode
	beq a1 t0 r_type
	li t0 0x67 # I type opcode(jalr)
	beq a1 t0 i_jalr
	li t0 0x13 # I type opcode(addi ori)
	beq a1 t0 i_normal
	li t0 0x03 # I type opcode(lb,lbu,lw)
	beq a1 t0 i_load
	li t0 0x23 # Is type opcode(lb,lbu,lw)
	beq a1 t0 s_type
	li t0 0x6f # UJ type opcode
	beq a1 t0 uj_type
	li t0 0x37 # U type opcode
	beq a1 t0 u_type
	li t0 0x63 # b type opcode
	beq a1 t0 b_type
	
	# r_type
	r_type: # arguments: a0 = instruction
		# r type to choose
		# save:
		# s0 = instruction name
		# s1 = the first register
		# s2 = the second register
		# s3 = the third register
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rs1
		li a2 15 # start position of rs1
		jal load_segment_with_position # return rs1 in a1
		mv a0 a1
		jal translate_register # return name of rs1 in a1
		mv s2 a1

		mv a0 s0
		li a1 5 # length of rs2
		li a2 20 # start position of rs2
		jal load_segment_with_position # return rs2 in a1
		mv a0 a1
		jal translate_register # return name of rs2 in a1
		mv s3 a1

		mv a0 s0
		li a1 5 # length of rd
		li a2 7 # start position of rd
		jal load_segment_with_position # return rd in a1
		mv a0 a1
		jal translate_register # return name of rd in a1
		mv s1 a1

		mv a0 s0
		li a1 3 # length of funct3
		li a2 12 # start position of funct3
		jal load_segment_with_position # return funct3 in a1

		li t0 0x0 # add funct3
		beq a1 t0 add_process
		li t0 0x6 # or funct3
		beq a1 t0 or_process
		li t0 0x2 # slt funct3
		beq a1 t0 slt_process
		li t0 0x3 # sltu funct3
		beq a1 t0 sltu_process
		li t0 0x1 # sll funct3
		beq a1 t0 sll_process
		add_process:
			la s0 instruction_add
			j print_r
		or_process:
			la s0 instruction_or
			j print_r
		slt_process:
			la s0 instruction_slt
			j print_r
		sltu_process:
			la s0 instruction_sltu
			j print_r
		sll_process:
			la s0 instruction_sll
			j print_r
	# i_jalr
	i_jalr: # arguments: a0 = instruction 
		# jalr
		# save:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s2 = the second register(rs1)
		# s3 = the third register(imm)
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rs1
		li a2 15 # start position of rs1
		jal load_segment_with_position # return rs1 in a1
		mv a0 a1
		jal translate_register # return name of rs1 in a1
		mv s2 a1

		mv a0 s0
		li a1 5 # length of rd
		li a2 7 # start position of rd
		jal load_segment_with_position # return rd in a1
		mv a0 a1
		jal translate_register # return name of rd in a1
		mv s1 a1

		mv a0 s0
		li a1 12 # length of imm[11:0]
		li a2 20 # start position of imm[11:0]
		jal load_segment_with_position # return imm[11:0] in a1
		# slli s3 a1 0
		
		li a0 12 
		# mv a1 s3 #a1 has been set
		jal trans_to_signed
		mv s3 a0

		# because the only i type whose opcode is 0x67, so we don't need to care about func3
		la s0 instruction_jalr
		j print_i_normal_b
	
	# i_normal
	i_normal: # arguments: a0 = instruction
		# i_normal to choose
		# save:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s2 = the second register(rs1)
		# s3 = the third register(imm)
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rs1
		li a2 15 # start position of rs1
		jal load_segment_with_position # return rs1 in a1
		mv a0 a1
		jal translate_register # return name of rs1 in a1
		mv s2 a1

		mv a0 s0
		li a1 5 # length of rd
		li a2 7 # start position of rd
		jal load_segment_with_position # return rd in a1
		mv a0 a1
		jal translate_register # return name of rd in a1
		mv s1 a1

		mv a0 s0
		li a1 12 # length of imm[11:0]
		li a2 20 # start position of imm[11:0]
		jal load_segment_with_position # return imm[11:0] in a1
		slli s3 a1 0

		li a0 12 
		# mv a1 s3 #a1 has been set
		jal trans_to_signed
		mv s3 a0

		mv a0 s0
		li a1 3 # length of funct3
		li a2 12 # start position of funct3
		jal load_segment_with_position # return funct3 in a1

		li t0 0x0 # addi funct3
		beq a1 t0 addi_process
		li t0 0x6 # ori funct3
		beq a1 t0 ori_process
		
		addi_process:
			la s0 instruction_addi
			j print_i_normal_b
		ori_process:
			la s0 instruction_ori
			j print_i_normal_b

	
	# i_load
	i_load:# arguments: a0 = instruction
		# i_type_addi_ori to choose
		# save:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s2 = the second register(rs1)
		# s3 = the third register(imm)
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rs1
		li a2 15 # start position of rs1
		jal load_segment_with_position # return rs1 in a1
		mv a0 a1
		jal translate_register # return name of rs1 in a1
		mv s2 a1

		mv a0 s0
		li a1 5 # length of rd
		li a2 7 # start position of rd
		jal load_segment_with_position # return rd in a1
		mv a0 a1
		jal translate_register # return name of rd in a1
		mv s1 a1

		mv a0 s0
		li a1 12 # length of imm[11:0]
		li a2 20 # start position of imm[11:0]
		jal load_segment_with_position # return imm[11:0] in a1
		slli s3 a1 0
		
		mv a0 s0
		li a1 3 # length of funct3
		li a2 12 # start position of funct3
		jal load_segment_with_position # return funct3 in a1

		li t0 0x0 # lb funct3
		beq a1 t0 lb_process
		li t0 0x2 # lw funct3
		beq a1 t0 lw_process
		li t0 0x4 # lbu funct3
		beq a1 t0 lbu_process

		lb_process:
			mv s0 a1
			li a0 12 
			mv a1 s3 
			jal trans_to_signed
			mv s3 a0
			mv a1 s0

			la s0 instruction_lb
			j print_i_load_s
		lw_process:
			mv s0 a1
			li a0 12 
			mv a1 s3 
			jal trans_to_signed
			mv s3 a0
			mv a1 s0

			la s0 instruction_lw
			j print_i_load_s
		lbu_process:
			mv s0 a1
			li a0 12 
			mv a1 s3 
			jal trans_to_signed
			mv s3 a0
			mv a1 s0

			la s0 instruction_lbu
			j print_i_load_s

	# s_type 
	s_type:# arguments: a0 = instruction
		# s_type to choose
		# save:
		# s0 = instruction name
		# s1 = the first register(rs2)
		# s2 = the second register(rs1)
		# s3 = the third register(imm)
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rs1
		li a2 15 # start position of rs1
		jal load_segment_with_position # return rs1 in a1
		mv a0 a1
		jal translate_register # return name of rs1 in a1
		mv s2 a1

		mv a0 s0
		li a1 5 # length of rs2
		li a2 20 # start position of rs2
		jal load_segment_with_position # return rs2 in a1
		mv a0 a1
		jal translate_register # return name of rs2 in a1
		mv s1 a1

		mv a0 s0
		# use s4-* to combine imm
		li a1 5 # length of imm[4:0]
		li a2 7 # start position of imm[4:0]
		jal load_segment_with_position # return imm[4:0] in a1
		slli s4 a1 0 
		li a1 7 # length of imm[11:5]
		li a2 25 # start position of imm[11:5]
		jal load_segment_with_position # return imm[11:5] in a1
		slli s5 a1 5
		add s3 s4 s5 # s3 = the third register(imm)

		mv s0 a0
		# trans_to_signed
		li a0 12 
		mv a1 s3
		jal trans_to_signed
		mv s3 a0
		mv a0 s0

		li a1 3 # length of funct3
		li a2 12 # start position of funct3
		jal load_segment_with_position # return funct3 in a1

		li t0 0x0 # sb funct3
		beq a1 t0 sb_process
		li t0 0x2 # sw funct3
		beq a1 t0 sw_process

		sb_process:
			la s0 instruction_sb
			j print_i_load_s
		sw_process:
			la s0 instruction_sw
			j print_i_load_s
	# u_type
	u_type: # arguments: a0 = instruction
		#lui
		# save:
		# s0 = instruction name
		# s1 = the first register(rd)
		# s3 = the third register(imm)
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rd
		li a2 7 # start position of rd
		jal load_segment_with_position # return rd in a1
		mv a0 a1
		jal translate_register # return name of rd in a1
		mv s1 a1

		mv a0 s0
		
		li a1 20 # length of imm[31:12]
		li a2 12 # start position of imm[31:12]
		jal load_segment_with_position # return imm[31:12] in a1
		slli s3 a1 0

		# there's no func3 in lui

		la s0 instruction_lui
		j print_uj_u
	# uj_type
	uj_type: # arguments: a0 = instruction
		# later to manage
		# uj_type:jal
		# jal ra label(imm)
		# s0 = instruction name
		# s1 = the first register
		# s3 = the second register(imm)

		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rd
		li a2 7 # start position of rd
		jal load_segment_with_position # return rd in a1
		mv a0 a1
		jal translate_register # return name of rd in a1
		mv s1 a1

		mv a0 s0
		# use s4-* to combine imm
		li a1 1 #length of imm[11]
		li a2 20 # start position of imm[11]
		jal load_segment_with_position # return imm[11] in a1
		slli s4 a1 11

		li a1 10 # length of imm[10:1]
		li a2 21 # start position of imm[10:1]
		jal load_segment_with_position # return imm[10:1] in a1
		slli s5 a1 1

		li a1 8 # length of imm[19:12]
		li a2 12 # start position of imm[19:12]
		jal load_segment_with_position # return imm[19:12] in a1
		slli s6 a1 12

		li a1 1 # length of imm[20]
		li a2 31 # start position of imm[20]
		jal load_segment_with_position # return imm[20] in a1
		slli s7 a1 20

		add s6 s6 s7 
		add s5 s5 s6 
		add s3 s4 s5 # s3 = the third register(imm)

		mv s0 a1
		li a0 21 
		mv a1 s3 
		jal trans_to_signed
		mv s3 a0
		mv a1 s0

		#there's no func3 in jal(uj_type)

		la s0 instruction_jal
		j print_uj_u
	# b type
	b_type: # arguments: a0 = instruction
		# b type to choose
		# save:
		# s0 = instruction name
		# s1 = the first register
		# s2 = the second register
		# s3 = the third register(imm)
		mv s0 a0 # temporary storage for the instruction

		li a1 5 # length of rs1
		li a2 15 # start position of rs1
		jal load_segment_with_position # return rs1 in a1
		mv a0 a1
		jal translate_register # return name of rs1 in a1
		mv s1 a1

		mv a0 s0
		li a1 5 # length of rs2
		li a2 20 # start position of rs2
		jal load_segment_with_position # return rs2 in a1
		mv a0 a1
		jal translate_register # return name of rs2 in a1
		mv s2 a1

		mv a0 s0
		# use s4-* to combine imm
		li a1 1 # length of imm[11]
		li a2 7 # start position of imm[11]
		jal load_segment_with_position # return imm[11] in a1
		slli s4 a1 11 
		li a1 4 # length of imm[4:1]
		li a2 8 # start position of imm[4:1]
		jal load_segment_with_position # return imm[4:1] in a1
		slli s5 a1 1 
		li a1 6 # length of imm[10:5]
		li a2 25 # start position of imm[10:5]
		jal load_segment_with_position # return imm[10:5] in a1
		slli s6 a1 5 
		li a1 1 # length of imm[12]
		li a2 31 # start position of imm[12]
		jal load_segment_with_position # return imm[12] in a1
		slli s7 a1 12
		add s6 s6 s7 
		add s5 s5 s6 
		add s3 s4 s5 # s3 = the third register(imm)

		li a1 3 # length of funct3
		li a2 12 # start position of funct3
		jal load_segment_with_position # return funct3 in a1

		mv s0 a1
		# trans_to_signed
		li a0 13 
		mv a1 s3
		jal trans_to_signed
		mv s3 a0
		mv a1 s0

		li t0 0x0 # beq funct3
		beq a1 t0 beq_process
		li t0 0x1 # bne funct3
		beq a1 t0 bne_process
		li t0 0x4 # blt funct3
		beq a1 t0 blt_process
		li t0 0x5 # bge funct3
		beq a1 t0 bge_process
		beq_process:
			la s0 instruction_beq
			j print_i_normal_b
		bne_process:
			la s0 instruction_bne
			j print_i_normal_b
		blt_process:
			la s0 instruction_blt
			j print_i_normal_b
		bge_process:
			la s0 instruction_bge
			j print_i_normal_b

.globl main
main:  
	# main function
	# DO NOT EDIT THIS FUNCTION
	la s0 machine_codes	# load machine codes
	la s1 lines_of_machine_codes
	lw s1 0(s1)	# load lines of machine codes

	beqz s1 return_0 # suppose input is null, CHECKING　ERROR
	mv t0 zero # lines of codes appendix
	main_while_loop:
		# to stack
		addi sp sp -12
		sw s0 0(sp)
		sw s1 4(sp)
		sw t0 8(sp)

		lw a0 0(s0) # a0 = machine code
		jal format_type_choose # into format choose function

		# from stack
		lw s0 0(sp)
		lw s1 4(sp)
		lw t0 8(sp)
		addi sp sp 12

		addi s0 s0 4 # update the input list
		addi t0 t0 1 # update appendix
		bne s1 t0 main_while_loop # while (lines of machine codes) != lines of codes appendix
	# main function
	j return_0