# alib: classify objects in real time

alib is an intelligence written in C that lets you classify objects in real time. You supply a series of objects each with binary properties and a binary classification. You know the classes of these objects and you supply them to alib when you observe them. Then with new objects whose classes you do not know, you ask alib to classify the objects, which it does with a 0 or a 1: a bit to let you know whether alib thinks the object is a member of the target set.

alib also provides access to some statistical inference properties which you can use to calculate the liklihood, given the stream of objects you are providing, of a given indicator set inferring a given target set. These functions return a liklihood in the form of *a fractional number of reasons* to believe that..

## building and using alib

To build, get the source code, change to its directory and execute:

    ./make

That builds object files and a library. Include "alib.h" in your project and see that file and the rest of this document for the syntax of observation/classification functions and set-to-set inference functions.

## observe and classify objects

## infer from set to set
