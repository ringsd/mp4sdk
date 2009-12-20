/***************************************************************************

		_vsnprintf.c

		defalut.

		TIME LIST:
		CREATE	Rainy	2009-06-10 17:56:17

		Copyright (c)  ChinaChip 2007-2009
***************************************************************************/

#include <stdarg.h>
#include <ctype.h>

#define ZEROPAD 1               /* pad with zero */
#define SIGN    2               /* unsigned/signed long */
#define PLUS    4               /* show plus */
#define SPACE   8               /* space if plus */
#define LEFT    16              /* left justified */
#define SPECIAL 32              /* 0x */
#define LARGE   64              /* use 'ABCDEF' instead of 'abcdef' */

#define SHRT_MAX 32767

static int
is_digit (int c)
{
  return ('0' <= c && c <= '9');
}

static int skip_atoi(const char **s)
{
        int i=0;

        while (isdigit(**s))
                i = i*10 + *((*s)++) - '0';
        return i;
}


int
bvprintf(char** buf, char* end, register const char* format, va_list ap)
{
        register int    c;
        register char*  p;
        register char*  e;
        int             w;
        int             l;
        int             f;
        int             g;
        int             r;
        long            n;
        unsigned long   u;
#ifdef _ast_int8_t
        _ast_int8_t             q;
        unsigned _ast_int8_t    v;
#endif
        char*           s;
        char*           b;
        char*           x;
        char            num[32];

        static char     digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLNMOPQRSTUVWXYZ@_";

        if (buf)
        {
                p = *buf;
                e = end;
        }
        else e = (p = end) + SHRT_MAX;
        for (;;)
        {
                switch (c = *format++)
                {
                case 0:
                        goto done;
                case '%':
                        if (*format == '-')
                        {
                                format++;
                                l = 1;
                        }
                        else l = 0;
                        if (*format == '0')
                        {
                                format++;
                                f = l ? ' ' : '0';
                        }
                        else f = ' ';
                        if ((c = *format) == '*')
                        {
                                format++;
                                w = va_arg(ap, int);
                        }
                        else
                        {
                                w = 0;
                                while (c >= '0' && c <= '9')
                                {
                                        w = w * 10 + c - '0';
                                        c = *++format;
                                }
                        }
                        r = 0;
                        if (c == '.')
                        {
                                if ((c = *++format) == '*')
                                {
                                        format++;
                                        va_arg(ap, int);
                                }
                                else while (c >= '0' && c <= '9') c = *++format;
                                if (c == '.')
                                {
                                        if ((c = *++format) == '*')
                                        {
                                                format++;
                                                r = va_arg(ap, int);
                                        }
                                        else while (c >= '0' && c <= '9')
                                        {
                                                r = r * 10 + c - '0';
                                                c = *++format;
                                        }
                                }
                        }
                        if ((c = *format++) != 'l')
                                n = 0;
                        else if ((c = *format++) != 'l')
                                n = 1;
                        else
                        {
                                n = 2;
                                c = *format++;
                        }
                        g = 0;
                        b = num;
                        switch (c)
                        {
                        case 0:
                                break;
                        case 'c':
                                *b++ = va_arg(ap, int);
                                break;
                        case 'd':
                                switch (n)
                                {
                                case 0:
                                        n = va_arg(ap, int);
                                        break;
                                default:
#ifdef _ast_int8_t
                                        q = va_arg(ap, _ast_int8_t);
                                        if (q < 0)
                                        {
                                                g = '-';
                                                q = -q;
                                        }
                                        v = q;
                                        goto dec_8;
                                case 1:
#endif
                                        n = va_arg(ap, long);
                                        break;
                                }
                                if (n < 0)
                                {
                                        g = '-';
                                        n = -n;
                                }
                                u = n;
                                goto dec;
                        case 'o':
                                switch (n)
                                {
                                case 0:
                                        u = va_arg(ap, unsigned int);
                                        break;
                                default:
#ifdef _ast_int8_t
                                        v = va_arg(ap, unsigned _ast_int8_t);
                                        goto oct_8;
                                case 1:
#endif
                                        u = va_arg(ap, unsigned long);
                                        break;
                                }
                                do *b++ = (u & 07) + '0'; while (u >>= 3);
                                break;
                        case 's':
                                s = va_arg(ap, char*);
                                if (!s) s = "(null)";
                                if (w)
                                {
                                        n = w - strlen(s);
                                        if (l)
                                        {
                                                while (w-- > 0)
                                                {
                                                        if (p >= e) goto done;
                                                        if (!(*p = *s++)) break;
                                                        p++;
                                                }
                                                while (n-- > 0)
                                                {
                                                        if (p >= e) goto done;
                                                        *p++ = f;
                                                }
                                                continue;
                                        }
                                        while (n-- > 0)
                                        {
                                                if (p >= e) goto done;
                                                *p++ = f;
                                        }
                                }
                                for (;;)
                                {
                                        if (p >= e) goto done;
                                        if (!(*p = *s++)) break;
                                        p++;
                                }
                                continue;
                        case 'u':
                                switch (n)
                                {
                                case 0:
                                        u = va_arg(ap, unsigned int);
                                        break;
                                default:
#ifdef _ast_int8_t
                                        v = va_arg(ap, unsigned _ast_int8_t);
                                        goto dec_8;
                                case 1:
#endif
                                        u = va_arg(ap, unsigned long);
                                        break;
                                }
                        dec:
                                if (r <= 0 || r >= sizeof(digits)) r = 10;
                                do *b++ = digits[u % r]; while (u /= r);
                                break;
                        case 'p':
                                if (x = va_arg(ap, char*))
                                {
                                        g = 'x';
                                        w = 10;
                                        f = '0';
                                        l = 0;
                                }
#ifdef _ast_int8_t
                                if (sizeof(char*) == sizeof(_ast_int8_t))
                                {
                                        v = (unsigned _ast_int8_t)x;
                                        goto hex_8;
                                }
#endif
                                u = (unsigned long)x;
                                goto hex;
                        case 'x':
                                switch (n)
                                {
                                case 0:
                                        u = va_arg(ap, unsigned int);
                                        break;
                                default:
#ifdef _ast_int8_t
                                        v = va_arg(ap, unsigned _ast_int8_t);
                                        goto hex_8;
                                case 1:
#endif
                                        u = va_arg(ap, unsigned long);
                                        break;
                                }
                        hex:
                                do *b++ = ((n = (u & 0xf)) >= 0xa) ? n - 0xa + 'a' : n + '0'; while (u >>= 4);
                                break;
                        default:
                                if (p >= e) goto done;
                                *p++ = c;
                                continue;
#ifdef _ast_int8_t
                        dec_8:
                                if (r <= 0 || r >= sizeof(digits)) r = 10;
                                do *b++ = digits[v % r]; while (v /= r);
                                break;
                        hex_8:
                                do *b++ = ((n = (v & 0xf)) >= 0xa) ? n - 0xa + 'a' : n + '0'; while (v >>= 4);
                                break;
                        oct_8:
                                do *b++ = (v & 07) + '0'; while (v >>= 3);
                                break;
#endif
                        }
                        if (w)
                        {
                                if (g == 'x') w -= 2;
                                else if (g) w -= 1;
                                n = w - (b - num);
                                if (!l)
                                {
                                        if (g && f != ' ')
                                        {
                                                if (g == 'x')
                                                {
                                                        if (p >= e) goto done;
                                                        *p++ = '0';
                                                        if (p >= e) goto done;
                                                        *p++ = 'x';
                                                }
                                                else if (p >= e) goto done;
                                                else *p++ = g;
                                                g = 0;
                                        }
                                        while (n-- > 0)
                                        {
                                                if (p >= e) goto done;
                                                *p++ = f;
                                        }
                                }
                        }
                        if (g == 'x')
                        {
                                if (p >= e) goto done;
                                *p++ = '0';
                                if (p >= e) goto done;
                                *p++ = 'x';
                        }
                        else if (g)
                        {
                                if (p >= e) goto done;
                                *p++ = g;
                        }
                        while (b > num)
                        {
                                if (p >= e) goto done;
                                *p++ = *--b;
                        }
                        if (w && l) while (n-- > 0)
                        {
                                if (p >= e) goto done;
                                *p++ = f;
                        }
                        continue;
                default:
                        if (p >= e) goto done;
                        *p++ = c;
                        continue;
                }
                break;
        }
 done:
        if (p < e) *p = 0;
        if (buf)
        {
                c = p - *buf;
                *buf = p;
        }
        else c = p - end;
        return(c);
}

