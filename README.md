# obj

## *in development !!*

*obj is in development--it is not ready to use !!*

# contents

classify
synchronize
model

## classify--synchronize--model

obj is pronounced *object*

it is a library of c functions used to (classify--synchronize--model) generalized binary data objects

its a local slim real-time system using no disk i/o--no network connections--no threading--no dynamic memory--the only dependency is the c standard library

## [obj_t](OBJ.md)

obj defines obj_t as a binary object--an object with binary attributes

then it lets you do things with binary objects--right now it lets you classify them--soon it may be able to synchronize and model them

read more about [obj_t](OBJ.md)

## [classify](CLASS.md)

classify lets you classify binary objects

first you supply a series of objects (each with binary attributes and a binary classification)--you know the classes of these objects and you supply them to classify when you observe them

then (with new objects whose classes you do not know) you ask classify to classify the objects--which it does with a 0 or a 1 (a bit to let you know whether classify thinks the object is a member of the target set)

read more about [classify](CLASS.md)

## [synchronize](SYNC.md)

coming soon !!

read more about [synchronize](SYNC.md)

## [model](MODEL.md)

coming soon !!

read more about [model](MODEL.md)

## [build and use](BUILD.md)

how to build and use obj in your project

getting the code--setting code options--compiling--linking

read more about how to [build and use](BUILD.md)
