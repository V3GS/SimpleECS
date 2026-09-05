cmake_minimum_required(VERSION 3.20)

# Project definition
project(SimpleECS VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# For Visual Studio multi-config
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Set the startup project for Visual Studio
set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT SimpleECS)

# Enable Testing
enable_testing()

# GoogleTest configuration
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Define header files
set(COMPONENT_HEADERS
    Source/Component/Component.h
    Source/Component/ComponentArray.h
    Source/Component/ComponentManager.h
    Source/Component/IComponentArray.h
)

set(ENTITY_HEADERS
    Source/Entity/Entity.h
    Source/Entity/EntityManager.h
)

set(SYSTEM_HEADERS
    Source/System/System.h
    Source/System/SystemManager.h
)

set(PROJECT_HEADERS
    Source/World.h
)

set(PROJECT_SOURCES
    Source/main.cpp
)

# Combine all headers and sources
set(ALL_HEADERS
    ${COMPONENT_HEADERS}
    ${ENTITY_HEADERS}
    ${SYSTEM_HEADERS}
    ${PROJECT_HEADERS}
)

set(ALL_SOURCES
    ${PROJECT_SOURCES}
    ${ALL_HEADERS}
)

# Create executable
add_executable(SimpleECS ${ALL_SOURCES})

# Include directories
target_include_directories(SimpleECS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Compiler-specific settings for Visual Studio
if(MSVC)
    # Use MultiThreaded DLL runtime
    target_compile_options(SimpleECS PRIVATE /MD$<$<CONFIG:Debug>:d>)
    
    # Warning level
    target_compile_options(SimpleECS PRIVATE /W4)
    
    # Enable parallel builds
    target_compile_options(SimpleECS PRIVATE /MP)
    
    # Treat warnings as errors (optional, remove if not desired)
    # target_compile_options(SimpleECS PRIVATE /WX)
endif()

# Source groups for Visual Studio (preserves folder structure in IDE)
source_group("Component" FILES ${COMPONENT_HEADERS})
source_group("Entity" FILES ${ENTITY_HEADERS})
source_group("System" FILES ${SYSTEM_HEADERS})
source_group("" FILES ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Set properties
set_target_properties(SimpleECS PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ============================================================================
# Tests Project - SimpleECS.Tests
# ============================================================================

# Define test source files
file(GLOB_RECURSE TEST_SOURCES "Tests/*.cpp")

# Create test executable
add_executable(SimpleECS.Tests ${TEST_SOURCES})

# Include directories for tests
target_include_directories(SimpleECS.Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests
)

# Link GoogleTest libraries
target_link_libraries(SimpleECS.Tests PRIVATE
    gtest
    gtest_main
)

# Compiler-specific settings for test project
if(MSVC)
    # Use MultiThreaded DLL runtime
    target_compile_options(SimpleECS.Tests PRIVATE /MD$<$<CONFIG:Debug>:d>)
    
    # Warning level
    target_compile_options(SimpleECS.Tests PRIVATE /W4)
    
    # Enable parallel builds
    target_compile_options(SimpleECS.Tests PRIVATE /MP)
endif()

# Set test project properties
set_target_properties(SimpleECS.Tests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(SimpleECS.Tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
