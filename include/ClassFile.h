/**
 * @file ClassFile.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.36
 **/
# ifndef __CLASSFILE_H__
# define __CLASSFILE_H__

# include <vector>
# include <stdio.h>
# include <stdlib.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

/**
 * @addtogroup ClassFile
 * @{
 **/

namespace JBC {

struct ConstantInfo;
struct ConstantClassInfo;

class MemberInfo;
struct AttributeInfo;

enum ClassFlags {
	CLASS_PUBLIC		= 0x0001,
	CLASS_FINAL			= 0x0010,
	CLASS_SUPER			= 0x0020,
	CLASS_INTERFACE		= 0x0200,
	CLASS_ABSTRACT		= 0x0400,
	CLASS_SYNTHETIC		= 0x1000,
	CLASS_ANNOTATION	= 0x2000,
	CLASS_ENUM			= 0x4000
};

/**
 * @class ClassFile
 * @brief An object representation of a Java class file.
 **/
class ClassFile {
public:
	/**
	 * @brief The magic number.
	 *
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
	 * @brief A table containing constatnt values defined in the class file.
	 *
	 * This include constants such as class, method, and field names.
	 * The first element in this pool should always be NULL, thus the first
	 * usuable element may be found at index 1.
	 *
	 * This table corresponds to both the constant_pool_count item and
	 * the constant_pool item in the file format.
	 */
	std::vector<ConstantInfo *> constant_pool;

	/**
	 * @brief Access permissions and property flags.
	 *
	 * A set of flags denoting various access permissions and properties
	 * for this class file.
	 **/
	uint16_t	access_flags;

	// This Class
	/**
	 * @brief The local type, declared in this ClassFile.
	 *
	 * A Constant class object, representing the local outermost type
	 * within the class file.
	 **/
	ConstantClassInfo *this_class;

	// Super Class
	/**
	 * @brief The parent type to this ClassFile, or NULL.
	 *
	 * A Constant class object, representing the direct supertype
	 * to the type within the class file. If this item is NULL,
	 * it is assumed to be Object.
	 **/
	ConstantClassInfo *super_class;

	// Interfaces Table
	/**
	 * @brief A list of interfaces that the local type implements.
	 **/
	std::vector<ConstantClassInfo *> interfaces;

	// Fields Table
	/**
	 * @brief The list of fields declared in this ClassFile.
	 *
	 * A list of MemberInfo items, represeting the table of fields present
	 * within the local type. This list only contains fields declared
	 * directly within the class, and not those inheritted from a parent type.
	 **/
	std::vector<MemberInfo *> fields;

	// Methods Table
	/**
	 * @brief The list of methods declared in this ClassFile.
	 *
	 * A list of MemberInfo items, represeting the table of methods present
	 * within the local type. This list only contains methods declared
	 * directly within the class, and not those inheritted from a parent type.
	 * This also includes constructors, and the type initializer method.
	 **/
	std::vector<MemberInfo *> methods;

	// Attributes Table
	/**
	 * @brief The AttributeInfo list for this ClassFile.
	 *
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
	 * - RuntimeVisibleAnnotationsAttribute
	 * - RuntimeInvisibleAnnotationsAttribute
	 * - BootstrapMethodsAttribute
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
	 * @param magic The magic number to check for when decoding.
	 **/
	ClassFile(ClassBuffer *buffer, uint32_t magic = 0xCAFEBABE);

	/**
	 * @brief Destructor for the class file type.
	 **/
	~ClassFile();

public:
	/**
	 * @breif Returns a reference to the magic number.
	 **/
	inline
	uint32_t &Magic() {
		return magic;
	}

	/**
	 * @breif Returns a reference to the major version number.
	 **/
	inline
	uint16_t &MajorVersion() {
		return major_version;
	}

	/**
	 * @breif Returns a reference to the minor version number.
	 **/
	inline
	uint16_t &MinorVersion() {
		return minor_version;
	}

	/**
	 * @breif Adds a Constant to this Class File.
	 *
	 * This also sets the index field of the attribute to its
	 * position within the constant pool.
	 *
	 * @return A refernce to the Constant.
	 */
	ConstantInfo *&AddConstant(ConstantInfo *info);

