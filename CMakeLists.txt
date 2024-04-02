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

CPMAddPackage("gh:nyyakko/LibError#a664747")
CPMAddPackage("gh:nyyakko/LibPreprocessor#b3169c8")
CPMAddPackage("gh:jarro2783/cxxopts@3.0.0")

set(kukui_ExternalLibraries cxxopts LibError::LibError LibPreprocessor::LibPreprocessor)

add_subdirectory(kukui)
