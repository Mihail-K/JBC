# ifndef __DEFINES_H__
# define __DEFINES_H__

/* Portable inline definition */
# ifndef __cplusplus
#	if defined __GNUC__
#		define inline __inline__
#	elif defined _MSC_VER
#		define inline __inline
#	else
#		define inline
#	endif /* Compiler */
# endif /* C++ */

/* Cast disambiguation */
# ifndef __cplusplus
#	define const_cast(T, V)			(T)(V)
#	define static_cast(T, V)		(T)(V)
#	define dynamic_cast(T, V)		(T)(V)
#	define reinterpret_cast(T, V)	*(T *)(V)
# else /* Pure C */
#	define const_cast(T, V)			const_cast<T>(V)
#	define static_cast(T, V)		static_cast<T>(V)
#	define reinterpret_cast(T, V)	reinterpret_cast<T>(V)
# endif /* C++ */

# ifndef __cplusplus
#	define NEW(T)		(T *)zalloc(sizeof(T))
#	define ALLOC(T, C)	(T *)zalloc(sizeof(T) * C)
#	define DELETE(P)	free(P)
# else /* Pure C */
#	define NEW(T)		new T
#	define ALLOC(T, C)	new T[C]
#	define DELETE(P)	delete P
# endif /* C++ */

# endif /* Defines.h */
