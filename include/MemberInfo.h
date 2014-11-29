/**
 * @file MemberInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.36
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
	 * @breif Returns a reference to this member's flags.
	 **/
	inline
	uint16_t &Flags() {
		return access_flags;
	}

	/**
	 * @breif Returns the state of a single flag.
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
	MemberInfo *DecodeMember(ClassBuffer *buffer, ClassFile *classFile);
	MemberInfo *EncodeMember(ClassBuilder *builder, ClassFile *classFile);
};

} /* JBC */

/**
 * }@
 **/

# endif /* MemberInfo.h */
