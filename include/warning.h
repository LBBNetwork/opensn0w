/*
 * ODIOUS HEINOUS PIECE OF SHIT COMPILER
 */

#ifndef _warning_h_
#define _warning_h_

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
#pragma warning(disable:4706)	/* assignment within cond expression */
#pragma warning(disable:4702)	/* unreachable code */
#pragma warning(disable:4701)	/* *potentially* unused variable used? */
#endif

#endif
