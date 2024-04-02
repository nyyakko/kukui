add_subdirectory(source)
add_subdirectory(include/${PROJECT_NAME})

add_executable(${PROJECT_NAME} "${kukui_SourceFiles}")

if (ENABLE_CLANGTIDY)
    enable_clang_tidy()
endif()

if (ENABLE_CPPCHECK)
    enable_cppcheck()
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${kukui_HeaderFiles})

export(TARGETS      ${PROJECT_NAME}
       FILE         ${EXPORT_DIR}/${PROJECT_NAME}Targets.cmake
       NAMESPACE    ${PROJECT_NAME}::
)

install(TARGETS     ${PROJECT_NAME}
        EXPORT      ${PROJECT_NAME}Targets
        ARCHIVE
        FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT      ${PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake
        NAMESPACE   ${PROJECT_NAME}::
)

install(FILES ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)

target_link_options(${PROJECT_NAME} PRIVATE ${kukui_LinkerOptions})
target_compile_options(${PROJECT_NAME} PRIVATE ${kukui_CompilerOptions})
target_link_libraries(${PROJECT_NAME} PRIVATE ${kukui_ExternalLibraries})
