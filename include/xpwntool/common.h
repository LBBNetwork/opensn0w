#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#define fseeko fseeko64
#define ftello ftello64
#define off_t off64_t
#define mkdir(x, y) mkdir(x)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#define TRUE 1
#define FALSE 0

#define FLIPENDIAN(x) flipEndian((unsigned char *)(&(x)), sizeof(x))
#define FLIPENDIANLE(x) flipEndianLE((unsigned char *)(&(x)), sizeof(x))

#define IS_BIG_ENDIAN      0
#define IS_LITTLE_ENDIAN   1

#define TIME_OFFSET_FROM_UNIX 2082844800L
#define APPLE_TO_UNIX_TIME(x) ((x) - TIME_OFFSET_FROM_UNIX)
#define UNIX_TO_APPLE_TIME(x) ((x) + TIME_OFFSET_FROM_UNIX)

#define ASSERT(x, m) if(!(x)) { fflush(stdout); fprintf(stderr, "error: %s\n", m); perror("error"); fflush(stderr); exit(1); }

extern char endianness;

#ifndef LIBPARTIAL_H
#ifdef _MSC_VER
#define inline	__forceinline	/* damn you microsoft */
#define off64_t size_t 
#undef fseeko
#define fseeko fseek
#undef ftello
#define ftello ftell
#ifdef _MSC_VER
/*
 * evil hacks 
 */
#pragma warning(disable:4131)	/* k&r c style used */
#pragma warning(disable:4003)	/* not enough actual parameters for macro */
#pragma warning(disable:4127)	/* conditional expression is always constant */
#pragma warning(disable:4214)	/* unknown nonstandard extension */
#pragma warning(disable:4200)	/* unknown nonstandard extension */
#pragma warning(disable:4201)	/* unknown nonstandard extension */
#pragma warning(disable:4204)	/* unknown nonstandard extension */
#pragma warning(disable:4018)	/* signed/unsigned mismatch */
#pragma warning(disable:4245)	/* signed/unsigned mismatch */
#pragma warning(disable:4242)	/* integer conversion */
#pragma warning(disable:4244)	/* integer conversion */
#pragma warning(disable:4005)	/* macro redefinition */
#pragma warning(disable:4702)	/* unreachable code */
#pragma warning(disable:4706)	/* assignment within cond expression */
#pragma warning(disable:4701)	/* *potentially* unused variable used? */
#endif
#pragma warning(disable:4100)	/* unreferenced formal parameter */
#endif
#endif

#ifndef LIBPARTIAL_H



static inline void flipEndian(unsigned char* x, int length) {
  int i;
  unsigned char tmp;

  if(endianness == IS_BIG_ENDIAN) {
    return;
  } else {
    for(i = 0; i < (length / 2); i++) {
      tmp = x[i];
      x[i] = x[length - i - 1];
      x[length - i - 1] = tmp;
    }
  }
}

static inline void flipEndianLE(unsigned char* x, int length) {
  int i;
  unsigned char tmp;

  if(endianness == IS_LITTLE_ENDIAN) {
    return;
  } else {
    for(i = 0; i < (length / 2); i++) {
      tmp = x[i];
      x[i] = x[length - i - 1];
      x[length - i - 1] = tmp;
    }
  }
}

static inline void hexToBytes(const char* hex, uint8_t** buffer, size_t* bytes) {
	size_t i;	/* note: msvc likes things being here */
	*bytes = strlen(hex) / 2;
	*buffer = (uint8_t*) malloc(*bytes);
	for(i = 0; i < *bytes; i++) {
		uint32_t byte;
		sscanf(hex, "%2x", &byte);
		(*buffer)[i] = byte;
		hex += 2;
	}
}

static inline void hexToInts(const char* hex, unsigned int** buffer, size_t* bytes) {
	size_t i;
	*bytes = strlen(hex) / 2;
	*buffer = (unsigned int*) malloc((*bytes) * sizeof(int));
	for(i = 0; i < *bytes; i++) {
		sscanf(hex, "%2x", &((*buffer)[i]));
		hex += 2;
	}
}

struct io_func_struct;

typedef int (*readFunc)(struct io_func_struct* io, off_t location, size_t size, void *buffer);
typedef int (*writeFunc)(struct io_func_struct* io, off_t location, size_t size, void *buffer);
typedef void (*closeFunc)(struct io_func_struct* io);

typedef struct io_func_struct {
  void* data;
  readFunc read;
  writeFunc write;
  closeFunc close;
} io_func;
#endif

struct AbstractFile;

unsigned char* decodeBase64(char* toDecode, size_t* dataLength);
void writeBase64(struct AbstractFile* file, unsigned char* data, size_t dataLength, int tabLength, int width);
char* convertBase64(unsigned char* data, size_t dataLength, int tabLength, int width);

#endif
