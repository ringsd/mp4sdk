;
; arch/arm/setjmp.S
;
; setjmp/longjmp for the ARM architecture
;
;
;
; The jmp_buf is assumed to contain the following, in order:
;               r4
;               r5
;               r6
;               r7
;               r8
;               r9
;               r10
;               fp
;               sp
;               lr
;

	EXPORT setjmp
	EXPORT _setjmp
	EXPORT longjmp

        AREA	text, CODE, READONLY
_setjmp

setjmp
        stmia   r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
        mov     r0, #0
        mov     pc, lr

        AREA	text, CODE, READONLY

longjmp
        ldmia   r0, {r4, r5, r6, r7, r8, r9, r10, fp, sp, lr}
        mov     r0, r1
        mov     pc, lr
