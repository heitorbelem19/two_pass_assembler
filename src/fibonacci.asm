TrIANGuLO: 
;comentario		
                EQU 1
;comentrasa

SECTION TEXT	;linha dos dados
		INPUT		B
		INPUT		H
		LOAD		B ;load load
		MuLT		H
		CASO: 
		;OLA

		IF TRIANGULO
		
		DIV		DOIS
		STORE		R
		OUTPUT	R
		STOP
SECTION DATA
	B:		
		SPACE
	H:		
		SPACE
	R:		
	SPACE 5 
	DOIS:	CONST	