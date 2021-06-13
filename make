gcc -ansi -pedantic -O3 -c agene.c alib.c aobj.c asum.c
ar rcs liba.a *.o
gcc -ansi -pedantic -O3 -o atest *.o atest.c
