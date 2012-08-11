// Copyright 2010            Sven Peter <svenpeter@gmail.com>
// Copyright 2007,2008,2010  Segher Boessenkool  <segher@kernel.crashing.org>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#include "core.h"
#include "types.h"
#include "aes.h"
#include "sha1.h"

//
// crypto
//
void aes256cbc(u8 *key, u8 *iv_in, u8 *in, u64 len, u8 *out)
{
	AES_KEY k;
	u32 i;
	u8 tmp[16];
	u8 iv[16];

	memcpy(iv, iv_in, 16);
	memset(&k, 0, sizeof k);
	AES_set_decrypt_key(key, 256, &k);

	while (len > 0) {
		memcpy(tmp, in, 16);
		AES_decrypt(in, out, &k);

		for (i = 0; i < 16; i++)
			out[i] ^= iv[i];

		memcpy(iv, tmp, 16);

		out += 16;
		in += 16;
		len -= 16;

	}
}

void aes256cbc_enc(u8 *key, u8 *iv, u8 *in, u64 len, u8 *out)
{
	AES_KEY k;
	u32 i;
	u8 tmp[16];

	memcpy(tmp, iv, 16);
	memset(&k, 0, sizeof k);
	AES_set_encrypt_key(key, 256, &k);

	while (len > 0) {
		for (i = 0; i < 16; i++)
			tmp[i] ^= *in++;

		AES_encrypt(tmp, out, &k);
		memcpy(tmp, out, 16);

		out += 16;
		len -= 16;
	}
}

void aes128ctr(u8 *key, u8 *iv, u8 *in, u64 len, u8 *out)
{
	AES_KEY k;
	u32 i;
	u8 ctr[16];
	u64 tmp;

	memset(ctr, 0, 16);
	memset(&k, 0, sizeof k);

	AES_set_encrypt_key(key, 128, &k);

	for (i = 0; i < len; i++) {
		if ((i & 0xf) == 0) {
			AES_encrypt(iv, ctr, &k);

			// increase nonce
			tmp = be64(iv + 8) + 1;
			wbe64(iv + 8, tmp);
			if (tmp == 0)
				wbe64(iv, be64(iv) + 1);
		}
		*out++ = *in++ ^ ctr[i & 0x0f];
	}
}


// FIXME: use a non-broken sha1.c *sigh*
static void sha1_fixup(struct SHA1Context *ctx, u8 *digest)
{
	u32 i;

	for(i = 0; i < 5; i++) {
		*digest++ = ctx->Message_Digest[i] >> 24 & 0xff;
		*digest++ = ctx->Message_Digest[i] >> 16 & 0xff;
		*digest++ = ctx->Message_Digest[i] >> 8 & 0xff;
		*digest++ = ctx->Message_Digest[i] & 0xff;
	}
}

void sha1(u8 *data, u32 len, u8 *digest)
{
	struct SHA1Context ctx;

	SHA1Reset(&ctx);
	SHA1Input(&ctx, data, len);
	SHA1Result(&ctx);

	sha1_fixup(&ctx, digest);
}

void sha1_hmac(u8 *key, u8 *data, u32 len, u8 *digest)
{
	struct SHA1Context ctx;
	u32 i;
	u8 ipad[0x40];
	u8 tmp[0x40 + 0x14]; // opad + hash(ipad + message)

	SHA1Reset(&ctx);

	for (i = 0; i < sizeof ipad; i++) {
		tmp[i] = key[i] ^ 0x5c; // opad
		ipad[i] = key[i] ^ 0x36;
	}

	SHA1Input(&ctx, ipad, sizeof ipad);
	SHA1Input(&ctx, data, len);
	SHA1Result(&ctx);

	sha1_fixup(&ctx, tmp + 0x40);

	sha1(tmp, sizeof tmp, digest);

}

#ifdef _SUPER_TOOLS_
#error Equistie Tendencies is not supported on this platform. Sorry.
#endif
