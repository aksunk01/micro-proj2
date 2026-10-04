#ifndef _SYSREGS_H
#define _SYSREGS_H

// ***************************************
// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
// ***************************************

#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)	// reserved bits that have to be 1, manual says so
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)	// EL1 data accesses are little endian
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)	// same thing but for EL0
#define SCTLR_I_CACHE_DISABLED          (0 << 12)	// instruction cache off, keeping it simple for now
#define SCTLR_D_CACHE_DISABLED          (0 << 2)	// data cache off
#define SCTLR_MMU_DISABLED              (0 << 0)	// mmu off, addresses are physical
#define SCTLR_MMU_ENABLED               (1 << 0)	// not used yet, will need this once we do paging

// everything off except the reserved bits, this is what gets written to sctlr_el1 in boot.S
#define SCTLR_VALUE_MMU_DISABLED	(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
// ***************************************

#define HCR_RW	    			(1 << 31)	// 1 means EL1 runs in 64 bit mode, 0 would be 32 bit
#define HCR_VALUE			HCR_RW		// 64 bit EL1 is the only thing we care about here

// ***************************************
// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
// ***************************************

#define SCR_RESERVED	    		(3 << 4)	// reserved bits that have to be 1
#define SCR_RW				(1 << 10)	// next level down (EL2) is 64 bit
#define SCR_NS				(1 << 0)	// lower levels are non secure
#define SCR_VALUE	    	    	(SCR_RESERVED | SCR_RW | SCR_NS)

// ***************************************
// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
// ***************************************

#define SPSR_MASK_ALL 			(7 << 6)	// masks the interrupts (and async abort) so nothing fires while we boot
#define SPSR_EL1h			(5 << 0)	// EL1 using its own stack pointer (SP_EL1), the "h" is for handler
#define SPSR_VALUE			(SPSR_MASK_ALL | SPSR_EL1h)	// what the cpu looks like after eret into EL1


#define SPSR_EL2h           (9 << 0)	// same idea but EL2 using SP_EL2
#define SPSR_VALUE_EL2      (SPSR_MASK_ALL | SPSR_EL2h)	// what the cpu looks like after eret into EL2

#endif
