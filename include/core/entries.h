#ifndef MAPPING_ENTRIES_H
#define MAPPING_ENTRIES_H

#include "../../sancho-panza.h"

/* ================================================================ */

struct mapping_entry {
    const char* name;
    Uint32 flag;
};

/* ================================================================ */
/* ======================== SDL_Init Flags ======================== */
/* ================================================================ */

struct mapping_entry SDL_Init_Flags[] = {
    {"SDL_INIT_TIMER", SDL_INIT_TIMER},
    {"SDL_INIT_AUDIO", SDL_INIT_AUDIO},
    {"SDL_INIT_VIDEO", SDL_INIT_VIDEO},
    {"SDL_INIT_JOYSTICK", SDL_INIT_JOYSTICK},
    {"SDL_INIT_HAPTIC", SDL_INIT_HAPTIC},
    {"SDL_INIT_GAMECONTROLLER", SDL_INIT_GAMECONTROLLER},
    {"SDL_INIT_EVENTS", SDL_INIT_EVENTS},
    {"SDL_INIT_EVERYTHING", SDL_INIT_EVERYTHING},
    {"SDL_INIT_NOPARACHUTE", SDL_INIT_NOPARACHUTE}
};

/* ================================================================ */
/* ==================== SDL_CreateWindow Flags ==================== */
/* ================================================================ */

struct mapping_entry SDL_CreateWindow_Flags[] = {
    {"SDL_WINDOW_FULLSCREEN", SDL_WINDOW_FULLSCREEN},
    {"SDL_WINDOW_OPENGL", SDL_WINDOW_OPENGL},
    {"SDL_WINDOW_SHOWN", SDL_WINDOW_SHOWN},
    {"SDL_WINDOW_HIDDEN", SDL_WINDOW_HIDDEN},
    {"SDL_WINDOW_BORDERLESS", SDL_WINDOW_BORDERLESS},
    {"SDL_WINDOW_RESIZABLE", SDL_WINDOW_RESIZABLE},
    {"SDL_WINDOW_MINIMIZED", SDL_WINDOW_MAXIMIZED},
    {"SDL_WINDOW_MOUSE_GRABBED", SDL_WINDOW_MOUSE_GRABBED},
    {"SDL_WINDOW_INPUT_FOCUS", SDL_WINDOW_INPUT_FOCUS},
    {"SDL_WINDOW_MOUSE_FOCUS", SDL_WINDOW_MOUSE_FOCUS},
    {"SDL_WINDOW_FULLSCREEN_DESKTOP", ( SDL_WINDOW_FULLSCREEN | 0x00001000 )},
    {"SDL_WINDOW_FOREIGN", SDL_WINDOW_FOREIGN},
    {"SDL_WINDOW_ALLOW_HIGHDPI", SDL_WINDOW_ALLOW_HIGHDPI},
    {"SDL_WINDOW_MOUSE_CAPTURE", SDL_WINDOW_MOUSE_CAPTURE},
    {"SDL_WINDOW_ALWAYS_ON_TOP", SDL_WINDOW_ALWAYS_ON_TOP},
    {"SDL_WINDOW_SKIP_TASKBAR", SDL_WINDOW_SKIP_TASKBAR},
    {"SDL_WINDOW_UTILITY", SDL_WINDOW_UTILITY},
    {"SDL_WINDOW_TOOLTIP", SDL_WINDOW_TOOLTIP},
    {"SDL_WINDOW_POPUP_MENU", SDL_WINDOW_POPUP_MENU},
    {"SDL_WINDOW_KEYBOARD_GRABBED", SDL_WINDOW_KEYBOARD_GRABBED},
    {"SDL_WINDOW_VULKAN", SDL_WINDOW_VULKAN},
    {"SDL_WINDOW_METAL", SDL_WINDOW_METAL},
    {"SDL_WINDOW_INPUT_GRABBED", SDL_WINDOW_MOUSE_GRABBED}
};

/* ================================================================ */
/* =================== SDL_CreateRenderer Flags =================== */
/* ================================================================ */

struct mapping_entry SDL_CreateRenderer_flags[] = {
    {"SDL_RENDERER_SOFTWARE", SDL_RENDERER_SOFTWARE},
    {"SDL_RENDERER_ACCELERATED", SDL_RENDERER_ACCELERATED},
    {"SDL_RENDERER_PRESENTVSYNC", SDL_RENDERER_PRESENTVSYNC},
    {"SDL_RENDERER_TARGETTEXTURE", SDL_RENDERER_TARGETTEXTURE}
};

/* ================================================================ */

#endif /* MAPPING_ENTRIES_H */
