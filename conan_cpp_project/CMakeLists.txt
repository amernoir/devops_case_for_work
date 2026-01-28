cmake_minimum_required(VERSION 3.15)
project(JsonFormatter VERSION 1.0.0 LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включение Conan
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Настройка предупреждений
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/json_processor.cpp
)

# Заголовочные файлы
set(HEADERS
    include/json_processor.h
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Подключение зависимостей через Conan
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

# Линковка библиотек
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
)

# Установка
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
