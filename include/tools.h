// Copyright 2010            Sven Peter <svenpeter@gmail.com>
// Copyright 2007,2008,2010  Segher Boessenkool  <segher@kernel.crashing.org>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#ifndef TOOLS_H__
#define TOOLS_H__ 1
#include <stdint.h>

#include "types.h"

void aes256cbc(u8 *key, u8 *iv, u8 *in, u64 len, u8 *out);
void aes256cbc_enc(u8 *key, u8 *iv, u8 *in, u64 len, u8 *out);
void aes128ctr(u8 *key, u8 *iv, u8 *in, u64 len, u8 *out);

void sha1(u8 *data, u32 len, u8 *digest);
void sha1_hmac(u8 *key, u8 *data, u32 len, u8 *digest);

#define		round_up(x,n)	(-(-(x) & -(n)))

#define		array_size(x)	(sizeof(x) / sizeof(*(x)))
#endif
