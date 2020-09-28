SECTION TEXT ;tem que remover esse comentario
mul_n:                MACRO 
MULT N
STORE              N
ENDMACRO
INPUT         N ;esse tambem
LOAD N
FAT: 

    ;oioioi
  SUB ONE
COPY N1         M2
JMPZ FIM
STORE AUX ; e esse
MUL_N 
LABEL:
     ;com
          loAD AUX
JMP Fat
Fim: 

  ;asdiasiod
OUTPUT N ; mais esse aqui
Stop
SECTION                 DATA
AUX: SPACE
N: SPACE
ONE: CONST 1
