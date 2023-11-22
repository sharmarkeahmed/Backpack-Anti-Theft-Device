.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _example_public_function, _second_public_function
    
    .global _write_0, _write_1, _wait_100us, _wait_1ms

_write_0: ; TOTAL : 6 highs, 14 lows
    
    BSET LATB, #6
    repeat #3
    nop

    ; 14 LOW (Note, 3 low during return, 2 low during next function call)
    
    ; --- 9 cycles ---
    BCLR LATB, #6 ; 1 cycle for clr
    repeat #6
    nop
    ; ----------------
    
    return ; 3 cycles
	
_write_1: ; TOTAL: 11 HIGH, 9 LOW
    
    ; 11 HIGH
    BSET LATB, #6
    repeat #8
    nop
    
    ; 9 LOW ( Note, 3 low during return, 2 low during next function call)
    
    ; -- 4 cycles ---
    BCLR LATB, #6
    repeat #1
    nop
    
    return ; 3 cycles
    
_wait_100us:
    repeat #1593 ; 1 cycle to load and prep
    nop ; 11 cycles to execute NOP 
    return ; 3 cycles for return

_wait_1ms:
    repeat #15993
    nop
    return
    ; total 16000 cycles
    
    
    


