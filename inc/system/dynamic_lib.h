#ifndef _16NAR_DYNAMIC_LIB_H
#define _16NAR_DYNAMIC_LIB_H

#include <string>
#include <stdexcept>
#include <dlfcn.h>

namespace _16nar {

class DynamicLib {
public:
	DynamicLib(const string& lib_name);
	// no copy construction
	DynamicLib(const DynamicLib&) =delete;
	DynamicLib& operator= (const DynamicLib&) =delete;
	// move construction
	DynamicLib(DynamicLib&& lib);
	DynamicLib& operator= (DynamicLib&& lib);
	~DynamicLib() { if (handle) dlclose(handle); }

	void *get_symbol(const string& sym_name) const;

private:
	std::string name;
	void *handle = nullptr;
};

}

#endif	// #ifndef _16NAR_DYNAMIC_LIB_H
