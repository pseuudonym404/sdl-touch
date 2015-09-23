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

/* *INDENT-OFF* */

SDL_MIR_MODULE(MIR_CLIENT)
SDL_MIR_SYM(MirDisplayConfiguration*,mir_connection_create_display_config,(MirConnection *connection))
SDL_MIR_SYM(MirSurfaceSpec*,mir_connection_create_spec_for_normal_surface,(MirConnection* connection, int width, int height, MirPixelFormat format))
SDL_MIR_SYM(MirSurface*,mir_surface_create_sync,(MirSurfaceSpec *requested_specification))
SDL_MIR_SYM(void,mir_connection_get_available_surface_formats,(MirConnection* connection, MirPixelFormat* formats, unsigned const int format_size, unsigned int *num_valid_formats))
SDL_MIR_SYM(MirEGLNativeDisplayType,mir_connection_get_egl_native_display,(MirConnection *connection))
SDL_MIR_SYM(bool,mir_connection_is_valid,(MirConnection *connection))
SDL_MIR_SYM(void,mir_connection_release,(MirConnection *connection))
SDL_MIR_SYM(MirConnection *,mir_connect_sync,(char const *server, char const *app_name))
SDL_MIR_SYM(void,mir_display_config_destroy,(MirDisplayConfiguration* display_configuration))
SDL_MIR_SYM(char const *,mir_surface_get_error_message,(MirSurface *surface))
SDL_MIR_SYM(void,mir_surface_spec_set_buffer_usage,(MirSurfaceSpec *spec, MirBufferUsage usage))
SDL_MIR_SYM(void,mir_surface_get_parameters,(MirSurface *surface, MirSurfaceParameters *parameters))
SDL_MIR_SYM(bool,mir_surface_is_valid,(MirSurface *surface))
SDL_MIR_SYM(void,mir_surface_release_sync,(MirSurface *surface))
SDL_MIR_SYM(void,mir_surface_set_event_handler,(MirSurface *surface, mir_surface_event_callback callback, void *context))
SDL_MIR_SYM(MirWaitHandle*,mir_surface_set_state,(MirSurface *surface, MirSurfaceState state))
SDL_MIR_SYM(MirEventType,mir_event_get_type,(MirEvent const *ev))
SDL_MIR_SYM(MirInputEvent const*,mir_event_get_input_event,(MirEvent const *ev))
SDL_MIR_SYM(MirInputEventType,mir_input_event_get_type,(MirInputEvent const *ev))
SDL_MIR_SYM(MirBufferStream *,mir_surface_get_buffer_stream,(MirSurface *surface))
SDL_MIR_SYM(MirEGLNativeWindowType,mir_buffer_stream_get_egl_native_window,(MirBufferStream *buffer_stream))
SDL_MIR_SYM(void,mir_surface_spec_release,(MirSurfaceSpec *spec))
SDL_MIR_SYM(void,mir_buffer_stream_get_graphics_region,(MirBufferStream *buffer_stream, MirGraphicsRegion *graphics_region))
SDL_MIR_SYM(void,mir_buffer_stream_swap_buffers_sync,(MirBufferStream *buffer_stream))
SDL_MIR_SYM(MirKeyboardEvent const *,mir_input_event_get_keyboard_event,(MirInputEvent const *ev))
SDL_MIR_SYM(MirTouchEvent const *,mir_input_event_get_touch_event,(MirInputEvent const *ev))
SDL_MIR_SYM(MirPointerEvent const *,mir_input_event_get_pointer_event,(MirInputEvent const *ev))
SDL_MIR_SYM(MirResizeEvent const *,mir_event_get_resize_event,(MirEvent const *ev))
SDL_MIR_SYM(int,mir_resize_event_get_width,(MirResizeEvent const *ev))
SDL_MIR_SYM(int,mir_resize_event_get_height,(MirResizeEvent const *ev))
SDL_MIR_SYM(MirPointerAction,mir_pointer_event_action,(MirPointerEvent const *event))
SDL_MIR_SYM(MirPointerButtons,mir_pointer_event_buttons,(MirPointerEvent const *event))
SDL_MIR_SYM(float,mir_pointer_event_axis_value,(MirPointerEvent const *event, MirPointerAxis axis))
SDL_MIR_SYM(MirInputDeviceId,mir_input_event_get_device_id,(MirInputEvent const *ev))
SDL_MIR_SYM(unsigned int,mir_touch_event_point_count,(MirTouchEvent const *event))
SDL_MIR_SYM(MirTouchId,mir_touch_event_id,(MirTouchEvent const *event, size_t touch_index))
SDL_MIR_SYM(MirTouchAction,mir_touch_event_action,(MirTouchEvent const *event, size_t touch_index))
SDL_MIR_SYM(float,mir_touch_event_axis_value,(MirTouchEvent const *event, size_t touch_index, MirTouchAxis axis))
SDL_MIR_SYM(MirKeyboardAction,mir_keyboard_event_action,(MirKeyboardEvent const *event))
SDL_MIR_SYM(int,mir_keyboard_event_scan_code,(MirKeyboardEvent const *event))
SDL_MIR_SYM(xkb_keysym_t,mir_keyboard_event_key_code,(MirKeyboardEvent const *event))

SDL_MIR_MODULE(XKBCOMMON)
SDL_MIR_SYM(int,xkb_keysym_to_utf8,(xkb_keysym_t keysym, char *buffer, size_t size))

/* *INDENT-ON* */

/* vi: set ts=4 sw=4 expandtab: */
