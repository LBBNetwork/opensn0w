/*
 * Pretty version header <3
 */

#ifndef _version_h_
#define _version_h_


#define __SN0W_VERSION__ "opensn0w-" PACKAGE_VERSION

#ifndef RELEASE
#define __SN0W_CONFIG__ "DEVELOPMENT"
#else
#define __SN0W_CONFIG__ "RELEASE"
#endif

/* fun here */
#ifdef _MSC_VER
#ifdef _M_X64
#elif defined _M_IX86
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_I386(msvc)"
#elif defined _M_IA64
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_IA64(msvc)"
#elif defined _M_MPPC
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_PPC(msvc)"
#elif defined _M_MRX000
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_MIPS(msvc)"
#else
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_GENERIC(msvc)"
#endif	/* end msvc */
#elif defined __clang__
#ifdef __alpha__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_ALPHA(clang)"
#elif defined __x86_64__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_X86_64(clang)"
#elif defined __arm__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_ARM(clang)"
#elif defined __thumb__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_THUMB(clang)"
#elif defined __convex__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_CONVEX(clang)"
#elif defined __hppa__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_HPPA(clang)"
#elif defined __i386__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_I386(clang)"
#elif defined __ia64__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_IA64(clang)"
#elif defined __m68k__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_M68K(clang)"
#elif defined __mips__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_MIPS(clang)"
#elif defined __powerpc__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_POWERPC(clang)"
#elif defined __sparc__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_SPARC(clang)"
#elif defined __sh__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_SUPERH(clang)"
#elif defined __s390__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_S390(clang)"
#endif
#elif defined __GNUC__
#ifdef __alpha__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_ALPHA(gcc)"
#elif defined __x86_64__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_X86_64(gcc)"
#elif defined __arm__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_ARM(gcc)"
#elif defined __thumb__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_THUMB(gcc)"
#elif defined __convex__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_CONVEX(gcc)"
#elif defined __hppa__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_HPPA(gcc)"
#elif defined __i386__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_I386(gcc)"
#elif defined __ia64__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_IA64(gcc)"
#elif defined __m68k__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_M68K(gcc)"
#elif defined __mips__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_MIPS(gcc)"
#elif defined __powerpc__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_POWERPC(gcc)"
#elif defined __sparc__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_SPARC(gcc)"
#elif defined __sh__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_SUPERH(gcc)"
#elif defined __s390__
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_S390(gcc)"
#endif
#else	/* end supported compilers */
#define __SN0W_VERSION_FULL__ __SN0W_VERSION__ "/" __SN0W_CONFIG__ "_GENERIC(unknown)"
#endif

#endif
