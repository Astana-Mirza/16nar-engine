#ifndef _16NAR_DYNAMIC_RESOURCE_MANAGER_H
#define _16NAR_DYNAMIC_RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include <stdexcept>
#include "../16nardefs.h"

namespace _16nar {

template <typename T>
class DynamicResourceManager {
public:
	static DynamicResourceManager& instance() {
		static DynamicResourceManager obj;
		return obj;
	}

	DynamicResourceManager(DynamicResourceManager const&)	= delete;
	void operator=(DynamicResourceManager const&)		= delete;

	void load(const std::string& name) {
		if (!resources[name].loadFromFile(name)) {
			throw std::runtime_error{
				"Cannot load resource from file " + name};
		}
	}

	const T& get(const std::string& name) {
		auto it = resources.find(name);
		if (it == resources.end()) {
			load(name);
			return resources.at(name);
		}
		return it->second;
	}

	void free(const std::string& name) {
		resources.erase(name);
	}

private:
	std::unordered_map<std::string, T> resources;

	DynamicResourceManager() =default;
};

}

#endif
