from conans import ConanFile, CMake, tools

class ChernoOpenGLConan(ConanFile):
	name = "cherno-opengl"
	generators = ["cmake", "virtualenv"]
	requires = (
		"glew/2.1.0",
		"glfw/3.3.2",
		"glm/0.9.9.8",
		"fmt/7.0.3",
		"spdlog/1.8.0",
		"stb/20200203"
	)
	build_requires = (
		"cmake-tools/1.0.0@puya/testing"
	)

	options = {"shared": [True, False], "fPIC": [True, False]}
	default_options = {"shared": False, "fPIC": True}
