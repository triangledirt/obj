DEBUG=
gcc $DEBUG -O3 -c case.c core.c filt.c fold.c gene.c jung.c obj.c sum.c tool.c
ar rcs libcase.a *.o
gcc $DEBUG -O3 -o test test.c -lcase -L.
