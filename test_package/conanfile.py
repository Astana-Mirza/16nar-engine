import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class NarengineTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if os.getenv("DISPLAY") and self.settings.os != "Windows" and can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "test_package --no-window")
            self.run(cmd, env="conanrun")
