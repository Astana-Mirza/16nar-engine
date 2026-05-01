from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cppstd
import re


class NarengineRecipe(ConanFile):
    name = "16nar_engine"
    version = "0.1.0-1"

    url = "https://github.com/Astana-Mirza/16nar-engine"

    exports_sources = "CMakeLists.txt", "engine/*", "schemas/*", "tools/*"

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "log_level": [1, 2, 3, 4, 5, 6, 7, 8, 9],
        "with_utils": [True, False],
        "with_tools_json": [True, False],
        "with_tools_flatbuffers": [True, False],
        "with_render_opengl": [True, False],
        "with_arch_constructor2d": [True, False]
    }
    default_options = {
        "shared": True,
        "log_level": 9,
        "with_utils": True,
        "with_tools_json": True,
        "with_tools_flatbuffers": True,
        "with_render_opengl": True,
        "with_arch_constructor2d": True
    }

    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("stb/cci.20240213")
        self.requires("flatbuffers/24.3.25")
        if self.options.with_tools_json:
            self.requires("nlohmann_json/3.11.3")
        if self.options.with_utils:
            self.requires("cxxopts/3.3.1")
        if self.options.with_render_opengl:
            self.requires("opengl/system")

        self.test_requires("catch2/3.6.0")

    def validate(self):
        check_min_cppstd(self, "17")

    def layout(self):
        cmake_layout(self)

    def build(self):
        version_items = re.split(r'\.|-|\+', self.version)
        cmake = CMake(self)
        cmake.configure({
            "NARENGINE_LOG_LEVEL": self.options.log_level,
            "NARENGINE_BUILD_UTILS": "ON" if self.options.with_utils else "OFF",
            "NARENGINE_TOOLS_JSON": "ON" if self.options.with_tools_json else "OFF",
            "NARENGINE_TOOLS_FLATBUFFERS": "ON" if self.options.with_tools_flatbuffers else "OFF",
            "NARENGINE_RENDER_OPENGL": "ON" if self.options.with_render_opengl else "OFF",
            "NARENGINE_BUILD_CONSTRUCTOR2D": "ON" if self.options.with_arch_constructor2d else "OFF",
            "NARENGINE_VERSION_MAJOR": version_items[ 0 ],
            "NARENGINE_VERSION_MINOR": version_items[ 1 ],
            "NARENGINE_VERSION_PATCH": version_items[ 2 ],
        })
        cmake.build()
        if self.settings.os != "Windows":
            # tests need libaries from different directories, which is impossible on Windows (Windows sucks)
            # tests will be run only if tools.build:skip_test option is false (default is false)
            cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def add_package_component(self, name, requires = [], system_libs = []):
        nmspc_name = "16nar"
        self.cpp_info.components[name].libs = [name]
        self.cpp_info.components[name].system_libs = system_libs
        self.cpp_info.components[name].requires = requires
        self.cpp_info.components[name].set_property("cmake_target_name", nmspc_name + "::" + name)


    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "16nar")
        self.cpp_info.set_property("cmake_find_mode", "both")

        # tools
        self.add_package_component("16nar_tools", [], ["flatbuffers::libflatbuffers"])

        if self.options.with_tools_json:
            self.add_package_component("16nar_tools_json", ["16nar_tools"])
        if self.options.with_tools_flatbuffers:
            self.add_package_component("16nar_tools_fb", ["16nar_tools"], ["flatbuffers::libflatbuffers"])
        if self.options.with_arch_constructor2d:
            self.add_package_component("16nar_tools_constructor2d",
                ["16nar_tools"], ["flatbuffers::libflatbuffers"])

        # engine
        self.add_package_component("16nar_math", [], ["glm::glm"])
        self.add_package_component("16nar_base", ["16nar_math"], ["glfw"])

        if self.options.with_render_opengl:
            self.add_package_component("16nar_render_gl", ["16nar_base"], ["opengl::opengl"])

        if self.options.with_arch_constructor2d:
            self.add_package_component("16nar_constructor2d", ["16nar_base"])

