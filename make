DEBUG=-g
gcc $DEBUG -O3 -c bit.c bool.c case.c core.c filt.c fold.c gene.c indx.c jung.c net.c obj.c op.c trace.c sum.c
ar rcs libcase.a *.o
gcc $DEBUG -O3 -o test test.c -lcase -L.
