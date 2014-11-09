# ifndef __DEBUG_H__
# define __DEBUG_H__

# include <stdio.h>
# include <stdarg.h>

# if (_DEBUG >= 3)
#	define level3(fmt, ...) __debug_printf(3, __func__, fmt, ## __VA_ARGS__)
# else
#	define level3(fmt, ...) /* Nothing */
# endif /* Level 3 */

# if (_DEBUG >= 2)
#	define level2(fmt, ...) __debug_printf(2, __func__, fmt, ## __VA_ARGS__)
# else
#	define level2(fmt, ...) /* Nothing */
# endif /* Level 2 */

# if (_DEBUG >= 1)
#	define level1(fmt, ...) __debug_printf(1, __func__, fmt, ## __VA_ARGS__)
# else
#	define level1(fmt, ...) /* Nothing */
# endif /* Level 1 */

# ifdef _DEBUG
#	define level0(fmt, ...) __debug_printf(0, __func__, fmt, ## __VA_ARGS__)
# else
#	define level0(fmt, ...) /* Nothing */
# endif /* Level 0 */

# ifdef _DEBUG
# define debug_printf(level, fmt, ...) level((char *)fmt, ## __VA_ARGS__)

static __inline__
void __debug_printf(int level, const char *func, char *fmt, ...) {
	va_list valist;
	va_start(valist, fmt);
	printf("[Level %d] %s() : ", level, func);
	vprintf(fmt, valist);
	va_end(valist);
}
# else 
# define debug_printf(level, fmt, ...) /* Nothing */
# endif /* debug_printf */

# endif /* Debug.h */
