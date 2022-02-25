DEBUG=-g
COMPILER=gcc
OPTIM=-O3
$COMPILER $DEBUG $OPTIM -c bit.c bool.c case.c core.c filt.c fold.c gene.c indx.c jack.c jung.c node.c obj.c op.c trace.c sum.c val.c
ar rcs libcase.a *.o
$COMPILER $DEBUG $OPTIM -o test test.c -lcase -L.
