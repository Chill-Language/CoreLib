cmake_minimum_required(VERSION 3.0)
project(TinyChillParser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
	add_compile_options(/GR-)
else(MSVC)
	add_compile_options(-fno-rtti)
endif()

include_directories(Include)
add_subdirectory(Source/Lexer)
add_subdirectory(Source/Test)
