# case

## classify objects in real time

case is a library of C functions used to classify objects in real time

First you supply a series of objects each with binary attributes and a binary classification. You know the classes of these objects and you supply them to case when you observe them

Then with new objects whose classes you do not know, you ask case to classify the objects, which it does with a 0 or a 1 (a bit to let you know whether case thinks the object is a member of the target set)

## statistical inference properties

case also provides access to some statistical inference properties which you can use to calculate the likelihood, given the stream of objects you have observed, of a given indicator set having a specific relationship with a given target set

For example indicator overlap (which is the same as conditional implication) shows the degree to which the indicator set does *indicator overlap* with respect to the target set (the degree to which the indicator set implies the target set)

These 14 properties are a fingerprint of the inference landscape. Their meanings are [explained](#infer-from-set-to-set) later

## real time

case doesn't use much memory or processor time. So it is real time

It is real time in the sense that observation and classification take place interleaved in time

case doesn't guarantee to produce the same result twice. It doesn't store its state when it's not running. There are no functional settings for you to mess with. No threads, disk access, network or database connections (just a small library to attach to your process)

case does not profess to be perfect for any one task. It is not for critical tasks. case is generalized object classification with statistical, genetic, and other methods under the hood

## build and use

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

## observe and classify

These two functions are used to observe and classify objects

    void case_observe(case_obj_t obj, long type);
    case_bit_t case_classify(case_obj_t obj, long type);

Call case_observe() on a case_obj_t when you see it. Pass the type, which is a long you define to be one of 32 types

    #define MUSHROOM 0
    #define GAME_MAP 1
    #define IMAGE_GOTHIC 2
    #define IMAGE_PRECAMBRIAN 3

You can observe objects of these 32 types in any order

    case_observe(obj1, MUSHROOM);
    case_observe(obj2, GAME_MAP);
    case_observe(obj3, IMAGE_GOTHIC);
    case_observe(obj4, GAME_MAP);

If you're observing a MUSHROOM-type case_obj_t, you specify that when observing it. But your app can then case_observe() a case_obj_t that's a GAME_MAP type

When the time comes, you can classify new objects of unknown classification using the type parameter

    c = case_classify(obj1, MUSHROOM);
    d = case_classify(obj2, GAME_MAP);

Call case_observe() and case_classify() as often and in any order you like

If you want to re-use a type to mean another type, go ahead and do so. If you need to discontinue use of a type, do so

## case_obj_t

case_obj_ts are longs. [obj.h](https://github.com/triangledirt/case/blob/main/obj.h) defines some ways to manipulate them. At base, you'll do this

    case_obj_clear(&obj);

to initialize. And

    case_obj_setattr(&obj, idx, val);
    val = case_obj_getattr(obj, idx);

to set and get attributes. Index values go from 0 to 31. Bit 0 is the classification attribute. Say you're setting up a MUSHROOM object and you've decided to use bit 6 to represent whether the cap has spots on it. This says that the cap does

    case_obj_setattr(&obj, 6, 1);

This says the cap doesn't

    case_obj_setattr(&obj, 6, 0);

This sets attribute 7 to 0 in the object

    case_obj_setattr(&obj, 7, 0);

The 0-indexed attribute is the class attribute. You can set it in two ways

    case_obj_setattr(&obj, 0, class);
    case_obj_setclass(&obj, class);

and get it similarly

    class = case_obj_getattr(obj, 0);
    class = case_obj_getclass(obj);

You don't have to set any particular bit. You don't have to set the class when you don't know it. You don't have to use all 32 bits. If you have an unknown, ignore it. Or set it with a random bit. Don't worry about cleaning up your data. case likes it messy. If you're feeling daring, instead of initializing a case_obj_t with case_obj_clear(), do this

    case_obj_randomize(&obj);

Then set only the bits you are certain are a 0 or a 1 in only the attributes you know about, and give us a try!

See [test.c](https://github.com/triangledirt/case/blob/main/test.c) for examples

## lens()

TODO: write

## infer from set to set

These 14 functions return inference properties of the set of objects case is managing at this time. Call them once you've case_observe()d at least one object

    double case_indifreq(case_obj_t indicator, case_obj_t target, long type);

The following 13 functions use the same parameters as above

    double case_targfreq(..);
    double case_indiover(..);
    double case_targover(..);
    double case_over(..);
    double case_indimis(..);
    double case_targmis(..);
    double case_indiimp(..);
    double case_targimp(..);
    double case_indiopac(..);
    double case_targopac(..);
    double case_inditrans(..);
    double case_targtrans(..);
    double case_trans(..);

Some of the names begin with *indi* or *targ*. Those mean indicator or target. *freq* means frequency, *over* means overlap, *mis* means mismatch, *imp* means impertinence, *opac* means opacity, and *trans* means transparency. So indifreq() returns the indicator frequency. targfreq() returns the target frequency. Etc

Each takes an indicator and a target, as well as the usual type parameter where you specify MUSHROOM, GAME_MAP, etc. The indicator and target are case_obj_t types, but they are interpreted as a type mask which represents a set of objects. A 0 bit in these variables means that attribute field is not used in matching objects to the type. A 1 bit in these variables means an object must also have a 1 in that field in order to match the type

### indicator frequency

    indicator / target

This shows the rate at which the indicator set grows with respect to the growth of the target set. That is, the size or cardinality of the set of objects defined by *indicator* divided by the cardinality of the set defined by *target*

If *indicator* and *target* grow at the same rate, indicator frequency will be 1. If *indicator* grows (or occurs) at twice the rate of *target*, indicator frequency will be 2. If *indicator* occurs at half the rate of *target*, indicator frequency will be 1/2. So this is the frequency with which the indicator occurs with respect to the target. When it is high, there are a lot of indicator objects for every target object. When it is low, there are a lot of target objects for every indicator object. This provides one aspect of the inference fingerprint for this set of objects

### target frequency

    target / indicator

This is the target set as a part of the indicator set. The number of objects defined as members of the *target* set divided by the number of objects defined by *indicator*

When this is below 1, there are many indicator objects per target object

### indicator overlap

    intersection(indicator, target) / indicator

Indicator overlap is the same as conditional probability. indicator overlap is the degree to which *indicator* implies *target*

### target overlap

    intersection(indicator, target) / target

Target overlap is the degree to which the target set is being indicated by, or targeted by, the indicator set. You can use this to construct sets that resist counterinference (by modifying your indicator set to minimize the value of this calculation)

### overlap

    intersection(indicator, target) / union(indicator, target)

When overlap is close to 0, the overlap between indicator and target is small. When overlap is close to 1, the overlap between indicator and target set is large. When close to 0, this means not much inference is possible but the quality of inferences will be high. When close to 1, this means lots of inference is possible but the quality of inferences will be low

### indicator mismatch

    (indicator - target) / (target - indicator)

is the degree to which the indicator set is unsuitable for inferring the target set

### target mismatch

    (target - indicator) / (indicator - target)

is the degree to which the target set is unsuitable for targeting the indicator set

### indicator impertinence

    (indicator - target) / target

the degree to which the indicator set is irrelevant or impertinent to the target set. A high number here means that almost all the indicator set is useless inferring the target set

### target impertinence

    (target - indicator) / indicator

the degree to which the target set is impertinent to the indicator set. A high number here means that almost all the target set is useless targeting the indicator set

### indicator opacity

    (indicator - target) / intersection(indicator, target)

The greater the indicator opacity, the more opaque the indicator set. This means it doesn't reveal capacity for inference. The smaller the indicator opacity, the greater the capacity for inference

### target opacity

    (target - indicator) / intersection(indicator, target)

The greater the target opacity, the more opaque the target set. This means it doesn't reveal capacity for targeting. The smaller the target opacity, the greater the capacity for targeting

### indicator transparency

    intersection(indicator, target) / (indicator - target)

This can be thought of as an extent to which, or a thoroughness with which, the target set invades the indicator set. It can be thought of as a degree to which the target set disects the indicator set

### target transparency

    intersection(indicator, target) / (target - indicator)

This can be thought of as an extent to which, or a thoroughness with which, the indicator set invades the target set. It can be thought of as a degree to which the indicator set disects the target set

### transparency

    intersection(indicator, target) / xor(indicator, target)

Transparency is the ratio between the overlap and the xor of the indicator and target sets. As this approaches 0, the indicator and target sets are less and less related in terms of inference. As this approaches infinity, the indicator set and the target set are more and more related in terms of inference. As it approaches infinity, though, the indicator set and the target set are identical and inference between them is meaningless

## contributing

- Follow existing naming and formatting style
- No dependencies outside the C standard library
- No disk access or network connections
- No malloc/free() or other heap use (outside of test.c)
- No const parameters
- No comments in completed code
- Thank you!
