#include <stdio.h>

void __assert_fail (const char *assertion,
               const char *file, unsigned int line, const char *function)
{
	printf ( "__assert_fail %s %s %d %s\n", assertion, file, line, function );
}

