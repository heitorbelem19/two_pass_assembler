TrIANGuLO: 
;comentario		
                EQU 1
;comentrasa

SECTION TEXT	;linha dos dados
		INPUT		B
		INPUT		H
		LOAD		B
		MuLT		H
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
	SPACE
	DOIS:	CONST	2