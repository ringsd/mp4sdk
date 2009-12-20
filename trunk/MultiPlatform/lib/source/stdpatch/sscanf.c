/***************************************************************************

		sscanf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-05-14 12:32:48

		Copyright (c)  Yu.liang <lzysoft@126.com> 2007-2009
		All rights reserved.
***************************************************************************/

#include <stdarg.h>
#include <stdio.h>

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#define simple_strtol  strtol
#define simple_strtoul strtoul
#define simple_strtoll strtoll
#define simple_strtoull strtoull

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

static int skip_atoi(const char **s)
{
        int i=0;

        while (isdigit(**s))
                i = i*10 + *((*s)++) - '0';
        return i;
}


#define internal_va_arg( a, b ) \
		((in_skip == 1) ? (b)dummy_buffer : va_arg( a, b ))

/**
 * vsscanf - Unformat a buffer into a list of arguments
 * @buf:        input buffer
 * @fmt:        format of buffer
 * @args:       arguments
 */
int vsscanf(const char * buf, const char * fmt, va_list args)
{
        const char *str = buf;
        char *next;
        char digit;
        int num = 0;
        int qualifier;
        int base;
        int field_width;
        int is_sign = 0;
		int in_skip = 0;
		int dummy_buffer[0x100];		//¿ÕµÄ»º´æ
		
        while(*fmt && *str) {
                /* skip any white space in format */
                /* white space in format matchs any amount of
                 * white space, including none, in the input.
                 */
                if (isspace(*fmt)) {
                        while (isspace(*fmt))
                                ++fmt;
                        while (isspace(*str))
                                ++str;
                }

                /* anything that is not a conversion must match exactly */
                if (*fmt != '%' && *fmt) {
                        if (*fmt++ != *str++)
                                break;
                        continue;
                }

                if (!*fmt)
                        break;
                ++fmt;

                /* skip this conversion.
                 * advance both strings to next white space
                 */
                if (*fmt == '*') {
                        in_skip = 1;
                        fmt++;
                }
                else{
						in_skip = 0;
                }

                /* get field width */
                field_width = -1;
                if (isdigit(*fmt))
                        field_width = skip_atoi(&fmt);

                /* get conversion qualifier */
                qualifier = -1;
                if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
                    *fmt == 'Z' || *fmt == 'z') {
                        qualifier = *fmt++;
                        if (unlikely(qualifier == *fmt)) {
                                if (qualifier == 'h') {
                                        qualifier = 'H';
                                        fmt++;
                                } else if (qualifier == 'l') {
                                        qualifier = 'L';
                                        fmt++;
                                }
                        }
                }
                base = 10;
                is_sign = 0;

                if (!*fmt || !*str)
                        break;

                switch(*fmt++) {
                case 'c':
                {
                        char *s = (char *) internal_va_arg(args,char*);
                        if (field_width == -1)
                                field_width = 1;
                        do {
                                *s++ = *str++;
                        } while (--field_width > 0 && *str);
                        num++;
                }
                continue;
                case 's':
                {
                        char *s = (char *) internal_va_arg(args, char *);
                        if(field_width == -1)
                                field_width = INT_MAX;
                        /* first, skip leading white space in buffer */
                        while (isspace(*str))
                                str++;

                        /* now copy until next white space */
                        while (*str && !isspace(*str) && field_width--) {
                                *s++ = *str++;
                        }
                        *s = '\0';
                        num++;
                }
                continue;
                case 'n':
                        /* return number of characters read so far */
                {
                        int *i = (int *)internal_va_arg(args,int*);
                        *i = str - buf;
                }
                continue;
                case 'o':
                        base = 8;
                        break;
                case 'x':
                case 'X':
                        base = 16;
                        break;
                case 'i':
                        base = 0;
                case 'd':
                        is_sign = 1;
                case 'u':
                        break;
                case '%':
                        /* looking for '%' in str */
                        if (*str++ != '%')
                                return num;
                        continue;
                case '[':
				{
                        char *s = (char *) internal_va_arg(args, char *);
                        if(field_width == -1)
                                field_width = INT_MAX;
                        /* first, skip leading white space in buffer */
                        while (isspace(*str))
                                str++;
						if( *fmt == '^' )
						{
							while (*str) {
								const char * chkchar = fmt+1;
								
								while( *chkchar )
								{
									if( chkchar[1] == '-' )
									{
										char rang_s = chkchar[0];
										char rang_d = chkchar[2];
										chkchar += 3;
										if( *str >= rang_s && *str <= rang_d )
											goto _get_char_nor;
									}
									else
									{
										if( *str == *chkchar )
											goto _get_char_nor;
										chkchar ++;
									}
									if( *chkchar == ']' )
										break;
								}
								*s++ = *str++;
								continue;
							_get_char_nor:
								break;
							}
						}
						else
						{
							while (*str) {
								const char * chkchar = fmt;
								
								while( *chkchar != ']' )
								{
									if( chkchar[1] == '-' )
									{
										char rang_s = chkchar[0];
										char rang_d = chkchar[2];
										chkchar += 3;
										if( *str >= rang_s && *str <= rang_d )
											goto _get_char_or;
									}
									else
									{
										if( *str == *chkchar )
											goto _get_char_or;
										chkchar ++;
									}
								}
								break;
							_get_char_or:
								*s++ = *str++;
							}
						}
                        
                        *s = '\0';
                        num++;
                        fmt = (const char *)strchr( fmt, ']' )+1;
                }
				continue;
                default:
                        /* invalid format; stop here */
                        return num;
                }

                /* have some sort of integer conversion.
                 * first, skip white space in buffer.
                 */
                while (isspace(*str))
                        str++;

                digit = *str;
                if (is_sign && digit == '-')
                        digit = *(str + 1);

                if (!digit
                    || (base == 16 && !isxdigit(digit))
                    || (base == 10 && !isdigit(digit))
                    || (base == 8 && (!isdigit(digit) || digit > '7'))
                    || (base == 0 && !isdigit(digit)))
                                break;

                switch(qualifier) {
                case 'H':       /* that's 'hh' in format */
                        if (is_sign) {
                                signed char *s = (signed char *) internal_va_arg(args,signed char *);
                                *s = (signed char) simple_strtol(str,&next,base);
                        } else {
                                unsigned char *s = (unsigned char *) internal_va_arg(args, unsigned char *);
                                *s = (unsigned char) simple_strtoul(str, &next, base);
                        }
                        break;
                case 'h':
                        if (is_sign) {
                                short *s = (short *) internal_va_arg(args,short *);
                                *s = (short) simple_strtol(str,&next,base);
                        } else {
                                unsigned short *s = (unsigned short *) internal_va_arg(args, unsigned short *);
                                *s = (unsigned short) simple_strtoul(str, &next, base);
                        }
                        break;
                case 'l':
                        if (is_sign) {
                                long *l = (long *) internal_va_arg(args,long *);
                                *l = simple_strtol(str,&next,base);
                        } else {
                                unsigned long *l = (unsigned long*) internal_va_arg(args,unsigned long*);
                                *l = simple_strtoul(str,&next,base);
                        }
                        break;
                case 'L':
                        if (is_sign) {
                                long long *l = (long long*) internal_va_arg(args,long long *);
                                *l = simple_strtoll(str,&next,base);
                        } else {
                                unsigned long long *l = (unsigned long long*) internal_va_arg(args,unsigned long long*);
                                *l = simple_strtoull(str,&next,base);
                        }
                        break;
                case 'Z':
                case 'z':
                {
                        size_t *s = (size_t*) internal_va_arg(args,size_t*);
                        *s = (size_t) simple_strtoul(str,&next,base);
                }
                break;
                default:
                        if (is_sign) {
                                int *i = (int *) internal_va_arg(args, int*);
                                *i = (int) simple_strtol(str,&next,base);
                        } else {
                                unsigned int *i = (unsigned int*) internal_va_arg(args, unsigned int*);
                                *i = (unsigned int) simple_strtoul(str,&next,base);
                        }
                        break;
                }
                num++;

                if (!next)
                        break;
                str = next;
        }
        return num;
}

/**
 * sscanf - Unformat a buffer into a list of arguments
 * @buf:        input buffer
 * @fmt:        formatting of buffer
 * @...:        resulting arguments
 */
int sscanf(const char * buf, const char * fmt, ...)
{
        va_list args;
        int i;

        va_start(args,fmt);
        i = vsscanf(buf,fmt,args);
        va_end(args);

        return i;
}

