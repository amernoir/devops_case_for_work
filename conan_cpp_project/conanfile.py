from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import check_min_cpp_standard

class JsonFormatterRecipe(ConanFile):
    name = "json_formatter"
    version = "1.0.0"
    package_type = "application"
    
    # Настройки
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    
    compiler = "cppstd"
    
    def requirements(self):
        self.requires("nlohmann_json/3.11.3")
        self.requires("fmt/10.2.1")
        self.requires("spdlog/1.14.1")
    
    def validate(self):
        check_min_cpp_standard(self, 17)
    
    def layout(self):
        cmake_layout(self)
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
