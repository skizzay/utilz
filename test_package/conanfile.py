import os

from conans import ConanFile, CMake, tools


class UtilzTestConan(ConanFile):
    generators = "cmake"
    build_requires = "catch2/2.2.2@bincrafters/stable", "cmake_installer/3.11.2@conan/stable"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._cmake = None

    def build(self):
        self._cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
        # in "test_package"
        self._cmake.configure()
        self._cmake.build()

    def test(self):
        self._cmake.test()
