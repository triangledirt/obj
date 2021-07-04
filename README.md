# alib---classify objects in real time

alib is an intelligence written in C that lets you classify objects in real time. You supply a series of objects each with binary properties. You know the classes of these objects and you supply them to alib when you observe them. Then with new objects whose classes you do not know, you ask alib to classify the objects, which it does with a 0 or a 1--a bit to let you know whether alib thinks the object is a member of the target set.

alib also provides access to some statistical inference properties which you can use to calculate the liklihood, given the stream of objects you are providing, of a given indicator set inferring a given target set. These functions return a liklihood in the form of *a fractional number of reasons* to believe that..

## observe and classify objects

## infer from set to set
