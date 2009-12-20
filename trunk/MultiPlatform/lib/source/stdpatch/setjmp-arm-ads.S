
	EXPORT _setjmp
	IMPORT setjmp
	
        AREA	text, CODE, READONLY
_setjmp
				b setjmp

	END

