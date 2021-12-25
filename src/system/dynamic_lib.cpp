#include "dynamic_lib.h"

using namespace _16nar;

/************************************************************************
** Constructor opens the dynamic library
*************************************************************************/
DynamicLib::DynamicLib(const string& lib_name) : name{lib_name} {
	handle = dlopen(name.c_str(), RTLD_LAZY);
	if (!handle) {
		throw std::runtime_error{"Cannot open library: " +
						string{dlerror()}};
	}
	dlerror();	// clear errors
}


/************************************************************************
** Move constructor
*************************************************************************/
DynamicLib::DynamicLib(DynamicLib&& lib) {
	std::swap(name, lib.name);
	std::swap(handle, lib.handle);
}


/************************************************************************
** Move assignment
*************************************************************************/
DynamicLib& DynamicLib::operator= (DynamicLib&& lib) {
	if (this != &lib) {
		handle = nullptr;
		std::swap(name, lib.name);
		std::swap(handle, lib.handle);
	}
	return *this;
}


/************************************************************************
** Load symbol and handle possible errors
*************************************************************************/
void *DynamicLib::get_symbol(const string& sym_name) const {
	void *sym = dlsym(handle, sym_name.c_str());
	const char *error = dlerror();
	if (error) {
		throw std::runtime_error{"Cannot load symbol " + sym_name +
				" from library " + name + ": " + error};
	}
	return sym;
}
