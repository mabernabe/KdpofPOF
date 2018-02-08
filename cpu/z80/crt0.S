.module crt0
.globl 	l__INITIALIZER
.globl 	s__INITIALIZER
.globl 	l__INITIALIZED
.globl 	s__INITIALIZED
.globl	_main
.globl	_isrRx
.globl	_isrTx
.globl	_isrT0
.globl	_isrT1
.globl	_nmi_isr
.area	_HEADER (ABS)
;; Reset vector
.org 	0H0000
	jp	init
.org    0H00a8
	ret
.org    0H00b0
        ret
.org 	0H040
	.dw _isrRx

.org 	0H048
	.dw _isrTx

.org 	0H050
	.dw _isrT0

.org 	0H052
	.dw _isrT1

.org	0H0066
	call	_nmi_isr
	reti

.org	0H0C0
init::
	;; Initialize Stack Pointer
	ld	sp,#0Hefff
	;; Initialize Internal Registers
	ld      bc,#0H0000
	ld      de,#0H0000
	ld      hl,#0H0000
	ld      ix,#0H0000
	ld      iy,#0H0000

	;; Initialize global variables
	call	gsinit
	call	zinit
	call	_main
	jp	_exit

	;; Debugger code
__clock::
	ld	a,#2
	rst     0H28
	ret

_exit::
	;; Exit - special code to the emulator
	ld	a,#0
	rst     0H30
1$:
	halt
	jr	1$


	.area   _GSINIT
gsinit::
	ld	bc, #l__INITIALIZER
	ld	a,b
	or	a,c
	jr	z, gsinit_next
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir
gsinit_next:
	ret

zinit::
	ld	hl, #_datastart
 	ld	bc, #_dataend
_clear_loop:
	ld	a, h
	sub	b
	jr	nz, _clear_next
	ld	a, l
	sub	c
	jr	z, _clear_exit
_clear_next:
	ld	(hl), #0
	inc	hl
	jr	_clear_loop
_clear_exit:
	.area   _GSFINAL
	ret

	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE
	.area	_INITIALIZER
	.area   _GSFINAL

	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _CODE
	.area	_DATA

_datastart::
	.area	_DATAFINAL
_dataend::
	.area   _HEAP
