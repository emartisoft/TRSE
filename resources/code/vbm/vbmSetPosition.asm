vbmSetPosition
	lda vbmX ; x position
        pha      ; store x position for later
        lsr      ; divide by 8 to get column number
	lsr
	lsr
        tax      ; locate address of start of column
	lda vbmScrL,x
	ldy vbmScrH,x
	clc
        adc vbmY ; add y position
	bcc vbmSPos_noOverflow
	iny ; overflow
vbmSPos_noOverflow
        sta screenmemory   ; set screenmemory to the correct address in the bitmap
	sty screenmemory+1
        pla      ; retrieve x position
	and #7   ; get x offset 0-7
	sta vbmX ; for use in sprite routines
