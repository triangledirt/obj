gcc -ansi -pedantic -O3 -c acore.c agene.c ajung.c alib.c aobj.c asum.c atool.c
ar rcs liba.a *.o
gcc -ansi -pedantic -O3 -o atest *.o atest.c
