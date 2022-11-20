#ifndef obj_person_h
#define obj_person_h

#define OBJ_PERSON_CACHE 1024
#define OBJ_PERSON_TYPE OBJ

void obj_person_observe(obj_t obj, long type);
long obj_person_classify(obj_t obj, long type);

void obj_person_resetstat(long type);
obj_personstat_t *obj_person_stat(long type);

#endif
