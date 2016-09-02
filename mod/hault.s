.include "mmu.inc"

.export _nrpck_hault


.segment "CODE"

_nrpck_hault:
	mmu $4D
	L0043:	bra     L0043
rts

