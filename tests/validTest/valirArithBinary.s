	.text
	.file	"valirArithBinary.q"
	.globl	call1                   # -- Begin function call1
	.p2align	4, 0x90
	.type	call1,@function
call1:                                  # @call1
	.cfi_startproc
# %bb.0:                                # %entry
	movq	%rdi, %rax
	movq	%rdi, -8(%rsp)
	retq
.Lfunc_end0:
	.size	call1, .Lfunc_end0-call1
	.cfi_endproc
                                        # -- End function
	.globl	plusNum200              # -- Begin function plusNum200
	.p2align	4, 0x90
	.type	plusNum200,@function
plusNum200:                             # @plusNum200
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$200, -8(%rsp)
	movl	$200, %eax
	retq
.Lfunc_end1:
	.size	plusNum200, .Lfunc_end1-plusNum200
	.cfi_endproc
                                        # -- End function
	.globl	plusVar                 # -- Begin function plusVar
	.p2align	4, 0x90
	.type	plusVar,@function
plusVar:                                # @plusVar
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rax
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	addq	%rsi, %rax
	jo	.LBB2_2
# %bb.1:                                # %normal
	movq	%rax, (%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB2_2:                                # %overflow
	.cfi_def_cfa_offset 32
	movl	$.L__unnamed_1, %edi
	movl	$14, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end2:
	.size	plusVar, .Lfunc_end2-plusVar
	.cfi_endproc
                                        # -- End function
	.globl	plusCall                # -- Begin function plusCall
	.p2align	4, 0x90
	.type	plusCall,@function
plusCall:                               # @plusCall
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	callq	call1
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	callq	call1
	addq	%rbx, %rax
	jo	.LBB3_2
# %bb.1:                                # %normal
	movq	%rax, 16(%rsp)
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.LBB3_2:                                # %overflow
	.cfi_def_cfa_offset 48
	movl	$.L__unnamed_2, %edi
	movl	$18, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end3:
	.size	plusCall, .Lfunc_end3-plusCall
	.cfi_endproc
                                        # -- End function
	.globl	plusArrIndex            # -- Begin function plusArrIndex
	.p2align	4, 0x90
	.type	plusArrIndex,@function
plusArrIndex:                           # @plusArrIndex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$8, %ecx
	movl	$2, %eax
	mulq	%rcx
	jo	.LBB4_1
# %bb.3:                                # %normal
	movl	$16, %edi
	callq	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rcx, 8(%rax)
	movq	%rsp, %rcx
	addq	$-16, %rcx
	movq	%rcx, %rsp
	movq	-8(%rbp), %rdx
	movq	8(%rdx), %rax
	addq	(%rdx), %rax
	jo	.LBB4_4
# %bb.5:                                # %normal6
	movq	%rax, (%rcx)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB4_1:                                # %overflow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_3, %edi
	movl	$22, %esi
	jmp	.LBB4_2
.LBB4_4:                                # %overflow5
	movl	$.L__unnamed_4, %edi
	movl	$25, %esi
.LBB4_2:                                # %overflow
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end4:
	.size	plusArrIndex, .Lfunc_end4-plusArrIndex
	.cfi_endproc
                                        # -- End function
	.globl	minusNum0               # -- Begin function minusNum0
	.p2align	4, 0x90
	.type	minusNum0,@function
minusNum0:                              # @minusNum0
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$0, -8(%rsp)
	xorl	%eax, %eax
	retq
.Lfunc_end5:
	.size	minusNum0, .Lfunc_end5-minusNum0
	.cfi_endproc
                                        # -- End function
	.globl	minusVar                # -- Begin function minusVar
	.p2align	4, 0x90
	.type	minusVar,@function
minusVar:                               # @minusVar
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rax
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	subq	%rsi, %rax
	jo	.LBB6_2
# %bb.1:                                # %normal
	movq	%rax, (%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB6_2:                                # %overflow
	.cfi_def_cfa_offset 32
	movl	$.L__unnamed_5, %edi
	movl	$34, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end6:
	.size	minusVar, .Lfunc_end6-minusVar
	.cfi_endproc
                                        # -- End function
	.globl	minusCall               # -- Begin function minusCall
	.p2align	4, 0x90
	.type	minusCall,@function
minusCall:                              # @minusCall
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	callq	call1
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	callq	call1
	subq	%rax, %rbx
	jo	.LBB7_2
# %bb.1:                                # %normal
	movq	%rbx, 16(%rsp)
	movq	%rbx, %rax
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.LBB7_2:                                # %overflow
	.cfi_def_cfa_offset 48
	movl	$.L__unnamed_6, %edi
	movl	$38, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end7:
	.size	minusCall, .Lfunc_end7-minusCall
	.cfi_endproc
                                        # -- End function
	.globl	minusArrIndex           # -- Begin function minusArrIndex
	.p2align	4, 0x90
	.type	minusArrIndex,@function
minusArrIndex:                          # @minusArrIndex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$8, %ecx
	movl	$2, %eax
	mulq	%rcx
	jo	.LBB8_1
# %bb.3:                                # %normal
	movl	$16, %edi
	callq	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rcx, 8(%rax)
	movq	%rsp, %rcx
	addq	$-16, %rcx
	movq	%rcx, %rsp
	movq	-8(%rbp), %rdx
	movq	(%rdx), %rax
	subq	8(%rdx), %rax
	jo	.LBB8_4
# %bb.5:                                # %normal6
	movq	%rax, (%rcx)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB8_1:                                # %overflow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_7, %edi
	movl	$42, %esi
	jmp	.LBB8_2
.LBB8_4:                                # %overflow5
	movl	$.L__unnamed_8, %edi
	movl	$45, %esi
.LBB8_2:                                # %overflow
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end8:
	.size	minusArrIndex, .Lfunc_end8-minusArrIndex
	.cfi_endproc
                                        # -- End function
	.globl	starNum100              # -- Begin function starNum100
	.p2align	4, 0x90
	.type	starNum100,@function
starNum100:                             # @starNum100
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$100, -8(%rsp)
	movl	$100, %eax
	retq
.Lfunc_end9:
	.size	starNum100, .Lfunc_end9-starNum100
	.cfi_endproc
                                        # -- End function
	.globl	starVar                 # -- Begin function starVar
	.p2align	4, 0x90
	.type	starVar,@function
starVar:                                # @starVar
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rax
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	imulq	%rsi, %rax
	jo	.LBB10_2
# %bb.1:                                # %normal
	movq	%rax, (%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB10_2:                               # %overflow
	.cfi_def_cfa_offset 32
	movl	$.L__unnamed_9, %edi
	movl	$54, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end10:
	.size	starVar, .Lfunc_end10-starVar
	.cfi_endproc
                                        # -- End function
	.globl	starCall                # -- Begin function starCall
	.p2align	4, 0x90
	.type	starCall,@function
starCall:                               # @starCall
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	callq	call1
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	callq	call1
	imulq	%rbx, %rax
	jo	.LBB11_2
# %bb.1:                                # %normal
	movq	%rax, 16(%rsp)
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.LBB11_2:                               # %overflow
	.cfi_def_cfa_offset 48
	movl	$.L__unnamed_10, %edi
	movl	$58, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end11:
	.size	starCall, .Lfunc_end11-starCall
	.cfi_endproc
                                        # -- End function
	.globl	starArrIndex            # -- Begin function starArrIndex
	.p2align	4, 0x90
	.type	starArrIndex,@function
starArrIndex:                           # @starArrIndex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$8, %ecx
	movl	$2, %eax
	mulq	%rcx
	jo	.LBB12_1
# %bb.3:                                # %normal
	movl	$16, %edi
	callq	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rcx, 8(%rax)
	movq	%rsp, %rcx
	addq	$-16, %rcx
	movq	%rcx, %rsp
	movq	-8(%rbp), %rdx
	movq	8(%rdx), %rax
	imulq	(%rdx), %rax
	jo	.LBB12_4
# %bb.5:                                # %normal6
	movq	%rax, (%rcx)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB12_1:                               # %overflow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_11, %edi
	movl	$62, %esi
	jmp	.LBB12_2
.LBB12_4:                               # %overflow5
	movl	$.L__unnamed_12, %edi
	movl	$65, %esi
.LBB12_2:                               # %overflow
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end12:
	.size	starArrIndex, .Lfunc_end12-starArrIndex
	.cfi_endproc
                                        # -- End function
	.globl	divNum1                 # -- Begin function divNum1
	.p2align	4, 0x90
	.type	divNum1,@function
divNum1:                                # @divNum1
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$1, -8(%rsp)
	movl	$1, %eax
	retq
.Lfunc_end13:
	.size	divNum1, .Lfunc_end13-divNum1
	.cfi_endproc
                                        # -- End function
	.globl	divVar                  # -- Begin function divVar
	.p2align	4, 0x90
	.type	divVar,@function
divVar:                                 # @divVar
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	testq	%rsi, %rsi
	je	.LBB14_1
# %bb.3:                                # %divnormal
	movq	%rdi, %rax
	cmpq	$-1, %rsi
	jne	.LBB14_6
# %bb.4:                                # %divnormal
	movabsq	$-9223372036854775808, %rcx # imm = 0x8000000000000000
	cmpq	%rcx, %rax
	je	.LBB14_5
.LBB14_6:                               # %normal
	cqto
	idivq	%rsi
	movq	%rax, (%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB14_1:                               # %divsion0
	.cfi_def_cfa_offset 32
	movl	$.L__unnamed_13, %edi
	jmp	.LBB14_2
.LBB14_5:                               # %overflow
	movl	$.L__unnamed_14, %edi
.LBB14_2:                               # %divsion0
	movl	$74, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end14:
	.size	divVar, .Lfunc_end14-divVar
	.cfi_endproc
                                        # -- End function
	.globl	divCall                 # -- Begin function divCall
	.p2align	4, 0x90
	.type	divCall,@function
divCall:                                # @divCall
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	callq	call1
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	callq	call1
	testq	%rax, %rax
	je	.LBB15_1
# %bb.3:                                # %divnormal
	movq	%rax, %rcx
	cmpq	$-1, %rax
	jne	.LBB15_6
# %bb.4:                                # %divnormal
	movabsq	$-9223372036854775808, %rax # imm = 0x8000000000000000
	cmpq	%rax, %rbx
	je	.LBB15_5
.LBB15_6:                               # %normal
	movq	%rbx, %rax
	cqto
	idivq	%rcx
	movq	%rax, 16(%rsp)
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.LBB15_1:                               # %divsion0
	.cfi_def_cfa_offset 48
	movl	$.L__unnamed_15, %edi
	jmp	.LBB15_2
.LBB15_5:                               # %overflow
	movl	$.L__unnamed_16, %edi
.LBB15_2:                               # %divsion0
	movl	$78, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end15:
	.size	divCall, .Lfunc_end15-divCall
	.cfi_endproc
                                        # -- End function
	.globl	divArrIndex             # -- Begin function divArrIndex
	.p2align	4, 0x90
	.type	divArrIndex,@function
divArrIndex:                            # @divArrIndex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$8, %ecx
	movl	$2, %eax
	mulq	%rcx
	jo	.LBB16_1
# %bb.2:                                # %normal
	movl	$16, %edi
	callq	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rcx, 8(%rax)
	movq	%rsp, %rcx
	addq	$-16, %rcx
	movq	%rcx, %rsp
	movq	-8(%rbp), %rax
	movq	8(%rax), %rsi
	testq	%rsi, %rsi
	je	.LBB16_3
# %bb.6:                                # %divnormal
	movq	(%rax), %rax
	cmpq	$-1, %rsi
	jne	.LBB16_9
# %bb.7:                                # %divnormal
	movabsq	$-9223372036854775808, %rdx # imm = 0x8000000000000000
	cmpq	%rdx, %rax
	je	.LBB16_8
.LBB16_9:                               # %normal5
	cqto
	idivq	%rsi
	movq	%rax, (%rcx)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB16_1:                               # %overflow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_17, %edi
	movl	$82, %esi
	jmp	.LBB16_5
.LBB16_3:                               # %divsion0
	movl	$.L__unnamed_18, %edi
	jmp	.LBB16_4
.LBB16_8:                               # %overflow4
	movl	$.L__unnamed_19, %edi
.LBB16_4:                               # %divsion0
	movl	$85, %esi
.LBB16_5:                               # %divsion0
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end16:
	.size	divArrIndex, .Lfunc_end16-divArrIndex
	.cfi_endproc
                                        # -- End function
	.globl	andNum4                 # -- Begin function andNum4
	.p2align	4, 0x90
	.type	andNum4,@function
andNum4:                                # @andNum4
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$4, -8(%rsp)
	movl	$4, %eax
	retq
.Lfunc_end17:
	.size	andNum4, .Lfunc_end17-andNum4
	.cfi_endproc
                                        # -- End function
	.globl	andVar                  # -- Begin function andVar
	.p2align	4, 0x90
	.type	andVar,@function
andVar:                                 # @andVar
	.cfi_startproc
# %bb.0:                                # %entry
	movq	%rdi, %rax
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	andq	%rsi, %rax
	movq	%rax, -24(%rsp)
	retq
.Lfunc_end18:
	.size	andVar, .Lfunc_end18-andVar
	.cfi_endproc
                                        # -- End function
	.globl	andCall                 # -- Begin function andCall
	.p2align	4, 0x90
	.type	andCall,@function
andCall:                                # @andCall
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	callq	call1
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	callq	call1
	andq	%rbx, %rax
	movq	%rax, 16(%rsp)
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end19:
	.size	andCall, .Lfunc_end19-andCall
	.cfi_endproc
                                        # -- End function
	.globl	andArrIndex             # -- Begin function andArrIndex
	.p2align	4, 0x90
	.type	andArrIndex,@function
andArrIndex:                            # @andArrIndex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$8, %ecx
	movl	$2, %eax
	mulq	%rcx
	jo	.LBB20_2
# %bb.1:                                # %normal
	movl	$16, %edi
	callq	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rcx, 8(%rax)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	-8(%rbp), %rdx
	movq	(%rdx), %rax
	andq	8(%rdx), %rax
	movq	%rax, -16(%rcx)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB20_2:                               # %overflow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_20, %edi
	movl	$102, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end20:
	.size	andArrIndex, .Lfunc_end20-andArrIndex
	.cfi_endproc
                                        # -- End function
	.globl	orNum4                  # -- Begin function orNum4
	.p2align	4, 0x90
	.type	orNum4,@function
orNum4:                                 # @orNum4
	.cfi_startproc
# %bb.0:                                # %entry
	movq	$4, -8(%rsp)
	movl	$4, %eax
	retq
.Lfunc_end21:
	.size	orNum4, .Lfunc_end21-orNum4
	.cfi_endproc
                                        # -- End function
	.globl	orVar                   # -- Begin function orVar
	.p2align	4, 0x90
	.type	orVar,@function
orVar:                                  # @orVar
	.cfi_startproc
# %bb.0:                                # %entry
	movq	%rdi, %rax
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	orq	%rsi, %rax
	movq	%rax, -24(%rsp)
	retq
.Lfunc_end22:
	.size	orVar, .Lfunc_end22-orVar
	.cfi_endproc
                                        # -- End function
	.globl	orCall                  # -- Begin function orCall
	.p2align	4, 0x90
	.type	orCall,@function
orCall:                                 # @orCall
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, 24(%rsp)
	movq	%rsi, 8(%rsp)
	callq	call1
	movq	%rax, %rbx
	movq	8(%rsp), %rdi
	callq	call1
	orq	%rbx, %rax
	movq	%rax, 16(%rsp)
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end23:
	.size	orCall, .Lfunc_end23-orCall
	.cfi_endproc
                                        # -- End function
	.globl	orArrIndex              # -- Begin function orArrIndex
	.p2align	4, 0x90
	.type	orArrIndex,@function
orArrIndex:                             # @orArrIndex
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$8, %ecx
	movl	$2, %eax
	mulq	%rcx
	jo	.LBB24_2
# %bb.1:                                # %normal
	movl	$16, %edi
	callq	malloc
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rcx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rcx, 8(%rax)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	-8(%rbp), %rdx
	movq	(%rdx), %rax
	orq	8(%rdx), %rax
	movq	%rax, -16(%rcx)
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB24_2:                               # %overflow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_21, %edi
	movl	$122, %esi
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end24:
	.size	orArrIndex, .Lfunc_end24-orArrIndex
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_1, 22

	.type	.L__unnamed_2,@object   # @1
.L__unnamed_2:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_2, 22

	.type	.L__unnamed_3,@object   # @2
.L__unnamed_3:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_3, 22

	.type	.L__unnamed_4,@object   # @3
.L__unnamed_4:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_4, 22

	.type	.L__unnamed_5,@object   # @4
.L__unnamed_5:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_5, 22

	.type	.L__unnamed_6,@object   # @5
.L__unnamed_6:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_6, 22

	.type	.L__unnamed_7,@object   # @6
.L__unnamed_7:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_7, 22

	.type	.L__unnamed_8,@object   # @7
.L__unnamed_8:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_8, 22

	.type	.L__unnamed_9,@object   # @8
.L__unnamed_9:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_9, 22

	.type	.L__unnamed_10,@object  # @9
.L__unnamed_10:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_10, 22

	.type	.L__unnamed_11,@object  # @10
.L__unnamed_11:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_11, 22

	.type	.L__unnamed_12,@object  # @11
.L__unnamed_12:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_12, 22

	.type	.L__unnamed_13,@object  # @12
.L__unnamed_13:
	.asciz	"division zero at line: %d\n"
	.size	.L__unnamed_13, 27

	.type	.L__unnamed_14,@object  # @13
.L__unnamed_14:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_14, 22

	.type	.L__unnamed_15,@object  # @14
.L__unnamed_15:
	.asciz	"division zero at line: %d\n"
	.size	.L__unnamed_15, 27

	.type	.L__unnamed_16,@object  # @15
.L__unnamed_16:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_16, 22

	.type	.L__unnamed_17,@object  # @16
.L__unnamed_17:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_17, 22

	.type	.L__unnamed_18,@object  # @17
.L__unnamed_18:
	.asciz	"division zero at line: %d\n"
	.size	.L__unnamed_18, 27

	.type	.L__unnamed_19,@object  # @18
.L__unnamed_19:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_19, 22

	.type	.L__unnamed_20,@object  # @19
.L__unnamed_20:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_20, 22

	.type	.L__unnamed_21,@object  # @20
.L__unnamed_21:
	.asciz	"overflow at line: %d\n"
	.size	.L__unnamed_21, 22

	.section	".note.GNU-stack","",@progbits
