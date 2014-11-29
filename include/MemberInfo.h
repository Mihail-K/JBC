/**
 * @file MemberInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.40
 *
 * @brief Defines class member types and functions.
 **/
# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include <vector>
# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

# include "ConstantInfo.h"

/**
 * @addtogroup MemberInfo
 * @{
 **/
namespace JBC {

// Forward Declarations.
class ClassFile;
struct AttributeInfo;

/* Access Flags */

enum MemberFlags {
	FIELD_PUBLIC		= 0x0001,
	FIELD_PRIVATE		= 0x0002,
	FIELD_PROTECTED		= 0x0004,
	FIELD_STATIC		= 0x0008,
	FIELD_FINAL			= 0x0010,
	FIELD_VOLATILE		= 0x0040,
	FIELD_TRANSIENT		= 0x0080,
	FIELD_SYNTHETIC		= 0x1000,
	FIELD_ENUM			= 0x4000,

	METHOD_PUBLIC		= 0x0001,
	METHOD_PRIVATE		= 0x0002,
	METHOD_PROTECTED	= 0x0004,
	METHOD_STATIC		= 0x0008,
	METHOD_FINAL		= 0x0010,
	METHOD_SYNCHRONIZED	= 0x0020,
	METHOD_BRIDGE		= 0x0040,
	METHOD_VARARGS		= 0x0080,
	METHOD_NATIVE		= 0x0100,
	METHOD_ABSTRACT		= 0x0400,
	METHOD_STRICT		= 0x0800,
	METHOD_SYNTHETIC	= 0x1000
};

/* Field/Method Info */

/**
 * @class MemberInfo
 * @brief An object representation of Java class members,
 *			such as methods or fields.
 **/
class MemberInfo {
public:
	/**
	 * @brief Access permissions and property flags.
	 *
	 * A set of flags denoting various access permissions and properties
	 * for this class member.
	 **/
	uint16_t	access_flags;

	// Name
	/**
	 * @brief A constant value denoting the name of the member.
	 **/
	ConstantUtf8Info *name;

	// Descriptor
	/**
	 * @brief A constant value denoting the method descriptor.
	 **/
	ConstantUtf8Info *descriptor;

	// Attributes Table
	/**
	 *
	 **/
	std::vector<AttributeInfo *> attributes;

public:
	/**
	 * @brief Constructor for MemverInfo.
	 **/
	MemberInfo();
	/**
	 * @brief Destructor for MemberInfo.
	 **/
	~MemberInfo();

public:
	/**
	 * @brief Returns a reference to this member's flags.
	 **/
	inline
	uint16_t &Flags() {
		return access_flags;
	}

	/**
	 * @brief Returns the state of a single flag.
	 *
	 * @param flag The flag to check for.
	 **/
	bool GetFlag(MemberFlags flag) {
		return !!(access_flags & flag);
	}

	/**
	 * @brief Sets the state of a single flag.
	 *
	 * @param flag The flag to be modified.
	 * @param state The new state for the flag.
	 **/
	void SetFlag(MemberFlags flag, bool state) {
		if(state) access_flags |= flag;
		else access_flags &= ~flag;
	}

	/**
	 * @brief Returns this member's name.
	 *
	 * @return The name of this member.
	 **/
	std::string Name() {
		if(name == NULL) return NULL;
		return reinterpret_cast<char *>(name->bytes);
	}

	/**
	 * @brief Returns this member's descriptor descriptor.
	 *
	 * @return The descriptor for this member.
	 **/
	std::string Descriptor() {
		if(descriptor == NULL) return NULL;
		return reinterpret_cast<char *>(descriptor->bytes);
	}

public:
	/**
	 * @brief Checks if this member is public.
	 **/
	inline
	bool IsPublic() {
		return GetFlag(FIELD_PUBLIC);
	}

	/**
	 * @brief Checks if this member is protected.
	 **/
	inline
	bool IsProtected() {
		return GetFlag(FIELD_PROTECTED);
	}

	/**
	 * @brief Checks if this member is private.
	 **/
	inline
	bool IsPrivate() {
		return GetFlag(FIELD_PRIVATE);
	}

	/**
	 * @brief Checks if this member is package private (default).
	 **/
	inline
	bool IsPackagePrivate() {
		return !IsPublic() && !IsProtected()
				&& !IsPrivate();
	}

	/**
	 * @brief Checks if this member is static.
	 **/
	inline
	bool IsStatic() {
		return GetFlag(FIELD_STATIC);
	}

	/**
	 * @brief Checks if this member is final.
	 **/
	inline
	bool IsFinal() {
		return GetFlag(FIELD_FINAL);
	}

	/**
	 * @brief Checks if this member is volatile.
	 *
	 * This property is only meaningful for fields.
	 **/
	inline
	bool IsVolatile() {
		return GetFlag(FIELD_VOLATILE);
	}

	/**
	 * @brief Checks if this member is transient.
	 *
	 * This property is only meaningful for fields.
	 **/
	inline
	bool IsTransient() {
		return GetFlag(FIELD_TRANSIENT);
	}

	/**
	 * @brief Checks if this member is synthetic.
	 **/
	inline
	bool IsSynthetic() {
		return GetFlag(FIELD_SYNTHETIC);
	}


	/**
	 * @brief Checks if this member is an enum constant.
	 *
	 * This property is only meaningful for fields.
	 **/
	inline
	bool IsEnumConstant() {
		return GetFlag(FIELD_ENUM);
	}

	/**
	 * @brief Checks if this member is synchronzied.
	 *
	 * This property is only meaningful for methods.
	 **/
	inline
	bool IsSynchronized() {
		return GetFlag(METHOD_SYNCHRONIZED);
	}
	
	/**
	 * @brief Checks if this member is a bridge.
	 *
	 * This property is only meaningful for methods.
	 **/
	inline
	inline
	bool IsBridge() {
		return GetFlag(METHOD_BRIDGE);
	}

	/**
	 * @brief Checks if this member takes variable arguments.
	 *
	 * This property is only meaningful for methods.
	 **/
	inline
	inline
	bool IsVarArgs() {
		return GetFlag(METHOD_VARARGS);
	}

	/**
	 * @brief Checks if this member is native.
	 *
	 * This property is only meaningful for methods.
	 **/
	inline
	inline
	bool IsNative() {
		return GetFlag(METHOD_NATIVE);
	}

	/**
	 * @brief Checks if this member is abstract.
	 *
	 * This property is only meaningful for methods.
	 **/
	inline
	inline
	bool IsAbstract() {
		return GetFlag(METHOD_ABSTRACT);
	}

	/**
	 * @brief Checks if this member is in strict floating-point mode.
	 *
	 * This property is only meaningful for methods.
	 **/
	inline
	inline
	bool IsStrict() {
		return GetFlag(METHOD_SCRICT);
	}

public:
	MemberInfo *DecodeMember(ClassBuffer *buffer, ClassFile *classFile);
	MemberInfo *EncodeMember(ClassBuilder *builder, ClassFile *classFile);
};

} /* JBC */

/**
 * }@
 **/

# endif /* MemberInfo.h */
