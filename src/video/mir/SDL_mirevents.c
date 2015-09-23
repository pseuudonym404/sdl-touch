/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2015 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/*
  Contributed by Brandon Schaefer, <brandon.schaefer@canonical.com>
*/

#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_MIR

#include "../../events/SDL_events_c.h"
#include "../../events/SDL_keyboard_c.h"
#include "../../events/SDL_touch_c.h"
#include "../../events/scancodes_xfree86.h"

#include "SDL_mirevents.h"
#include "SDL_mirwindow.h"

#include <xkbcommon/xkbcommon.h>

#include "SDL_mirdyn.h"

static void HandleKeyPressEvent(Uint8 state, xkb_keysym_t keycode, int scancode, SDL_Window *sdl_window) {
    if (SDL_GetKeyboardFocus() != sdl_window)
        SDL_SetKeyboardFocus(sdl_window);
    if (scancode < SDL_arraysize(xfree86_scancode_table2))
        SDL_SendKeyboardKey(state, xfree86_scancode_table2[scancode]);
    if (state == SDL_PRESSED) {
        char text[8];
        int size = MIR_xkb_keysym_to_utf8(keycode, text, sizeof(text));
        if (size > 0) {
            text[size] = 0;
            SDL_SendKeyboardText(text);
        }
    }
}

static void HandleKeyEvent(MirKeyboardEvent const *ev, SDL_Window *sdl_window) {
    switch (MIR_mir_keyboard_event_action(ev)) {
        case mir_keyboard_action_up:
            HandleKeyPressEvent(SDL_RELEASED, MIR_mir_keyboard_event_key_code(ev), MIR_mir_keyboard_event_scan_code(ev), sdl_window);
            break;
        case mir_keyboard_action_down:
            HandleKeyPressEvent(SDL_PRESSED, MIR_mir_keyboard_event_key_code(ev), MIR_mir_keyboard_event_scan_code(ev), sdl_window);
            break;
        default:
            break;
    }
}

static void HandleTouchEvent(MirInputDeviceId id, MirTouchEvent const *ev, SDL_Window *sdl_window) {
    unsigned int i, numtouch = MIR_mir_touch_event_point_count(ev);
    SDL_AddTouch(id, "");
    for (i = 0; i < numtouch; ++i) {
        MirTouchId tid = MIR_mir_touch_event_id(ev, i);
        float x = MIR_mir_touch_event_axis_value(ev, i, mir_touch_axis_x) / sdl_window->w;
        float y = MIR_mir_touch_event_axis_value(ev, i, mir_touch_axis_y) / sdl_window->h;
        float pressure = MIR_mir_touch_event_axis_value(ev, i, mir_touch_axis_pressure);
        switch (MIR_mir_touch_event_action(ev, i)) {
            case mir_touch_action_up:
                SDL_SendTouch(id, tid, SDL_RELEASED, x, y, pressure);
                break;
            case mir_touch_action_down:
                SDL_SendTouch(id, tid, SDL_PRESSED, x, y, pressure);
                break;
            case mir_touch_action_change:
                SDL_SendTouchMotion(id, tid, x, y, pressure);
                break;
            default:
                break;
        }
    }
}

