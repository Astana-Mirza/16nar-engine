from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cppstd


class NarengineRecipe(ConanFile):
    name = "16nar_engine"
    version = "0.0.0-1"

    url = "https://github.com/Astana-Mirza/16nar-engine"

    exports_sources = "CMakeLists.txt", "engine/*", "schemas/*", "tools/*"

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "log_level": [1, 2, 3, 4, 5, 6, 7, 8, 9],
        "with_tools": [True, False],
        "with_render_opengl": [True, False],
        "with_arch_constructor2d": [True, False]
    }
    default_options = {
        "shared": True,
        "log_level": 9,
        "with_tools": True,
        "with_render_opengl": True,
        "with_arch_constructor2d": True
    }

    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("opengl/system")
        self.requires("glfw/3.4")
        self.requires("flatbuffers/24.3.25")
        self.requires("nlohmann_json/3.11.3")
        self.requires("stb/cci.20240213")

        # public dependencies
        self.requires("glm/1.0.1", transitive_headers=True)

        self.test_requires("catch2/3.6.0")

    def validate(self):
        check_min_cppstd(self, "17")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure({
            "NARENGINE_RENDER_OPENGL": "ON" if self.options.with_render_opengl else "OFF",
            "NARENGINE_LOG_LEVEL": self.options.log_level,
            "NARENGINE_BUILD_TOOLS": "ON" if self.options.with_tools else "OFF",
            "NARENGINE_BUILD_CONSTRUCTOR2D": "ON" if self.options.with_arch_constructor2d else "OFF"
        })
        cmake.build()
        # tests will be run only if tools.build:skip_test option is false (default is true)
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        nmspc_name = "16nar"
        self.cpp_info.set_property("cmake_file_name", "16nar")
        self.cpp_info.set_property("cmake_find_mode", "both")

        self.cpp_info.components["16nar_logger"].libs = ["16nar_logger"]
        self.cpp_info.components["16nar_logger"].set_property("cmake_target_name",
            nmspc_name + "::16nar_logger")

        self.cpp_info.components["16nar_math"].libs = ["16nar_math"]
        self.cpp_info.components["16nar_math"].system_libs = ["glm::glm"]
        self.cpp_info.components["16nar_math"].requires = ["16nar_logger"]
        self.cpp_info.components["16nar_math"].set_property("cmake_target_name",
            nmspc_name + "::16nar_math")

        self.cpp_info.components["16nar_base"].libs = ["16nar_base"]
        self.cpp_info.components["16nar_base"].system_libs = ["glfw"]
        self.cpp_info.components["16nar_base"].requires = ["16nar_math"]
        self.cpp_info.components["16nar_base"].set_property("cmake_target_name",
            nmspc_name + "::16nar_base")

        if self.options.with_tools:
            self.cpp_info.components["16nar_tools"].libs = ["16nar_tools"]
            self.cpp_info.components["16nar_tools"].set_property("cmake_target_name",
                nmspc_name + "::16nar_tools")

        if self.options.with_render_opengl:
            self.cpp_info.components["16nar_render_gl"].libs = ["16nar_render_gl"]
            self.cpp_info.components["16nar_render_gl"].system_libs = ["opengl::opengl"]
            self.cpp_info.components["16nar_render_gl"].requires = ["16nar_base"]
            self.cpp_info.components["16nar_render_gl"].set_property("cmake_target_name",
                nmspc_name + "::16nar_render_gl")

        if self.options.with_arch_constructor2d:
            self.cpp_info.components["16nar_constructor2d"].libs = ["16nar_constructor2d"]
            self.cpp_info.components["16nar_constructor2d"].requires = ["16nar_base"]
            self.cpp_info.components["16nar_constructor2d"].set_property("cmake_target_name",
                nmspc_name + "::16nar_constructor2d")
