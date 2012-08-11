// Copyright 2010       Sven Peter <svenpeter@gmail.com>
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
#ifndef TYPES_H__
#define TYPES_H__

#include <stdint.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct key {
	u8 key[32];
	u8 iv[16];

	int pub_avail;
	int priv_avail;
	u8 pub[40];
	u8 priv[21];
	u32 ctype;
};

struct keylist {
	u32 n;
	struct key *keys;
};

static inline u8 be8(u8 *p)
{
	return *p;
}

static inline u16 be16(u8 *p)
{
	u16 a;

	a  = p[0] << 8;
	a |= p[1];

	return a;
}

static inline u32 be32(u8 *p)
{
	u32 a;

	a  = p[0] << 24;
	a |= p[1] << 16;
	a |= p[2] <<  8;
	a |= p[3] <<  0;

	return a;
}

static inline u64 be64(u8 *p)
{
	u32 a, b;

	a = be32(p);
	b = be32(p + 4);

	return ((u64)a<<32) | b;
}

static inline void wbe16(u8 *p, u16 v)
{
	p[0] = v >>  8;
	p[1] = v;
}

static inline void wbe32(u8 *p, u32 v)
{
	p[0] = v >> 24;
	p[1] = v >> 16;
	p[2] = v >>  8;
	p[3] = v;
}

static inline void wbe64(u8 *p, u64 v)
{
	wbe32(p + 4, v);
	v >>= 32;
	wbe32(p, v);
}

#endif
