#ifndef SANCHO_PANZA_WINDOW_H
#define SANCHO_PANZA_WINDOW_H

#include "../../sancho-panza.h"

/* ================================================================ */

typedef struct window* Window_t;

/* ================================ */

extern Window_t Window_new(const char* title, int w, int h, Uint32 flags, Uint32 rflags);

/* ================================ */

extern int Window_destroy(Window_t*);

/* ================================ */

extern int Window_clear(const Window_t w);

/* ================================ */

extern void Window_update(const Window_t w);

/* ================================ */

extern int Window_set_HEX(Window_t const w, Uint32 color, Uint8 alpha);

/* ================================ */

extern int Window_set_RGBA(Window_t const w, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

/* ================================================================ */

#endif /* SANCHO_PANZA_WINDOW_H */
