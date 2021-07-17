DEBUG=
gcc $DEBUG -O3 -c acore.c afold.c agene.c ajung.c alib.c aobj.c asum.c atool.c
ar rcs liba.a *.o
gcc $DEBUG -O3 -o atest *.o atest.c
