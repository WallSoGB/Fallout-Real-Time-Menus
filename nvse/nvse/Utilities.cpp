#include "Utilities.h"
#include "tlhelp32.h"
#include "shared/BSMemory/BSMemory.hpp"

#pragma warning (push)
#pragma warning (disable : 4200)
struct RTTIType
{
	void	* typeInfo;
	uint32_t	pad;
	char	name[0];
};

struct RTTILocator
{
	uint32_t		sig, offset, cdOffset;
	RTTIType	* type;
};
#pragma warning (pop)

// use the RTTI information to return an object's class name
const char * GetObjectClassName(void * objBase)
{
	const char	* result = "<no rtti>";

	__try
	{
		void		** obj = (void **)objBase;
		RTTILocator	** vtbl = (RTTILocator **)obj[0];
		RTTILocator	* rtti = vtbl[-1];
		RTTIType	* type = rtti->type;

		// starts with ,?
		if((type->name[0] == '.') && (type->name[1] == '?'))
		{
			// is at most 100 chars long
			for(uint32_t i = 0; i < 100; i++)
			{
				if(type->name[i] == 0)
				{
					// remove the .?AV
					result = type->name + 4;
					break;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// return the default
	}

	return result;
}

char* CopyCString(const char* src) {
	const uint32_t size = src ? strlen(src) : 0;
	if (size == 0)
		return nullptr;

	char* result = BSMemory::malloc<char>(size+1);
	result[size] = 0;
	if (size) {
		strcpy_s(result, size+1, src);
	}

	return result;
}