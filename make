DEBUG=-g
COMPILER=gcc
OPTIM=-O3
SOURCES=`ls *.c | grep -v test.c`
$COMPILER $DEBUG $OPTIM -c $SOURCES
ar rcs libcase.a *.o
$COMPILER $DEBUG $OPTIM test.c -o test -lcase -L.
