#include "arm/sysregs.h"		// system register constants (SCTLR, HCR, SCR, SPSR values)

#include "mm.h"				// memory constants like LOW_MEMORY

.section ".text.boot"			// keep this at the very start of the kernel image so the cpu runs it first

.globl _start				// entry point, the linker needs to see this
_start:
	mrs	x0, mpidr_el1		// get cpu info, mpidr_el1 holds which core we are
	and	x0, x0,#0xFF		// only need the lowest byte, that's the cpu number
	cbz	x0, master		// cpu 0 is the main one, so it goes to master
	b	proc_hang		// every other cpu goes to hang

proc_hang:
	b 	proc_hang		// just loops forever so the extra cpus do nothing

master:
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED	// mmu off value (also caches off), we don't set up paging yet
	msr	sctlr_el1, x0		// set it in the EL1 system control register

	ldr	x0, =HCR_VALUE		// makes EL1 run in 64 bit mode instead of 32 bit
	msr	hcr_el2, x0		// set it in the EL2 hypervisor config register

	ldr	x0, =SCR_VALUE		// EL3 config, says the next level (EL2) is 64 bit and non secure
	msr	scr_el3, x0		// set it in the EL3 secure config register

	ldr	x0, =SPSR_VALUE_EL2	// state we want when we land in EL2 (EL2 mode, interrupts masked)
	msr	spsr_el3, x0		// eret copies this into the cpu state when it runs

	adr	x0, el2_entry		// address of el2_entry, where we want to land
	msr	elr_el3, x0		// eret jumps to whatever address is in here

	eret				// go from EL3 down to EL2, lands at el2_entry

el2_entry:
	ldr x0, =SPSR_VALUE		// state we want when we land in EL1 (EL1 mode, interrupts masked)
	msr spsr_el2, x0		// eret copies this into the cpu state when it runs

	adr x0, el1_entry		// address of el1_entry, where we want to land
	msr elr_el2, x0			// eret jumps to whatever address is in here

	eret				// go from EL2 down to EL1, lands at el1_entry

el1_entry:
	adr	x0, bss_begin		// start of .bss (symbol comes from the linker script)
	adr	x1, bss_end		// end of .bss
	sub	x1, x1, x0		// size = end - start, memzero wants a size not an end address
	bl 	memzero			// zero out .bss, C expects uninitialized globals to be 0

	mov	sp, #LOW_MEMORY		// set the stack pointer, C code needs a stack to work
	bl	kernel_main		// jump into the C code
	b 	proc_hang		// should never come here, if kernel_main returns we just hang
