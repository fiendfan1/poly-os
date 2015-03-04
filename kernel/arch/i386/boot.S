# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
.global stack_bottom
stack_bottom:
# .skip 16384 # 16 KiB
# .skip 32768 # 32 KiB
.skip 131072 # 128 KiB
# .skip 1000000 # 1 MiB
.global stack_top
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	# Set up stack.
	movl $stack_top, %esp

	# Initialize the core kernel before running the global constructors.
	call kernel_early

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
	hlt
.Lhang:
	jmp .Lhang
.size _start, . - _start
