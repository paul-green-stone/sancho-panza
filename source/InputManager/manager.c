#include "../../sancho-panza.h"

/* ================================================================ */

void Input_update(App* application) {

    /* Store current key states in previous states */
    memcpy(application->imanager.previous_key_states, application->imanager.key_states, sizeof(application->imanager.key_states));

    /* Update current key states */
    const Uint8* current_state = SDL_GetKeyboardState(NULL);
    memcpy(application->imanager.key_states, current_state, sizeof(application->imanager.key_states));

    memcpy(application->imanager.previous_mouse_BTN_states, application->imanager.mouse_BTN_states, sizeof(application->imanager.mouse_BTN_states));

    /* Update mouse states */
    application->imanager.mouse_BTN_states[0] = SDL_GetMouseState(&application->imanager.mouse_x, &application->imanager.mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT);
    application->imanager.mouse_BTN_states[1] = SDL_GetMouseState(&application->imanager.mouse_x, &application->imanager.mouse_y) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    application->imanager.mouse_BTN_states[2] = SDL_GetMouseState(&application->imanager.mouse_x, &application->imanager.mouse_y) & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

/* ================================================================ */

int Input_isKey_pressed(const App* application, SDL_Scancode key) {

    if (application == NULL) {
        return 0;
    }

    return application->imanager.key_states[key];
}

/* ================================================================ */

int Input_wasKey_just_pressed(const App* application, SDL_Scancode key) {
    
    if (application == NULL) {
        return 0;
    }

    return application->imanager.key_states[key] && !application->imanager.previous_key_states[key];
}

/* ================================================================ */

int Input_isMouseBtn_pressed(const App* application, int btn) {

    if (application == 0) {
        return 0;
    }

    if ((btn < 0) || (btn > 2)) {
        return -1;
    }

    return application->imanager.mouse_BTN_states[btn];
}

/* ================================================================ */

int Input_wasBtn_just_pressed(const App* application, int btn) {

    if (application == 0) {
        return 0;
    }

    if ((btn < 0) || (btn > 2)) {
        return -1;
    }

    return application->imanager.mouse_BTN_states[btn] && !application->imanager.previous_mouse_BTN_states[btn];
}

/* ================================================================ */
