# ifndef __DEBUG_H__
# define __DEBUG_H__

# include <stdio.h>

# ifdef _DEBUG
# define debug_printf(fmt, ...) printf(fmt, ## __VA_ARGS__)
# else 
# define debug_printf(fmt, ...) ((void)0)
# endif /* debug_printf */

# endif /* Debug.h */
