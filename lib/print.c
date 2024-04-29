#include <config.h>
#include <io.h>
#include "print.h"

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

static void __vprint_s(struct vprint_context *c, va_list arg)
{
	const char *p = va_arg(arg, const char*);

	while (*p != '\0' && vprint_buf_free(c))
		*c->buf++ = *p++;
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
