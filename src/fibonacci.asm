SECTION TEXT ;tem que remover esse comentario
mul_n:                MACRO 
MULT N
STORE N
ENDMACRO
INPUT N ;esse tambem
LOAD N
FAT: SUB ONE
JMPZ FIM
STORE AUX ; e esse
MUL_N 
loAD AUX
JMP Fat
Fim: OUTPUT N ; mais esse aqui
Stop
SECTION DATA
AUX: SPACE
N: SPACE
ONE: CONST 1
