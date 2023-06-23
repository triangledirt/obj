# obj_t

obj_ts are long longs (64 bits)—[obj.h](https://github.com/triangledirt/obj/blob/main/obj.h) defines some ways to manipulate them—at base youll do this

    obj_clear(&obj);

to initialize—and

    obj_setattr(&obj, idx, val);
    val = obj_getattr(obj, idx);

to set and get attributes—index values go from 0 to 63—bit 0 is the classification attribute—say youre setting up a MUSHROOM object and youve decided to use bit 6 to represent whether the cap has spots on it—this says that the cap does

    obj_setattr(&obj, 6, 1);

this says the cap doesnt

    obj_setattr(&obj, 6, 0);

this sets attribute 7 to 0 in the object

    obj_setattr(&obj, 7, 0);

the 0-indexed attribute is the class attribute—you can set it in two ways

    obj_setattr(&obj, 0, class);
    obj_setclass(&obj, class);

and get it similarly

    class = obj_getattr(obj, 0);
    class = obj_getclass(obj);

you dont have to set any particular bit—you dont have to set the class when you dont know it—you dont have to use all 64 bits—if you have an unknown—ignore it—or set it with a random bit—dont worry about cleaning up your data—obj likes it messy—if youre feeling daring (instead of initializing an obj_t with obj_clear()) do this

    obj_randomize(&obj);

then set only the bits you are certain are a 0 or a 1 in only the attributes you know about—and give us a try !!

see [test.c](https://github.com/triangledirt/obj/blob/main/test.c) for examples
