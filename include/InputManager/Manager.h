#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "../../sancho-panza.h"

/* ================================================================ */

typedef struct input_manager {

    /* Array to hold the state of each key */
    unsigned char key_states[SDL_NUM_SCANCODES];
    unsigned char previous_key_states[SDL_NUM_SCANCODES];

    /* Left, Middle, Right mouse buttons */
    unsigned char mouse_BTN_states[3];
    unsigned char previous_mouse_BTN_states[3];

    /* Mouse/cursor position */
    int mouse_x;
    int mouse_y;

} Input_Manager;

/* ================================ */

extern void Input_update(App* application);

/* ================================================================ */

extern int Input_isKey_pressed(const App* application, SDL_Scancode key);

/* ================================================================ */

extern int Input_wasKey_just_pressed(const App* application, SDL_Scancode key);

/* ================================================================ */

extern int Input_isMouseBtn_pressed(const App* application, int btn);

/* ================================================================ */

extern int Input_wasBtn_just_pressed(const App* application, int btn);

/* ================================================================ */

#endif /* INPUT_MANAGER_H */
