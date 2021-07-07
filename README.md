# alib: classify objects in real time

alib lets you classify objects in real time. You supply a series of objects each with binary attributes and a binary classification. You know the classes of these objects and you supply them to alib when you observe them. Then with new objects whose classes you do not know, you ask alib to classify the objects, which it does with a 0 or a 1: a bit to let you know whether alib thinks the object is a member of the target set.

alib also provides access to some statistical inference properties which you can use to calculate the likelihood, given the stream of objects you have observed, of a given indicator set having a specific relationship with a given target set. For example indicator overlap (which is the same as conditional implication) shows the degree to which the indicator set does *indicator overlap* with respect to the target set: the degree to which the indicator set implies the target set. These 14 properties are a fingerprint of the inference landscape. Their meanings are explained later.

alib doesn't use much memory or processing time. So it is real time. It doesn't guarantee to produce the same result twice. It doesn't store its state when it's not running. There are no settings for you to mess with. No threads, disk access, network or database connections: just a small library. alib does not profess to be perfect for any one task. It is not for critical tasks. alib is generalized object classification based on whatever methods we've chosen to implement at this time.

## building and using alib

To build, get the source code, change to its directory and execute:

    ./make

That builds object files and a library. Include "[alib.h](https://github.com/triangledirt/alib/blob/main/alib.h)" in your project and see that file and the rest of this document for the syntax of observation/classification functions and set-to-set inference functions.

alib.h contains a #define called ALIB_VERBOSE. Set it to 1 to make noise when operating, 0 to be quiet.

## observe and classify objects

These two functions are used to observe and classify objects:

    void alib_observe(aobj_t obj, long type);
    abit_t alib_classify(aobj_t obj, long type);

Call alib_observe() on an aobj_t when you see it. Pass the type, which is a long you define to be one of 32 types.

    #define MUSHROOM 0
    #define GAME_MAP 1
    #define IMAGE_GOTHIC 2
    #define IMAGE_PRECAMBRIAN 3

You can do object observation on 32 types in any order.

    alib_observe(obj1, MUSHROOM);
    alib_observe(obj2, GAME_MAP);

So you can do object classification on 32 types in any order. If I'm observing a MUSHROOM-type aobj_t, I specify that when observing it. But your app can then alib_observe() an aobj_t that's a GAME_MAP type. When the time comes, you can classify new objects of unknown classification using the type parameter.

    c = alib_classify(obj1, MUSHROOM);
    d = alib_classify(obj2, GAME_MAP);

Call alib_observe() and alib_classify() as often and in any order you like.

aobj_ts are longs. [aobj.h](https://github.com/triangledirt/alib/blob/main/aobj.h) defines some ways to manipulate them. At base, you'll do this:

    aobj_clear(&obj);

:to initialize. And:

    aobj_setattr(&obj, idx, val);
    val = aobj_getattr(obj, idx);

:to set and get attributes. Index values go from 0 to 31. Bit 0 is the classification attribute. Say you're setting up a MUSHROOM object and you've decided to use bit 6 to represent whether the cap has spots on it. This says that the cap does:

    aobj_setattr(&obj, 6, 1);

This says the cap doesn't:

    aobj_setattr(&obj, 6, 0);

This sets attribute 7 to 0 in the object:

    aobj_setattr(&obj, 7, 0);

You don't have to set any particular bit. You don't have to set the class when you don't know it. You don't have to use all 32 bits. If you have an unknown, ignore it. Or set it with a random bit. Don't worry about cleaning up your data. alib likes it messy. If you're feeling daring, instead of initializing an aobj_t with aobj_clear(), do this:

    aobj_randomize(&obj);

Then set only the bits you are certain are a 0 or a 1 in only the attributes you know about, and give us a try!

See [atest.c](https://github.com/triangledirt/alib/blob/main/atest.c) for examples.

## infer from set to set

These 14 functions return inference properties of the set of objects alib is managing at this time. Call them once you've alib_observe()d at least one object.

    double alib_frequencyi(aobj_t indicator, aobj_t target, long type);
    double alib_frequencyt(aobj_t indicator, aobj_t target, long type);
    double alib_overlapi(aobj_t indicator, aobj_t target, long type);
    double alib_overlapt(aobj_t indicator, aobj_t target, long type);
    double alib_overlap(aobj_t indicator, aobj_t target, long type);
    double alib_mismatchi(aobj_t indicator, aobj_t target, long type);
    double alib_mismatcht(aobj_t indicator, aobj_t target, long type);
    double alib_impertinencei(aobj_t indicator, aobj_t target, long type);
    double alib_impertinencet(aobj_t indicator, aobj_t target, long type);
    double alib_opacityi(aobj_t indicator, aobj_t target, long type);
    double alib_opacityt(aobj_t indicator, aobj_t target, long type);
    double alib_transparencyi(aobj_t indicator, aobj_t target, long type);
    double alib_transparencyt(aobj_t indicator, aobj_t target, long type);
    double alib_transparency(aobj_t indicator, aobj_t target, long type);

Some of the names end with *i* or *t*. Those mean *indicator* or *target*. So frequencyi() returns the indicator frequency. frequencyt() returns the target frequency.

Each tages an indicator and a target, as well as the usual type parameter where you specify MUSHROOM, GAME_MAP, etc. The indicator and target are aobj_t types, but they are interpreted as a type mask which represents a set of objects. A 0 bit in these variables means that attribute field is not used in matching objects to the type. A 1 bit in these variables means an object must also have a 1 in that field in order to match the type.

[TODO: write the meanings of these functions]

### indicator frequency

### target frequency

### indicator overlap

### target overlap

### overlap

### indicator mismatch

### target mismatch

### indicator impertinence

### target impertinence

### indicator opacity

### target opacity

### indicator transparency

### target transparency

### transparency
