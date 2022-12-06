# obj

## *in development*

*obj is in development—it is not ready to use*

## contents

- [about](#about)
- [obj_t](#obj_t)
- [areas](#areas)
  - [classify](#classify)
  - [model](#model)
  - [sense](#sense)
  - [synchronize](#synchronize)
- [build and use](#build-and-use)

## about

obj is a library of c functions used to (classify—synchronize—model) (and do other things to) generalized binary data objects

its a local slim real-time system using no disk i/o—no network connections—no threading—no dynamic memory—the only dependency is the c standard library

## obj_t

obj defines obj_t as a binary object—an object with binary attributes

then it defines interfaces in some functional areas (classify—synchronize—etc)

read more about [obj_t](OBJ.md)

## areas

obj presents interfaces in these areas

### classify

classify lets you classify binary objects

first you supply a series of objects (each with binary attributes and a binary classification)—you know the classes of these objects and you supply them to classify when you observe them

then (with new objects whose classes you do not know) you ask classify to classify the objects—which it does with a 0 or a 1 (a bit to let you know whether classify thinks the object is a member of the target set)

read more about [classify](CLASS.md)

### model

model lets you grow a population of objects inside a self-partitioning geography—an obj model is like a zoo where objects interact conversationally to produce new objects following your fitness function

you insert objects into the model—define a fitness function—then pull the best objects out of the model and use them to solve your problem

model is a cultural evolutionary system (or a genetic algorithm with inherent organic partitioning)

read more about [model](MODEL.md)

### sense

sense shows you which parts of an object to pay attention to

first you observe a series of objects

then ask sense to tell you which bits of the object are telling you something

read more about [sense](SENSE.md)

### synchronize

synchronize lets you schedule group and otherwise organize objects into sets of various types

it can be used to synchronize tasks sharing resources—group people into personality types—and ..

read more about [synchronize](SYNC.md)

## build and use

this is how to build and use obj in your project

getting the code—setting code options—compiling—linking

read more about how to [build and use](BUILD.md)
