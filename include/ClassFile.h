# ifndef __CLASSFILE_H__
# define __CLASSFILE_H__

# include <vector>
# include <stdio.h>
# include <stdlib.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

namespace JBC {

struct ConstantInfo;
struct ConstantClassInfo;

class MemberInfo;
struct AttributeInfo;

/**
 * @file
 * @author Mihail K
 * @version 1.0
 *
 * @section DESCRIPTION
 * @class ClassFile
 *
 * @brief A representation of binary values within a class file.
 **/
class ClassFile {
public:
	/**
	 * A magic number serving to identify the class file format,
	 * typically with a value of 0xCAFEBABE in Java class files.
	 **/
	uint32_t	magic;

	// Version Info
	/**
	 * @brief The major version number for this class file.
	 */
	uint16_t	major_version;
	/**
	 * @brief The minor version number for this class file.
	 **/
	uint16_t	minor_version;

	// Constants Table
	/**
	 * A table containing the various constant values defined within the
	 * class file, and referenced by other sections for information
	 * such as class, method, and field names, and constant values
	 * of various sorts. The first element in this pool should always
	 * be NULL, thus the first usuable element is found a index 1.
	 *
	 * This table corresponds to both the constant_pool_count item and
	 * the constant_pool item in the file format.
	 */
	std::vector<ConstantInfo *> constant_pool;

	/**
	 * A set of flags denoting various access permissions and properties
	 * for this class file.
	 **/
	uint16_t	access_flags;

	// This Class
	/**
	 * A Constant class object, representing the local outermost type
	 * within the class file.
	 **/
	ConstantClassInfo *this_class;

	// Super Class
	/**
	 * A Constant class object, representing the direct supertype
	 * to the type within the class file. If this item is NULL,
	 * it is assumed to be the default supertype (usually Object).
	 **/
	ConstantClassInfo *super_class;

	// Interfaces Table
	/**
	 * @brief A list of interfaces that the local type implements.
	 **/
	std::vector<ConstantClassInfo *> interfaces;

	// Fields Table
	/**
	 * A list of MemberInfo items, represeting the table of fields present
	 * within the local type. This list only contains fields declared
	 * directly within the class, and not those inheritted from a parent type.
	 **/
	std::vector<MemberInfo *> fields;

	// Methods Table
	/**
	 * A list of MemberInfo items, represeting the table of methods present
	 * within the local type. This list only contains methods declared
	 * directly within the class, and not those inheritted from a parent type.
	 * This also includes constructors, and the type initializer method.
	 **/
	std::vector<MemberInfo *> methods;

	// Attributes Table
	/**
	 * A list of AttributeInfo items, representing the attributes table for
	 * the class file.
	 *
	 * According to the Java specification, the attributes that may appear
	 * within this field are:
	 * - InnerClassesAttribute
	 * - EnclosingMethodAttribute
	 * - SyntheticAttribute
	 * - SignatureAttribute
	 * - SourceFileAttribute
	 * - SourceDebugExtensionAttribute
	 * - DeprecatedAttribute
	 * - RuntimeVisibleAnnotations
	 * - RuntimeInvisibleAnnotations
	 * - BootstrapMethodAnnotations
	 **/
	std::vector<AttributeInfo *> attributes;

public:
	// Constructors
	/**
	 * @brief Plain constructor for the class file type.
	 **/
	ClassFile();
	/**
	 * @brief Decoding constructor for the class file type.
	 *
	 * A call to this is equivalant to:
	 * @code
	 * (new ClassFile)->DecodeClassFile(buffer)
	 * @endcode
	 *
	 * @param buffer The ClassBuffer to decode data from.
	 **/
	ClassFile(ClassBuffer *buffer);

	~ClassFile();

public:
	void DecodeClassFile(ClassBuffer *buffer);
	void EncodeClassFile(ClassBuilder *builder);

private:
	void DecodeConstants(ClassBuffer *buffer);
	void EncodeConstants(ClassBuilder *builder);

	void DecodeClasses(ClassBuffer *buffer);
	void EncodeClasses(ClassBuilder *builder);

	void DecodeInterfaces(ClassBuffer *buffer);
	void EncodeInterfaces(ClassBuilder *builder);

	void DecodeFields(ClassBuffer *buffer);
	void EncodeFields(ClassBuilder *builder);

	void DecodeMethods(ClassBuffer *buffer);
	void EncodeMethods(ClassBuilder *builder);

	void DecodeAttributes(ClassBuffer *buffer);
	void EncodeAttributes(ClassBuilder *builder);
};

ClassFile *DecodeClassFile(FILE *source);

int EncodeClassFile(FILE *target, ClassFile *classFile);

} /* JBC */

# endif /* ClassFile.h */
