# build and use

The case project has no release tags. The latest code is the latest release. I try to keep it compilable and runnable

To use, first get the source code

    git clone git@github.com:triangledirt/case.git

Next, before you build, edit [case.h](https://github.com/triangledirt/case/blob/main/case.h) and set CASE_VERBOSE and CASE_XVERBOSE to either 0 or 1. case is quiet with CASE_VERBOSE==0 and noisy with CASE_VERBOSE==1. When CASE_XVERBOSE is 1, you get extra information printed to stdout

Once you change those code settings, you can change some build settings. These are in [make](https://github.com/triangledirt/case/blob/main/make). Change DEBUG to be "" if you don't want debug info, "-g" if you do want debug info and your compiler is gcc. Change COMPILER to be whatever your compiler is. Etc

Now build. From the case directory, execute

    ./make

That builds object files and a library called libcase.a. It requires only the C standard library

To use case in your project, do

    #include "[path to case directory]/case.h"

When you build your project, add

    -lcase -L[path to case directory]

to your compile command

See case.h and the rest of this document for the syntax of observation/classification functions and set-to-set inference functions
