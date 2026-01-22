cmake_minimum_required(VERSION 3.10)
project(sqlite3_amalgamation LANGUAGES C)

# Опция для включения/отключения сборки библиотеки
option(BUILD_SHARED_LIBS "Build shared libraries (DLL/.so)" ON)

# Создаем библиотеку из амальгамированного исходного файла
add_library(sqlite3
    sqlite3.c
)

# Для сборки DLL в Windows требуется определить макрос `SQLITE_API` как `__declspec(dllexport)`
if(WIN32 AND BUILD_SHARED_LIBS)
    target_compile_definitions(sqlite3 PRIVATE SQLITE_API=__declspec(dllexport))
endif()

# Публичный заголовочный файл
target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Рекомендованные флаги компиляции для оптимизации под размер, как указано в руководстве
if(MSVC)
    target_compile_options(sqlite3 PRIVATE /Os)
else()
    target_compile_options(sqlite3 PRIVATE -Os)
endif()

# Опционально: определение общих для платформ флагов компиляции, упомянутых в руководстве
# Например, для отключения потоковой безопасности или включения расширений.
# target_compile_definitions(sqlite3 PRIVATE SQLITE_THREADSAFE=0 SQLITE_OMIT_LOAD_EXTENSION)

# Установка выходного имени библиотеки (опционально, для ясности)
set_target_properties(sqlite3 PROPERTIES OUTPUT_NAME "sqlite3")
