JBC
===
[![Build Status](https://travis-ci.org/Mihail-K/JBC.svg?branch=master)](https://travis-ci.org/Mihail-K/JBC)
[![Coverity Status](https://scan.coverity.com/projects/3468/badge.svg)](https://scan.coverity.com/projects/3468)

What is JBC?
------------
JBC is a simple C++ library for reading, writing, and otherwise manipulating Java Class files. It represents the fairly complex, flat format of Java Class files as a simple tree structure, without deviating too far from the original file structure. JBC resolves all indexed-references (when decoding from a file), replacing them with the referenced object for easy use and manipulation.
JBC is still in its alpha, and there's plenty more that needs to be done before it's finished.

How about a Sample?
-------------------
``` c++
int main(int argc, char **argv) {
    // Open a file.
	FILE *input;
	if((input = fopen(argv[1], "rb")) == NULL) {
		perror("fopen");
		return 1;
	}

	// Load the class.
	ClassFile *clazz = DecodeClassFile(input);

	// Lookup a field.
	MemberInfo *field = clazz->FindField("foo");
	cout << "Field descriptor for 'foo': " << field->Descriptor() << endl;

	// List class methods.
	for(auto itr = clazz->methods.begin(); itr != clazz->methods.end(); itr++) {
		cout << "Method: " << (*itr)->Name() << "." << endl;
	}

	// Done.
	delete clazz;
	return 0;
}
```

License
-------
JBC is distributed under the permissive MIT license, a copy of which may be found in the LICENSE file in the root directory of this project.
