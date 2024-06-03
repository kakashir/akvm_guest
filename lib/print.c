#include <config.h>
#include <io.h>
#include <lib/print.h>
#include <lib/string.h>

struct vprint_context {
	u8 *buf;
	u8 *buf_end;
	const char *format;
	int long_extend;
};

static bool vprint_buf_free(struct vprint_context *c)
{
	return c->buf < c->buf_end;
}

#define __dec2str(c, d, e)			\
	do {					\
		(e) = (d) - ((d) / 10) * 10;	\
		dec2a(e, (c)->buf++);		\
		(d) /= 10;			\
	} while((d) && vprint_buf_free(c));	\

static void __dec2str_signed(struct vprint_context *c, s64 d)
{
	int e;
	u8 *old;

	if (d < 0) {
		*c->buf++ = '-';
		if (!vprint_buf_free(c))
			return;
		d = 0 - d;
	}

	old = c->buf;
	__dec2str(c, d, e);
	swap_byte(old, c->buf - old);
}

static void __dec2str_unsigned(struct vprint_context *c, u64 d)
{
	int e;
	u8 *old;

	old = c->buf;
	__dec2str(c, d, e);
	swap_byte(old, c->buf - old);
}

static void __vprint_d(struct vprint_context *c, va_list arg, bool sign)
{
	if (!c->long_extend) {
		sign ? __dec2str_signed(c, va_arg(arg, int)) :
			__dec2str_unsigned(c, va_arg(arg, unsigned int));
		return;
	}

	sign ? __dec2str_signed(c, va_arg(arg, long)) :
		__dec2str_unsigned(c, va_arg(arg, unsigned long));
}

static void __vprint_s(struct vprint_context *c, va_list arg)
{
	const char *p = va_arg(arg, const char*);

	while (*p != '\0' && vprint_buf_free(c))
		*c->buf++ = *p++;
}

static void __vprint_x(struct vprint_context *c, va_list arg)
{
	u64 v;
	unsigned int i;
	u8 *old = c->buf;

	if (!c->long_extend)
		v = va_arg(arg, unsigned int);
	else
		v = va_arg(arg, unsigned long);

	do {
		i = v & 0xf;
		v >>= 4;
		hex2a(i, c->buf++);
	} while (v && vprint_buf_free(c));

	swap_byte(old, c->buf - old);
}

int vsnprint(u8 *_buf, int _size, const char *_format, va_list _arg)
{
	struct vprint_context c;

	if (!_size || _size < 0)
		return 0;
	if (_size == 1) {
		*_buf = '\0';
		return 1;
	}

	c.buf = _buf;
	/* -1 for '\0' when no enough space */
	c.buf_end = _buf + _size - 1;
	c.format = _format;
	c.long_extend = 0;
	while (*c.format != '\0' && vprint_buf_free(&c)) {
		switch (*c.format) {
		case '%':
			while (*++c.format != '\0' && vprint_buf_free(&c)) {
				switch (*c.format) {
				case 's':
					__vprint_s(&c, _arg);
					c.long_extend = 0;
					break;
				case 'd':
					__vprint_d(&c, _arg, true);
					c.long_extend = 0;
					break;
				case 'u':
					__vprint_d(&c, _arg, false);
					c.long_extend = 0;
					break;
				case 'x':
					__vprint_x(&c, _arg);
					c.long_extend = 0;
					break;
				case 'l':
					++c.long_extend;
					break;
				case '%':
					*c.buf++ = *c.format;
					break;
				default:
					goto outer_loop;
				}
			}
outer_loop:
			break;
		default:
			*c.buf++ = *c.format++;
		}
	}

	*c.buf = '\0';
	return c.buf - _buf;
}

int print(const char* f, ...)
{
	va_list arg;
	unsigned char buf[CONFIG_PRINT_BUF_SIZE];
	int c;

	va_start(arg, f);

	c = vsnprint(buf, sizeof(buf), f, arg);
	if (c) {
		for(int i = 0; i < c; ++i)
			mmio_write(SERIAL_PORT_OUT, 1, buf[i]);
	}

	va_end(arg);
	return c;
}
