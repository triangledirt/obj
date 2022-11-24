# classify

## contents

- [about](#about)
- [infer from set to set](#infer-from-set-to-set)
- [observe and classify](#observe-and-classify)
- [pack csv objects](#pack-csv-objects)

## about

classify is an area of obj used to classify objects

first you supply a series of objects each with binary attributes and a binary classification--you know the classes of these objects and you supply them to case when you observe them

then with new objects (whose classes you do not know) you ask obj to classify the objects--which it does with a 0 or a 1 (a bit to let you know whether obj thinks the object is a member of the target set)

## infer from set to set

obj also provides access to some statistical inference properties which you can use to calculate the likelihood (given the stream of objects you have observed) of an indicator set having a specific relationship with a target set

(for example) indicator overlap (which is the same as conditional implication) shows the degree to which the indicator set does *indicator overlap* with respect to the target set (the degree to which the indicator set implies the target set)

these 14 properties are a fingerprint of the inference landscape

read more about [inferring from set to set](INFER.md)

## observe and classify

these two functions are used to observe and classify objects

    void obj_class_observe(obj_t obj, long type);
    obj_bit_t obj_class_classify(obj_t obj, long type);

call obj_class_observe() on an obj_t when you see it--pass the type (which is a long you define to be one of 64 types)

    #define MUSHROOM 0
    #define GAME_MAP 1
    #define IMAGE_GOTHIC 2
    #define IMAGE_PRECAMBRIAN 3

you can observe objects of these 64 types in any order

    obj_class_observe(obj1, MUSHROOM);
    obj_class_observe(obj2, GAME_MAP);
    obj_class_observe(obj3, IMAGE_GOTHIC);
    obj_class_observe(obj4, GAME_MAP);

if youre observing a MUSHROOM-type obj_t--you specify that when observing it--but your app can then obj_class_observe() an obj_t thats a GAME_MAP type

(when the time comes) you can classify new objects of unknown classification using the type parameter

    c = obj_class_classify(obj1, MUSHROOM);
    d = obj_class_classify(obj2, GAME_MAP);

call obj_class_observe() and obj_class_classify() as often and in any order you like

if you want to re-use a type to mean another type--go ahead and do so--if you need to discontinue use of a type--do so

## pack csv objects

TODO: write
