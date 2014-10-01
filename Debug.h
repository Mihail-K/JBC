# ifndef __DEBUG_H__
# define __DEBUG_H__

# include <stdio.h>
# include <stdarg.h>

# ifdef _DEBUG
# define debug_printf(fmt, ...) __debug_printf(__func__, fmt, ## __VA_ARGS__)

static __inline__
void __debug_printf(const char *func, char *fmt, ...) {
	va_list valist;
	va_start(valist, fmt);
	printf("%s() : ", func);
	vprintf(fmt, valist);
	va_end(valist);
}
# else 
# define debug_printf(fmt, ...) ((void)0)
# endif /* debug_printf */

# endif /* Debug.h */
