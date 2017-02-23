
int_common:
	# ss, esp, eflags, cs, eip have already been pushed

	cld

	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	pushl %esp
	call idt_handler
	popl %esp

	popl %gs
	popl %fs
	popl %es
	popl %ds
	popl %edi
	popl %esi
	popl %ebp
	popl %ebx
	popl %edx
	popl %ecx
	popl %eax
	# Pop interrupt number and error code
	addl $8, %esp

	iret

# # define ERRCODE_MASK 01000000000000100111110100000000
# Is actually 0x40027d00 but we only generate isrs for the first 32 interrupts
#define ERRCODE_MASK 0x00027d00

.global idt_handlers
idt_handlers:
	i = 0
	.rept 32
	.ifeq ERRCODE_MASK & (1 << i)
	pushl $0
	.endif
	pushl $i
	jmp int_common
	i = i + 1
	.fill idt_handlers + i*9 - ., 1, 0xcc
	.endr

.global irq_handlers
irq_handlers:
	i = 0
	.rept 16
	pushl $0
	pushl $i+32
	jmp int_common
	i = i + 1
	.fill irq_handlers + i*9 - ., 1, 0xcc
	.endr
