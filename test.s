	.text
	.file	"test.q"
	.globl	test                    # -- Begin function test
	.p2align	4, 0x90
	.type	test,@function
test:                                   # @test
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movl	$8, %r14d
	movl	$5000, %eax             # imm = 0x1388
	mulq	%r14
	jo	.LBB0_1
# %bb.3:                                # %newMulNormal
	movq	%rax, %rdi
	callq	malloc
	movq	%rax, %rbx
	movl	$16, %edi
	callq	malloc
	movq	$5000, (%rax)           # imm = 0x1388
	movq	%rbx, 8(%rax)
	movq	%rax, -24(%rbp)
	movq	%rsp, %rbx
	addq	$-16, %rbx
	movq	%rbx, %rsp
	movl	$5000, %eax             # imm = 0x1388
	mulq	%r14
	jo	.LBB0_4
# %bb.5:                                # %newMulNormal4
	movq	%rax, %rdi
	callq	malloc
	movq	%rax, %r14
	movl	$16, %edi
	callq	malloc
	movq	$5000, (%rax)           # imm = 0x1388
	movq	%r14, 8(%rax)
	movq	%rax, (%rbx)
	leaq	-16(%rbp), %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB0_1:                                # %newMulOverFlow
	.cfi_def_cfa %rbp, 16
	movl	$.L__unnamed_1, %edi
	movl	$3, %esi
	jmp	.LBB0_2
.LBB0_4:                                # %newMulOverFlow3
	movl	$.L__unnamed_1, %edi
	movl	$4, %esi
.LBB0_2:                                # %newMulOverFlow
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"overflow when doing arithmatic calculation at line: %d\n"
	.size	.L__unnamed_1, 56

	.section	".note.GNU-stack","",@progbits