static MirInputDeviceId MouseIDs[8];
static MirPointerButtons LastButtons[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static int NumMice = 0;

static Uint32 GetMouseIndex(MirInputDeviceId id) {
    Uint32 i;
    for (i = 0; i < NumMice; ++i) if (MouseIDs[i] == id) return i;
    i = NumMice;
    MouseIDs[i] = id;
    if (NumMice < 7) NumMice++;
    return i;
}

static void HandlePointerButtonEvent(MirInputDeviceId id, MirPointerEvent const *ev, SDL_Window *sdl_window) {
    Uint32 index = GetMouseIndex(id);
    MirPointerButtons buttons = MIR_mir_pointer_event_buttons(ev);
    MirPointerButtons changed = buttons ^ LastButtons[index];
    MirPointerButtons pressed = buttons & changed;
    LastButtons[index] = buttons;

    if (changed & mir_pointer_button_primary)
        SDL_SendMouseButton(sdl_window, index, pressed & mir_pointer_button_primary ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_LEFT);
    if (changed & mir_pointer_button_secondary)
        SDL_SendMouseButton(sdl_window, index, pressed & mir_pointer_button_secondary ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_RIGHT);
    if (changed & mir_pointer_button_tertiary)
        SDL_SendMouseButton(sdl_window, index, pressed & mir_pointer_button_tertiary ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_MIDDLE);
    if (changed & mir_pointer_button_back)
        SDL_SendMouseButton(sdl_window, index, pressed & mir_pointer_button_back ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_X2);
    if (changed & mir_pointer_button_forward)
        SDL_SendMouseButton(sdl_window, index, pressed & mir_pointer_button_forward ? SDL_PRESSED : SDL_RELEASED, SDL_BUTTON_X1);
}

static void HandlePointerMotionEvent(MirInputDeviceId id, MirPointerEvent const *ev, SDL_Window *sdl_window) {
    Uint32 index = GetMouseIndex(id);
    float x = MIR_mir_pointer_event_axis_value(ev, mir_pointer_axis_x);
    float y = MIR_mir_pointer_event_axis_value(ev, mir_pointer_axis_y);
    float vscroll = MIR_mir_pointer_event_axis_value(ev, mir_pointer_axis_vscroll);
    float hscroll = MIR_mir_pointer_event_axis_value(ev, mir_pointer_axis_hscroll);
    SDL_SendMouseMotion(sdl_window, index, SDL_FALSE, x, y);
    SDL_SendMouseWheel(sdl_window, index, hscroll, vscroll, SDL_MOUSEWHEEL_NORMAL);
}

static void HandlePointerEvent(MirInputDeviceId id, MirPointerEvent const *ev, SDL_Window *sdl_window) {
    SDL_SetMouseFocus(sdl_window);

    switch(MIR_mir_pointer_event_action(ev)) {
        case mir_pointer_action_button_up:
        case mir_pointer_action_button_down:
            HandlePointerButtonEvent(id, ev, sdl_window);
            break;
        case mir_pointer_action_enter:
            break;
        case mir_pointer_action_leave:
            SDL_SetMouseFocus(NULL);
            break;
        case mir_pointer_action_motion:
            HandlePointerMotionEvent(id, ev, sdl_window);
            break;
        default:
            break;
    }
}

static void HandleInputEvent(MirInputEvent const *ev, SDL_Window *sdl_window) {
    switch (MIR_mir_input_event_get_type(ev)) {
        case mir_input_event_type_key:
            HandleKeyEvent(MIR_mir_input_event_get_keyboard_event(ev), sdl_window);
            break;
        case mir_input_event_type_touch:
            HandleTouchEvent(MIR_mir_input_event_get_device_id(ev), MIR_mir_input_event_get_touch_event(ev), sdl_window);
            break;
        case mir_input_event_type_pointer:
            HandlePointerEvent(MIR_mir_input_event_get_device_id(ev), MIR_mir_input_event_get_pointer_event(ev), sdl_window);
            break;
    }
}

static void HandleResizeEvent(MirResizeEvent const *ev, SDL_Window *sdl_window) {
    SDL_SendWindowEvent(sdl_window, SDL_WINDOWEVENT_RESIZED, MIR_mir_resize_event_get_width(ev), MIR_mir_resize_event_get_height(ev));
}

void MIR_HandleEvent(MirSurface* surface, MirEvent const* ev, void* context) {
    SDL_Window* window = (SDL_Window*)context;
    switch (MIR_mir_event_get_type(ev)) {
        case mir_event_type_input:
            HandleInputEvent(MIR_mir_event_get_input_event(ev), window);
            break;
        case (mir_event_type_resize):
            HandleResizeEvent(MIR_mir_event_get_resize_event(ev), window);
            break;
        default:
            break;
    }
}

#endif /* SDL_VIDEO_DRIVER_MIR */

/* vi: set ts=4 sw=4 expandtab: */
