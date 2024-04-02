cmake_minimum_required(VERSION 3.25)

project(kukui LANGUAGES CXX)

set(EXPORT_DIR "${CMAKE_SOURCE_DIR}/build/cmake")

include(cmake/static_analyzers.cmake)
include(cmake/CPM.cmake)
include(cmake/remote_clone.cmake)
include(GNUInstallDirs)

option(ENABLE_CLANGTIDY "" TRUE)
option(ENABLE_CPPCHECK "" TRUE)

set(kukui_CompilerOptions ${kukui_CompilerOptions} -Wno-gnu-statement-expression-from-macro-expansion)
# set(kukui_LinkerOptions ${kukui_LinkerOptions})

CPMAddPackage("gh:nyyakko/LibError#4f44ec6")
CPMAddPackage("gh:nyyakko/LibPreprocessor#88243c4")
CPMAddPackage("gh:jarro2783/cxxopts@3.0.0")

set(kukui_ExternalLibraries cxxopts LibError::LibError LibPreprocessor::LibPreprocessor)

add_subdirectory(kukui)
