/***************************************************************************

		regexp.h

 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.

		TIME LIST:
		CREATE	Rainy	2009-04-15 21:13:40

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/
#ifndef _regexp_h_
#define _regexp_h_

#ifdef	__cplusplus
extern "C" {
#endif

#define NSUBEXP  10
typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *regcomp(const char *re);
extern int regexec(regexp *rp, const char *s);
extern void regsub(const regexp *rp, const char *src, char *dst);
extern void regerror(char *message);
extern void regfree(regexp *rp);

#ifdef __cplusplus
}
#endif

#endif


