cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

project(BetterMenu VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
if(ANDROID)
CPMAddPackage("https://github.com/MuhXd/uibuilder_Docs#main")
else()
CPMAddPackage("gh:camila314/UIBuilder#main")
endif()
target_link_libraries(${PROJECT_NAME} UIBuilder)

setup_geode_mod(${PROJECT_NAME})
