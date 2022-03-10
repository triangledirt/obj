#ifndef wad_h
#define wad_h

struct wad_t;
typedef struct wad_t wad_t;

void wad_update(wad_t *wad, char *str);
char *wad_popular(wad_t *wad);

#endif
