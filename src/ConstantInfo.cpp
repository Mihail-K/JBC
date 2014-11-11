
# include "ConstantInfo.h"

namespace JBC {

ConstantUtf8Info::~ConstantUtf8Info() {
	if(bytes != NULL) {
		delete bytes;
	}
}

} /* JBC */
