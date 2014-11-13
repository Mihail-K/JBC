/**
 * @file ConstantInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.32
 *
 * @brief Defines all debug logging functions.
 **/
# ifndef __DEBUG_H__
# define __DEBUG_H__

# include <stdio.h>
# include <stdarg.h>

/**
 * @addtogroup Debug
 * @{
 **/
namespace JBC {

/**
 * @def level3
 *
 * @brief Level 3 (highest detail) log level.
 **/
# if (_DEBUG >= 3)
#	define level3(fmt, ...) JBC::__debug_printf(3, __func__, fmt, ## __VA_ARGS__)
# else
#	define level3(fmt, ...) /* Nothing */
# endif /* Level 3 */

/**
 * @def level2
 *
 * @brief Level 2 log level.
 **/
# if (_DEBUG >= 2)
#	define level2(fmt, ...) JBC::__debug_printf(2, __func__, fmt, ## __VA_ARGS__)
# else
#	define level2(fmt, ...) /* Nothing */
# endif /* Level 2 */

/**
 * @def level1
 *
 * @brief Level 1 log level.
 **/
# if (_DEBUG >= 1)
#	define level1(fmt, ...) JBC::__debug_printf(1, __func__, fmt, ## __VA_ARGS__)
# else
#	define level1(fmt, ...) /* Nothing */
# endif /* Level 1 */

/**
 * @def level0
 *
 * @brief Level 0 (lowest detail) log level.
 **/
# ifdef _DEBUG
#	define level0(fmt, ...) JBC::__debug_printf(0, __func__, fmt, ## __VA_ARGS__)
# else
#	define level0(fmt, ...) /* Nothing */
# endif /* Level 0 */

/**
 * @def debug_printf(level, fmt, ...)
 *
 * @brief Writes some information to the output log at
 * 			the specified log level.
 *
 * @param level The log level to write the logs at.
 * @param fmt The printf-style format for the output.
 * @param ... Any other arguments, based on format.
 **/
# ifdef _DEBUG
#	define debug_printf(level, fmt, ...) level((char *)fmt, ## __VA_ARGS__)

static inline
void __debug_printf(int level, const char *func, char *fmt, ...) {
	va_list valist;
	va_start(valist, fmt);
	printf("[Level %d] %s() : ", level, func);
	vprintf(fmt, valist);
	va_end(valist);
}
# else 
#	define debug_printf(level, fmt, ...) /* Nothing */
# endif /* debug_printf */

} /* JBC */

/**
 * }@
 **/

# endif /* Debug.h */
