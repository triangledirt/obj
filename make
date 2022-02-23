DEBUG=-g
COMPILER=gcc
$COMPILER $DEBUG -O3 -c bit.c bool.c case.c core.c filt.c fold.c gene.c indx.c jack.c jung.c node.c obj.c op.c trace.c sum.c
ar rcs libcase.a *.o
$COMPILER $DEBUG -O3 -o test test.c -lcase -L.