int
bprintf(char** buf, char* end, const char* format, ...)
{
        va_list ap;
        int     n;

        va_start(ap, format);
        n = bvprintf(buf, end, format, ap);
        va_end(ap);
        return(n);
}


int
sfsprintf(char* buffer, int size, const char* format, ...)
{
        va_list ap;
        char**  buf;
        char*   end;
        int     n;

        va_start(ap, format);
        if (size)
        {
                buf = &buffer;
                end = buffer + size;
        }
        else
        {
                buf = 0;
                end = buffer;
        }
        n = bvprintf(buf, end, format, ap);
        va_end(ap);
        return(n);
}


static char*
number(register char* s, register char* e, register long n, register int p, int w, int pad)
{
        char*   b;

        if (w && w > p)
                p = w;
        switch (pad)
        {
        case '-':
                p = 0;
                break;
        case '_':
                if (p > 0)
                        p = -p;
                break;
        case '0':
                if (p < 0)
                        p = -p;
                break;
        }
        b = s;
        if (p > 0)
                s += sfsprintf(s, e - s, "%0*lu", p, n);
        else if (p < 0)
                s += sfsprintf(s, e - s, "%*lu", -p, n);
        else
                s += sfsprintf(s, e - s, "%lu", n);
        if (w && (s - b) > w)
                *(s = b + w) = 0;
        return s;
}


