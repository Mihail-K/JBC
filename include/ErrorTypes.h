/**
 * @file ErrorTypes.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.36
 *
 * @brief Defines all common error types used by JBC.
 **/
# ifndef __ERRORTYPES_H__
# define __ERRORTYPES_H__

# include <string>

/**
 * @addtogroup ErrorTypes
 * @{
 **/
namespace JBC {

/**
 * @struct JBCError
 * @brief The common error type for all exception in JBC.
 **/
struct JBCError {
	/**
	 * @brief A message detailing the cause of the error.
	 **/
	const std::string msg;

	/**
	 * @brief Constructor for JBCError type.
	 *
	 * @param msg The error message.
	 **/
	inline
	JBCError(const char *msg)
		: msg(std::string(msg)) {
	}
};

/**
 * @struct DecodeError
 * @brief A common error type for all exceptions raised while
 *			decoding class information.
 **/
struct DecodeError
		: public JBCError {
	/**
	 * @brief Constructor for DecodeError type.
	 *
	 * @param msg The error message.
	 **/
	inline
	DecodeError(const char *msg)
		: JBCError(msg) {
	}
};

/**
 * @struct EncodeError
 * @brief A common error type for all exceptions raised while
 *			encoding class information.
 **/
struct EncodeError
		: public JBCError {
	/**
	 * @brief Constructor for EncodeError type.
	 *
	 * @param msg The error message.
	 **/
	inline
	EncodeError(const char *msg)
		: JBCError(msg) {
	}
};

/**
 * @struct NotFoundError
 * @brief An error caused by a requested element not being present.
 **/
struct NotFoundError
		: public JBCError {
	/**
	 * @brief Constructor for NotFoundError type.
	 *
	 * @param msg The error message.
	 **/
	inline
	NotFoundError(const char *msg)
		: JBCError(msg) {
	}
};

} /* JBC */

/**
 * }@
 **/

# endif /* ErrorTypes.h */
