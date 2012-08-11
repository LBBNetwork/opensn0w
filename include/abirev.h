/*
 * Module ABI revision
 */

#ifndef _abirev_h
#define _abirev_h

/*
 * Increment this in case of changes to the module ABI (i.e. changes
 * that require modules to be recompiled).
 * When incrementing, if the first two digits do not agree with the
 * current major.minor version (e.g. 3.3-devel), change the first two
 * digits and set the rest to 0 (e.g. 330000). Otherwise, increment
 * the lower digits.
 */
#define SN0W_ABIREV 200000

#endif
