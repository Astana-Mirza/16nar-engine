from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cppstd, can_run
import re


class NarengineRecipe(ConanFile):
    name = "16nar_engine"
    version = "0.1.0-1"

    url = "https://github.com/Astana-Mirza/16nar-engine"

    exports_sources = "CMakeLists.txt", "cmake/*", "schemas/*", "platform/*", "core/*", "plugins/*"

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "log_level": [1, 2, 3, 4, 5, 6, 7, 8, 9],
        "enable_tests": [True, False],
        "with_utils": [True, False],
        "with_assets_json": [True, False],
        "with_render_vulkan": [True, False],
        "with_render_opengl": [True, False]
    }
    default_options = {
        "shared": True,
        "log_level": 9,
        "enable_tests": True,
        "with_utils": True,
        "with_assets_json": True,
        "with_render_vulkan": True,
        "with_render_opengl": False
    }

    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("flatbuffers/24.3.25", transitive_headers=True)
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("glfw/3.4", visible=False)
        self.requires("stb/cci.20240213", visible=False)
        if self.options.with_assets_json:
            self.requires("nlohmann_json/3.11.3", transitive_headers=True)
        if self.options.with_utils:
            self.requires("cxxopts/3.3.1", visible=False)
        if self.options.with_render_opengl:
            self.requires("opengl/system")
        if self.options.enable_tests:
            self.test_requires("catch2/3.6.0")

    def validate(self):
        check_min_cppstd(self, "17")

    def layout(self):
        cmake_layout(self)

    def configure(self):
        if self.options.shared:
            self.package_type = "shared-library"
        else:
            self.package_type = "static-library"

    def build(self):
        version_items = re.split(r'\.|-|\+', self.version)
        cmake = CMake(self)
        cmake.configure({
            "NARENGINE_LOG_LEVEL": self.options.log_level,
            "NARENGINE_BUILD_UTILS": "ON" if self.options.with_utils else "OFF",
            "NARENGINE_ASSETS_JSON": "ON" if self.options.with_assets_json else "OFF",
            "NARENGINE_RENDER_VULKAN": "ON" if self.options.with_render_vulkan else "OFF",
            "NARENGINE_RENDER_OPENGL": "ON" if self.options.with_render_opengl else "OFF",
            "NARENGINE_VERSION_MAJOR": version_items[ 0 ],
            "NARENGINE_VERSION_MINOR": version_items[ 1 ],
            "NARENGINE_VERSION_PATCH": version_items[ 2 ],
        })
        cmake.build()
        if self.options.enable_tests and can_run(self):
            cmake.test(cli_args=["--verbose"])

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

        # Platform
        self.add_package_component("16nar_platform", ["glm::glm"])

        # Core
        self.add_package_component("16nar_core", ["16nar_platform"])
        self.add_package_component("16nar_assets_fb", [
            "16nar_core",
            "flatbuffers::libflatbuffers"
        ])
        if self.options.with_assets_json:
            self.add_package_component("16nar_assets_json", [
                "16nar_core",
                "nlohmann_json::nlohmann_json",
                "flatbuffers::libflatbuffers"
            ])

        # Plugins
        #if self.options.with_render_vulkan:
        #    self.add_package_component("16nar_render_vulkan", ["16nar_core"])
        #if self.options.with_render_opengl:
        #    self.add_package_component("16nar_render_opengl", ["16nar_core"])

