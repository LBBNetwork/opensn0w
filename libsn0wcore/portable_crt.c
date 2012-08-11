/*
 * Portable C Functions
 */

#include "core.h"

char* portable_optarg;
int portable_optind = 1, portable_opterr, portable_optopt;
static const char *__portable__optptr;

char *portable_strcasestr(char *haystack, char *needle)
{
	char *p, *startn = 0, *np = 0;

	for (p = haystack; *p; p++) {
		if (np) {
			if (toupper(*p) == toupper(*np)) {
				if (!*++np)
					return startn;
			} else
				np = 0;
		} else if (toupper(*p) == toupper(*needle)) {
			np = needle + 1;
			startn = p;
		}
	}

	return 0;
}

char* portable_strndup(const char *s, size_t n)
{
	char *result;
	size_t len = strlen(s);

	if (n < len)
		len = n;

	result = (char *)malloc(len + 1);
	if (!result)
		return 0;

	result[len] = '\0';
	return (char *)memcpy(result, s, len);
}

int portable_wcscasecmp(const wchar_t * s1, const wchar_t * s2)
{
	wchar_t	l1, l2;

	while ((l1 = towlower(*s1++)) == (l2 = towlower(*s2++))) {
		if (l1 == 0)
			return (0);
	}

	return ((wchar_t) l1 - (wchar_t) l2);
}

int portable_strcasecmp(const char *s1, const char *s2)
{
	while (*s1 != '\0' && tolower(*s1) == tolower(*s2)) {
		s1++;
		s2++;
	}

	return tolower(*(unsigned char *)s1) - tolower(*(unsigned char *)s2);
}


int portable_getopt(int argc, char * const *argv, const char *optstring)
{
  const char *carg = argv[portable_optind];
  const char *osptr;
  int opt;

  /* We don't actually need argc */
  (void)argc;

  /* First, eliminate all non-option cases */

  if ( !carg || carg[0] != '-' || !carg[1] ) {
    return -1;
  }

  if ( carg[1] == '-' && !carg[2] ) {
    portable_optind++;
    return -1;
  }

  if ( (uintptr_t)(__portable__optptr-carg) > (uintptr_t)strlen(carg) )
    __portable__optptr = carg+1;	/* Someone frobbed optind, change to new opt. */

  opt = *__portable__optptr++;

  if ( opt != ':' && (osptr = strchr(optstring, opt)) ) {
    if ( osptr[1] == ':' ) {
      if ( *__portable__optptr ) {
	/* Argument-taking option with attached argument */
	portable_optarg = (char *)__portable__optptr;
	portable_optind++;
      } else {
	/* Argument-taking option with non-attached argument */
	if ( argv[portable_optind+1] ) {
	  portable_optarg = (char *)argv[portable_optind+1];
	  portable_optind += 2;
	} else {
	  /* Missing argument */
	  return (optstring[0] == ':') ? ':' : '?';
	}
      }
      return opt;
    } else {
      /* Non-argument-taking option */
      /* __optptr will remember the exact position to resume at */
      if ( ! *__portable__optptr )
	portable_optind++;
      return opt;
    }
  } else {
    /* Unknown option */
    portable_optopt = opt;
    if ( ! *__portable__optptr )
      portable_optind++;
    return '?';
  }
}
