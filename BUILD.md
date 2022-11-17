# build and use

the obj project has no release tags--the latest code is the latest release--i try to keep it compilable and runnable

to use--first get the source code

    git clone git@github.com:triangledirt/obj.git

next (before you build) edit [obj.h](https://github.com/triangledirt/obj/blob/main/obj.h) and set OBJ_VERBOSE and OBJ_XVERBOSE to either 0 or 1--obj is quiet with OBJ_VERBOSE==0 and noisy with OBJ_VERBOSE==1--when OBJ_XVERBOSE is 1 you get extra information printed to stdout

once you change those code settings--you can change some build settings--these are in [make](https://github.com/triangledirt/obj/blob/main/make)--change DEBUG to be "" if you dont want debug info--"-g" if you do want debug info and your compiler is gcc--change COMPILER to be whatever your compiler is--etc

now build--(from the case directory) execute

    ./make

that builds object files and a library called libobj.a--it requires only the c standard library

(to use obj in your project) do

    #include "[path to case directory]/class.h"

and/or

    #include "[path to case directory]/sync.h"
    #include "[path to case directory]/model.h"

depending on which areas of obj you need

(then you build your project) add

    -lcase -L[path to case directory]

to your compile command

see case.h and the rest of this document for the syntax of observation/classification functions and set-to-set inference functions