	/**
	 * @brief Returns a reference to this class's flags.
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
	bool GetFlag(ClassFlags flag) {
		return !!(access_flags & flag);
	}

	/**
	 * @brief Sets the state of a single flag.
	 *
	 * @param flag The flag to be modified.
	 * @param state The new state for the flag.
	 **/
	void SetFlag(ClassFlags flag, bool state) {
		if(state) access_flags |= flag;
		else access_flags &= ~flag;
	}

	/**
	 * @brief Returns this class's name.
	 *
	 * @return The name of this class.
	 **/
	std::string ThisName() {
		if(this_class == NULL) return NULL;
		return reinterpret_cast<char *>(this_class);
	}

	/**
	 * @brief Returns this class's Constant Info.
	 **/
	inline
	ConstantClassInfo *&ThisClass() {
		return this_class;
	}

	/**
	 * @brief Returns the parent class's name.
	 *
	 * @return The name of the super class.
	 **/
	std::string SuperName() {
		if(super_class == NULL) return NULL;
		return reinterpret_cast<char *>(super_class);
	}

	/**
	 * @brief Returns the parent class's Constant Info.
	 **/
	inline
	ConstantClassInfo *&SuperClass() {
		return super_class;
	}

	/**
	 * @breif Adds an interface to this class.
	 *
	 * @param info The interface's Constant info.
	 **/
	ConstantClassInfo *&AddInterface(ConstantClassInfo *info);

	/**
	 * @brief Adds a field to this class.
	 **/
	MemberInfo *&AddField(MemberInfo *field);

	/**
	 * @brief Looks up a field, by name.
	 *
	 * @param name The name of the field to find.
	 * @return A reference to the named field, if found.
	 **/
	MemberInfo *&FindField(std::string &name);

	/**
	 * @breif Adds a method to this class.
	 **/
	MemberInfo *&AddMethod(MemberInfo *method);

	/**
	 * @brief Looks up a method, by name.
	 *
	 * @param name The name of the method to find.
	 * @return A reference to the named method, if found.
	 **/
	MemberInfo *&FindMethod(std::string &name);

	/**
	 * @breif Attaches an Attribute to this class.
	 * 
	 * This function also optionally validate that this Attribute
	 * is allowed to be attached to Class Files.
	 * Validation is done through blacklisting, so nonstandard
	 * Attributes will not cause an error.
	 *
	 * @param info The Attribute Info object to add.
	 * @param validate Whether to validate that this Attribute.
	 **/
	AttributeInfo *&AddAttribute(AttributeInfo *info, bool validate = true);

public:
	/**
	 * @brief Decoding function for the class file.
	 *
	 * Decodes information from a ClassBuffer, populating fields
	 * and subfields of the class file, and builds and resolves
	 * values in, as well as references to the constant pool.
	 *
	 * @param buffer The ClassBuffer to decode data from.
	 **/
	void DecodeClassFile(ClassBuffer *buffer);

	/**
	 * @brief Encoding function for the class file.
	 *
	 * Encodes information into a ClassBuilder, traversing the
	 * class file tree and writing it as a set of flat tables,
	 * as per the Java class file format.
	 *
	 * @param builder The ClassBuilder to encode data into.
	 **/
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

/**
 * @brief Reads and creates a class file from an input file.
 *
 * Decoder helper funciton. Creates a ClassBuffer object from
 * the input file, and uses it to populate a new ClassFile object,
 * which is returned if the operation is successful.
 * In all cases, the input file is closed when the method exits.
 *
 * @param source The file to create the ClassBuffer from.
 * @param magic The magic number to check for when decoding.
 * @return The class file representation of the input file.
 **/
ClassFile *DecodeClassFile(FILE *source, uint32_t magic = 0xCAFEBABE);

/**
 * @brief Writes a class file into an output file.
 *
 * Encoder helper funciton. Creates a ClassBuilder object from
 * the output file, and uses it to write information for the
 * ClassFile.
 * The output file is closed when the method exits.
 *
 * @param source The file to create the ClassBuilder from.
 * @param classFile The class file to be writtten.
 **/
void EncodeClassFile(FILE *target, ClassFile *classFile);

} /* JBC */

/**
 * }@
 **/

# endif /* ClassFile.h */
