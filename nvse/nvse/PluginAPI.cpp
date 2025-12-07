#include "PluginAPI.h"

void NVSEArrayVarInterface::Element::Reset() {
	if (type == kType_String) 
		delete str;
	type = kType_Invalid; 
	str = NULL; 
}