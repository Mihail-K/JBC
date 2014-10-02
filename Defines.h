# ifndef __DEFINES_H__
# define __DEFINES_H__

# ifndef __cplusplus
#	if defined __GNUC__
#		define inline __inline__
#	elif defined _MSC_VER
#		define inline __inline
#	else
		/* Just hope for the best */
#	endif /* Compiler */
# endif /* C++ */

# endif /* Defines.h */
