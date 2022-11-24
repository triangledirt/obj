#ifndef obj_xdouble_h
#define obj_xdouble_h

#define OBJ_XDOUBLE_CNT 8

struct obj_xdouble_t {
  double num[OBJ_XDOUBLE_CNT];
  long notecnt;
  double avg;
};

void obj_xdouble_init(struct obj_xdouble_t *xdouble);
void obj_xdouble_note(struct obj_xdouble_t *xdouble, double num);
double obj_xdouble_avg(struct obj_xdouble_t *xdouble);

#endif
