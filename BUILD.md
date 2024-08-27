# build and use

## contents

- [get the source](#get-the-source)
- [edit code settings](#edit-code-settings)
- [edit build settings](#edit-build-settings)
- [build](#build)
- [use](#use)

## get the source

the obj project has no release tags—the latest code is the latest release

to use—first get the source code

    git clone git@github.com:triangledirt/obj.git

## edit code settings

next (before you build) edit [obj.h](https://github.com/triangledirt/obj/blob/main/obj.h) and set OBJ_VERBOSE and OBJ_XVERBOSE to either 0 or 1

obj is quiet with OBJ_VERBOSE==0 and noisy with OBJ_VERBOSE==1

when OBJ_XVERBOSE is 1 you get extra information printed to stdout

## edit build settings

once you change those code settings—you can change some build settings—these are in [make](https://github.com/triangledirt/obj/blob/main/make)

change DEBUG to be "" if you dont want debug info—"-g" if you do want debug info and your compiler is gcc

change COMPILER to be whatever your compiler is—etc

## build

now build—(from the obj directory) execute

    ./make

that builds object files and a library called libobj.a—it requires only the c standard library

## use

(to use obj in your project) include any of the areas you want to use

    #include "[path to obj directory]/class.h"
    #include "[path to obj directory]/sync.h"
    #include "[path to obj directory]/model.h"

(then you build your project) add

    -lobj -L[path to obj directory]

to your compile command
