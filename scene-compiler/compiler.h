#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
#include "../scene_file.h"

using nlohmann::json;

void compile_scene(const std::string& scene_filename,
			const std::string& out_filename);

void compile_package(const std::string& pck_filename,
			const std::string& out_filename);

#endif // COMPILER_H
