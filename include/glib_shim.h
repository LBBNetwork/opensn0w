/*
 * GLIB shimmy thing
 */

#ifndef _glib_shim_h_
#define _glib_shim_h_

#define return_val_if_fail(expr, val) if (!(expr)) {printf("[%s] - expr '%s' failed", __FUNCTION__, #expr); return val;};
#define return_if_fail(expr) if (!(expr)) {printf("[%s] - expr '%s' failed", __FUNCTION__, #expr); return;};

#endif
