.include "mmu.inc"

.export _nrpck_hault, _nrpck_sleep10ms, _nrpck_sleep50ms, _nrpck_sleep100ms, _nrpck_bss_end


.segment	"CODE"
.proc	_nrpck_hault: near
.segment	"CODE"
	mmu $4D
	L0043:	bra     L0043
.endproc

.segment	"CODE"
.proc	_nrpck_sleep10ms: near
.segment	"CODE"
	mmu $50
	rts
.endproc

.segment	"CODE"
.proc	_nrpck_sleep50ms: near
.segment	"CODE"
	mmu $51
	rts
.endproc

.segment	"CODE"
.proc	_nrpck_sleep100ms: near
.segment	"CODE"
	mmu $52
	rts
.endproc


.segment	"CODE"
.proc	_nrpck_bss_end: near
.segment	"CODE"
	ldx     #$3E
	lda     #$80
	rts
.endproc