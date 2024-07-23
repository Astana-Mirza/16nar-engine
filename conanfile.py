from conan import ConanFile
from conan.tools.cmake import cmake_layout


class NarengineRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("opengl/system")
        self.requires("glm/1.0.1")
        self.requires("glfw/3.4")
        self.requires("flatbuffers/24.3.25")
        self.requires("nlohmann_json/3.11.3")
        self.requires("stb/cci.20240213")
        self.requires("catch2/3.6.0")


    def layout(self):
        cmake_layout(self)
