#ifndef xdouble_h
#define xdouble_h

#define XDOUBLE_CNT 8

struct xdouble_t {
  double num[XDOUBLE_CNT];
  long notecnt;
  double avg;
};
typedef struct xdouble_t xdouble_t;

void xdouble_init(xdouble_t *xdouble);
void xdouble_note(xdouble_t *xdouble, double num);
double xdouble_avg(xdouble_t *xdouble);

#endif
