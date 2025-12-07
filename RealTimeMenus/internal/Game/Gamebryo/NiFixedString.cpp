#include "NiFixedString.hpp"

NiFixedString::operator const char* () const {
	return m_kHandle;
}
