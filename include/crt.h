/*
 * Portable CRT definitions
 */

#ifndef _crt_h_
#define _crt_h_

extern char* portable_optarg;
extern int portable_optind, portable_opterr, portable_optopt;

char *portable_strcasestr(char *haystack, char *needle);
char* portable_strndup(const char *s, size_t n);
int portable_wcscasecmp(const wchar_t * s1, const wchar_t * s2);
int portable_strcasecmp(const char *s1, const char *s2);
int portable_getopt(int argc, char * const *argv, const char *optstring);

#define strcasestr portable_strcasestr
#define strndup portable_strndup
#define wcscasecmp portable_wcscasecmp
#define strcasecmp portable_strcasecmp
#define getopt portable_getopt

#define optarg portable_optarg
#define optind portable_optind
#define opterr portable_opterr
#define optopt portable_optopt

#endif
