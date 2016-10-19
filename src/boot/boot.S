# Constants for the multiboot header
.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1badb002
.set CHECKSUM, -(MAGIC + FLAGS)

# The multiboot header
# The boot loader searches for the magic number in the first 8 KiB
# of the kernel file, aligned at a 32-bit boundary
.section .multiboot
# Align at 32-bit boundary
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Stack
# Aligned to 16 bits because the System V ABI standard says so
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# _start is the entry point to the kernel. The bootloader jumps there after
# the kernel has been loaded into memory
.section .text
.global _start
.type _start, @function
_start:
	# We are in 32-bit protected mode, interrupts disabled, paging disabled

	# Set up stack
	mov $stack_top, %esp

	# Init crucial processor state
	# (floating point instrs, instr set extensions)
	# gdt, paging
	# (c++ features)
	lgdtl (gdt_descriptor)
	ljmpl $0x08, $1f

1:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	# Enter high-level kernel
	call kernel_main

	# The kernel returned although it should never do that
	# We just lock the cpu up

	# Disable interrupts
	cli
	# Wait for next interrupt
2:	hlt
	jmp 2b

# Set size of _start symbol (useful for debugging & call tracing)
.size _start, . - _start
