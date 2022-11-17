# obj_t

case_obj_ts are long longs (64 bits). [obj.h](https://github.com/triangledirt/case/blob/main/obj.h) defines some ways to manipulate them. At base, you'll do this

    case_obj_clear(&obj);

to initialize. And

    case_obj_setattr(&obj, idx, val);
    val = case_obj_getattr(obj, idx);

to set and get attributes. Index values go from 0 to 63. Bit 0 is the classification attribute. Say you're setting up a MUSHROOM object and you've decided to use bit 6 to represent whether the cap has spots on it. This says that the cap does

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

You don't have to set any particular bit. You don't have to set the class when you don't know it. You don't have to use all 64 bits. If you have an unknown, ignore it. Or set it with a random bit. Don't worry about cleaning up your data. case likes it messy. If you're feeling daring, instead of initializing a case_obj_t with case_obj_clear(), do this

    case_obj_randomize(&obj);

Then set only the bits you are certain are a 0 or a 1 in only the attributes you know about, and give us a try!

See [test.c](https://github.com/triangledirt/case/blob/main/test.c) for examples
