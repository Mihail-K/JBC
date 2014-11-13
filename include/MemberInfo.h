/**
 * @file MemberInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.33
 *
 * @brief Defines class member types and functions.
 **/
# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include <vector>
# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

/**
 * @addtogroup MemberInfo
 * @{
 **/
namespace JBC {

// Forward Declarations.
class ClassFile;
struct AttributeInfo;
struct ConstantUtf8Info;

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
	MemberInfo *DecodeMember(ClassBuffer *buffer, ClassFile *classFile);
	MemberInfo *EncodeMember(ClassBuilder *builder, ClassFile *classFile);
};

} /* JBC */

/**
 * }@
 **/

# endif /* MemberInfo.h */
