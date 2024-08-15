#ifndef SANCHO_PANZA_WINDOW_H
#define SANCHO_PANZA_WINDOW_H

#include "../../sancho-panza.h"

/* ================================================================ */

typedef struct window Window;

/* ================================================================ */

extern Window* Window_new(const char* title, int w, int h, Uint32 flags, Uint32 rflags);

/* ================================================================ */

extern int Window_destroy(Window**);

/* ================================================================ */

extern int Window_clear(const Window* w);

/* ================================================================ */

extern void Window_update(const Window* w);

/* ================================================================ */

extern int Window_set_HEX(Window* const w, Uint32 color, Uint8 alpha);

/* ================================================================ */

extern int Window_set_RGBA(Window* const w, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

/* ================================================================ */

#endif /* SANCHO_PANZA_WINDOW_H */
