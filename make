DEBUG=
gcc $DEBUG -O3 -c case.c core.c fold.c gene.c jung.c obj.c sum.c tool.c
ar rcs libcase.a *.o
gcc $DEBUG -O3 -o test *.o test.c
