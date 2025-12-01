cmake_minimum_required(VERSION 4.0)
project(bachelor)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SCENARIO_DIR "${SRC_DIR}/scenarios")
set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")

# All executables go into bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Compile model.cpp as an object library
add_library(objects OBJECT "${SRC_DIR}/model.cpp" "${SRC_DIR}/utils.cpp")

# Find all scenario files
file(GLOB SCENARIO_SOURCES "${SCENARIO_DIR}/scenario*.cpp")

# Collect scenario names
set(SCENARIO_NAMES "")

# Create one executable per scenario
foreach(SCENARIO ${SCENARIO_SOURCES})
    get_filename_component(SCENARIO_NAME ${SCENARIO} NAME_WE)
    add_executable(${SCENARIO_NAME} $<TARGET_OBJECTS:objects> ${SCENARIO})
    list(APPEND SCENARIO_NAMES ${SCENARIO_NAME})
endforeach()

# Meta-target that builds all scenarios
add_custom_target(bachelor ALL DEPENDS ${SCENARIO_NAMES})
add_executable(new_target src/utils.cpp)
