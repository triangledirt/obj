# alib

## classify objects in real time

alib is a library of C functions used to classify objects in real time.

First you supply a series of objects each with binary attributes and a binary classification. You know the classes of these objects and you supply them to alib when you observe them.

Then with new objects whose classes you do not know, you ask alib to classify the objects, which it does with a 0 or a 1: a bit to let you know whether alib thinks the object is a member of the target set.

## statistical inference properties

alib also provides access to some statistical inference properties which you can use to calculate the likelihood, given the stream of objects you have observed, of a given indicator set having a specific relationship with a given target set.

For example indicator overlap (which is the same as conditional implication) shows the degree to which the indicator set does *indicator overlap* with respect to the target set: the degree to which the indicator set implies the target set.

These 14 properties are a fingerprint of the inference landscape. Their meanings are [explained](#infer-from-set-to-set) later.

## real time

alib doesn't use much memory or processing time. So it is real time.

It doesn't guarantee to produce the same result twice. It doesn't store its state when it's not running. There are no settings for you to mess with. No threads, disk access, network or database connections: just a small library to attach to your process.

alib does not profess to be perfect for any one task. It is not for critical tasks. alib is generalized object classification based on whatever methods we've chosen to implement at this time.

## build and use alib

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

If you want to re-use a type to mean another type, go ahead and do so. If you need to discontinue use of a type, do so.

## aobj_t

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

The 0-indexed attribute is the class attribute. You can set it in two ways:

    aobj_setattr(&obj, 0, class);
    aobj_setclass(&obj, class);

:and get it similarly:

    class = aobj_getattr(obj, 0);
    class = aobj_getclass(obj);

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

### indicator frequency

    indicator / target

This shows the rate at which the indicator set grows with respect to the growth of the target set. That is, the size or cardinality of the set of objects defined by *indicator* divided by the cardinality of the set defined by *target*.

If *indicator* and *target* grow at the same rate, indicator frequency will be 1. If *indicator* grows (or occurs) at twice the rate of *target*, indicator frequency will be 2. If *indicator* occurs at half the rate of *target*, indicator frequency will be 1/2. So this is the frequency with which the indicator occurs with respect to the target. When it is high, there are a lot of indicator objects for every target object. When it is low, there are a lot of target objects for every indicator object. This provides one aspect of the inference fingerprint for this set of objects.

### target frequency

    target / indicator

This is the target set as a part of the indicator set. The number of objects defined as members of the *target* set divided by the number of objects defined by *indicator*.

When this is below 1, there are many indicator objects per target object.

### indicator overlap

    intersection(indicator, target) / indicator

Indicator overlap is the same as conditional probability. indicator overlap is the degree to which *indicator* implies *target*.

### target overlap

    intersection(indicator, target) / target

Target overlap is the degree to which the target set is being indicated by, or targeted by, the indicator set. You can use this to construct sets that resist counterinference (by modifying your indicator set to minimize the value of this calculation).

### overlap

    intersection(indicator, target) / union(indicator, target)

When overlap is close to 0, the overlap between indicator and target is small. When overlap is close to 1, the overlap between indicator and target set is large. When close to 0, this means not much inference is possible but the quality of inferences will be high. When close to 1, this means lots of inference is possible but the quality of inferences will be low.

### indicator mismatch

    (indicator - target) / (target - indicator)

:is the degree to which the indicator set is unsuitable for inferring the target set.

### target mismatch

    (target - indicator) / (indicator - target)

:is the degree to which the target set is unsuitable for targeting the indicator set.

### indicator impertinence

    (indicator - target) / target

:the degree to which the indicator set is irrelevant or impertinent to the target set. A high number here means that almost all the indicator set is useless inferring the target set.

### target impertinence

    (target - indicator) / indicator

:the degree to which the target set is impertinent to the indicator set. A high number here means that almost all the target set is useless targeting the indicator set.

### indicator opacity

    (indicator - target) / intersection(indicator, target)

The greater the indicator opacity, the more opaque the indicator set. This means it doesn't reveal capacity for inference. The smaller the indicator opacity, the greater the capacity for inference.

### target opacity

    (target - indicator) / intersection(indicator, target)

The greater the target opacity, the more opaque the target set. This means it doesn't reveal capacity for targeting. The smaller the target opacity, the greater the capacity for targeting.

### indicator transparency

    intersection(indicator, target) / (indicator - target)

This can be thought of as an extent to which, or a thoroughness with which, the target set invades the indicator set. It can be thought of as a degree to which the target set disects the indicator set.

### target transparency

    intersection(indicator, target) / (target - indicator)

This can be thought of as an extent to which, or a thoroughness with which, the indicator set invades the target set. It can be thought of as a degree to which the indicator set disects the target set.

### transparency

    intersection(indicator, target) / xor(indicator, target)

Transparency is the ratio between the overlap and the xor of the indicator and target sets. As this approaches 0, the indicator and target sets are less and less related in terms of inference. As this approaches infinity, the indicator set and the target set are more and more related in terms of inference. As it approaches infinity, though, the indicator set and the target set are identical and inference between them is meaningless.