int _vsnprintf(char *buf, int n, const char *fmt, va_list args)
{
        int len;
        unsigned long num;
        int i, base;
        char * str;
        const char *s;

        int flags;              /* flags to number() */

        int field_width;        /* width of output field */
        int precision;          /* min. # of digits for integers; max
                                   number of chars for from string */
        int qualifier;          /* 'h', 'l', or 'L' for integer fields */

        for (str = buf; *fmt && (n == -1 || str - buf < n); ++fmt) {
                if (*fmt != '%') {
                        *str++ = *fmt;
                        continue;
                }

                /* process flags */
                flags = 0;
                repeat:
                        ++fmt;          /* this also skips first '%' */
                        switch (*fmt) {
                                case '-': flags |= LEFT; goto repeat;
                                case '+': flags |= PLUS; goto repeat;
                                case ' ': flags |= SPACE; goto repeat;
                                case '#': flags |= SPECIAL; goto repeat;
                                case '0': flags |= ZEROPAD; goto repeat;
                                }

                /* get field width */
                field_width = -1;
                if (is_digit(*fmt))
                        field_width = skip_atoi(&fmt);
                else if (*fmt == '*') {
                        ++fmt;
                        /* it's the next argument */
                        field_width = va_arg(args, int);
                        if (field_width < 0) {
                                field_width = -field_width;
                                flags |= LEFT;
                        }
                }

                /* get the precision */
                precision = -1;
                if (*fmt == '.') {
                        ++fmt;
                        if (is_digit(*fmt))
                                precision = skip_atoi(&fmt);
                        else if (*fmt == '*') {
                                ++fmt;
                                /* it's the next argument */
                                precision = va_arg(args, int);
                        }
                        if (precision < 0)
                                precision = 0;
                }

                /* get the conversion qualifier */
                qualifier = -1;
                if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
                        qualifier = *fmt;
                        ++fmt;
                }

                /* default base */
                base = 10;

                switch (*fmt) {
                case 'c':
                        if (!(flags & LEFT))
                                while (--field_width > 0)
                                        *str++ = ' ';
                        *str++ = (unsigned char) va_arg(args, int);
                        while (--field_width > 0)
                                *str++ = ' ';
                        continue;

                case 's':
                        s = va_arg(args, char *);
                        if (!s)
                                s = "<NULL>";

                        len = strnlen(s, precision);

                        if (n != -1 && len >= n - (str - buf)) {
                                len = n - 1 - (str - buf);
                                if (len <= 0) break;
                                if (len < field_width) field_width = len;
                        }

                        if (!(flags & LEFT))
                                while (len < field_width--)
                                        *str++ = ' ';
                        for (i = 0; i < len; ++i)
                                *str++ = *s++;
                        while (len < field_width--)
                                *str++ = ' ';
                        continue;

                case 'p':
                        if (field_width == -1) {
                                field_width = 2*sizeof(void *);
                                flags |= ZEROPAD;
                        }
                        str = number(str,
                                (unsigned long) va_arg(args, void *), 16,
                                field_width, precision, flags);
                        continue;


                case 'n':
                        if (qualifier == 'l') {
                                long * ip = va_arg(args, long *);
                                *ip = (str - buf);
                        } else {
                                int * ip = va_arg(args, int *);
                                *ip = (str - buf);
                        }
                        continue;

                case '%':
                        *str++ = '%';
                        continue;

                /* integer number formats - set up the flags and "break" */
                case 'o':
                        base = 8;
                        break;

                case 'X':
                        flags |= LARGE;
                case 'x':
                        base = 16;
                        break;

                case 'd':
                case 'i':
                        flags |= SIGN;
                case 'u':
                        break;

                default:
                        *str++ = '%';
                        if (*fmt)
                                *str++ = *fmt;
                        else
                                --fmt;
                        continue;
                }
                if (qualifier == 'l')
                        num = va_arg(args, unsigned long);
                else if (qualifier == 'h') {
                        num = (unsigned short) va_arg(args, int);
                        if (flags & SIGN)
                                num = (short) num;
                } else if (flags & SIGN)
                        num = va_arg(args, int);
                else
                        num = va_arg(args, unsigned int);
                str = number(str, num, base, field_width, precision, flags);
        }
        *str = '\0';
        return str-buf;
}

int vsnprintf(char *buf, int n, const char *fmt, va_list args)
{
	_vsnprintf( buf, n, fmt, args );
}

int _IO_vsnprintf(char *buf, int n, const char *fmt, va_list args)
{
	_vsnprintf( buf, n, fmt, args );
}
