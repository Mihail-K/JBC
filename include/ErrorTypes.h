# ifndef __ERRORTYPES_H__
# define __ERRORTYPES_H__

# include <string>

struct JBCError {
	const std::string msg;

	inline
	JBCError(const char *msg)
		: msg(std::string(msg)) {
	}
};

struct DecodeError
		: public JBCError {
	inline
	DecodeError(const char *msg)
		: JBCError(msg) {
	}
};

struct EncodeError
		: public JBCError {
	inline
	EncodeError(const char *msg)
		: JBCError(msg) {
	}
};

# endif /* ErrorTypes.h */
