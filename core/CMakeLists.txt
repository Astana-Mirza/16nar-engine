set(NARENGINE_CORE_SRC_DIR "${PROJECT_SOURCE_DIR}/core/src")
set(NARENGINE_CORE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/core/include")

# Basic settings applied to all libraries on the layer
set(NARENGINE_CORE_COMMON_COMPILER_DEFS ${NARENGINE_COMMON_COMPILER_DEFS})
set(NARENGINE_CORE_COMMON_INCLUDE_DIRS ${NARENGINE_COMMON_INCLUDE_DIRS}
    PUBLIC "$<BUILD_INTERFACE:${NARENGINE_CORE_INCLUDE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:include>"
)
set(NARENGINE_CORE_COMMON_LINK_DIRS ${NARENGINE_COMMON_LINK_DIRS}
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:lib>"
)
set(NARENGINE_CORE_COMMON_LINK_LIBS ${NARENGINE_COMMON_LINK_LIBS})
configure_file("${NARENGINE_CORE_INCLUDE_DIR}/${NAME}/core/config.h.in" "${NAME}/core/config.h")


# Base Core layer library
set(NARENGINE_CORE_COMPILER_DEFS ${NARENGINE_CORE_COMMON_COMPILER_DEFS}
    PUBLIC "FLATBUFFERS_LOCALE_INDEPENDENT=0"
)
if ("${NARENGINE_WIN_EXPORT}")
    set(NARENGINE_CORE_COMPILER_DEFS ${NARENGINE_CORE_COMPILER_DEFS}
        PRIVATE "NARENGINE_CORE_EXPORT"
    )
endif() # if ("${NARENGINE_WIN_EXPORT}")
set(NARENGINE_CORE_INCLUDE_DIRS ${NARENGINE_CORE_COMMON_INCLUDE_DIRS}
    PRIVATE "${STB_INCLUDE_DIR}" "${PROJECT_BINARY_DIR}/schemas/flatbuffers-gen-cpp"
)
set(NARENGINE_CORE_LINK_DIRS ${NARENGINE_CORE_COMMON_LINK_DIRS})
set(NARENGINE_CORE_LINK_LIBS ${NARENGINE_CORE_COMMON_LINK_LIBS}
    PUBLIC "${NAME}_platform"
    PRIVATE "stb::stb" "flatbuffers::libflatbuffers"
)
set(NARENGINE_CORE_SOURCES
    "${NARENGINE_CORE_SRC_DIR}/assets/asset_iterator.cpp"

    "${NARENGINE_CORE_SRC_DIR}/resources/unified_storage.cpp"
    "${NARENGINE_CORE_SRC_DIR}/resources/resource_pkg_visitor.cpp"
    "${NARENGINE_CORE_SRC_DIR}/resources/stb_texture_reader.cpp"
    "${NARENGINE_CORE_SRC_DIR}/resources/stb_impl.cpp"
)
add_library("${NAME}_core" "${NARENGINE_LIB_TYPE}" ${NARENGINE_CORE_SOURCES})
add_dependencies("${NAME}_core" "flatbuffers-gen-cpp" "GENERATE_flatbuffers-gen-cpp")
narengine_add_deps("${NAME}_core" "NARENGINE_CORE")
set(NARENGINE_CORE_LIBS ${NARENGINE_CORE_LIBS} "${NAME}_core")


# JSON assets library
if ("${NARENGINE_ASSETS_JSON}")
    set(NARENGINE_ASSETS_JSON_COMPILER_DEFS ${NARENGINE_CORE_COMMON_COMPILER_DEFS})
    if ("${NARENGINE_WIN_EXPORT}")
        set(NARENGINE_ASSETS_JSON_COMPILER_DEFS ${NARENGINE_ASSETS_JSON_COMPILER_DEFS}
            PRIVATE "NARENGINE_ASSETS_JSON_EXPORT"
        )
    endif() # if ("${NARENGINE_WIN_EXPORT}")
    set(NARENGINE_ASSETS_JSON_INCLUDE_DIRS ${NARENGINE_CORE_COMMON_INCLUDE_DIRS})
    set(NARENGINE_ASSETS_JSON_LINK_DIRS ${NARENGINE_CORE_COMMON_LINK_DIRS})
    set(NARENGINE_ASSETS_JSON_LINK_LIBS ${NARENGINE_CORE_COMMON_LINK_LIBS}
        PUBLIC "${NAME}_core" "nlohmann_json::nlohmann_json" "flatbuffers::libflatbuffers"
    )
    set(NARENGINE_ASSETS_JSON_SOURCES
        "${NARENGINE_CORE_SRC_DIR}/assets/json/json_asset_reader.cpp"
        "${NARENGINE_CORE_SRC_DIR}/assets/json/json_asset_writer.cpp"
        "${NARENGINE_CORE_SRC_DIR}/assets/json/json_asset_file_processor.cpp"
        "${NARENGINE_CORE_SRC_DIR}/assets/json/json_to_flatbuffers_convertor.cpp"
    )

    add_library("${NAME}_assets_json" "${NARENGINE_LIB_TYPE}" ${NARENGINE_ASSETS_JSON_SOURCES})
    narengine_add_deps("${NAME}_assets_json" "NARENGINE_ASSETS_JSON")
    set(NARENGINE_CORE_LIBS ${NARENGINE_CORE_LIBS} "${NAME}_assets_json")
endif() # if ("${NARENGINE_ASSETS_JSON}")


# flatbuffers assets library
set(NARENGINE_ASSETS_FB_COMPILER_DEFS ${NARENGINE_CORE_COMMON_COMPILER_DEFS})
if ("${NARENGINE_WIN_EXPORT}")
    set(NARENGINE_ASSETS_FB_COMPILER_DEFS ${NARENGINE_ASSETS_FB_COMPILER_DEFS}
        PRIVATE "NARENGINE_ASSETS_FB_EXPORT"
    )
endif() # if ("${NARENGINE_WIN_EXPORT}")
set(NARENGINE_ASSETS_FB_INCLUDE_DIRS ${NARENGINE_CORE_COMMON_INCLUDE_DIRS}
    PRIVATE "${PROJECT_BINARY_DIR}/schemas/flatbuffers-gen-cpp"
)
set(NARENGINE_ASSETS_FB_LINK_DIRS ${NARENGINE_CORE_COMMON_LINK_DIRS})
set(NARENGINE_ASSETS_FB_LINK_LIBS ${NARENGINE_CORE_COMMON_LINK_LIBS}
    PUBLIC "${NAME}_core" "flatbuffers::libflatbuffers")
set(NARENGINE_ASSETS_FB_SOURCES
    "${NARENGINE_CORE_SRC_DIR}/assets/flatbuffers/flatbuffers_asset_reader.cpp"
    "${NARENGINE_CORE_SRC_DIR}/assets/flatbuffers/flatbuffers_asset_writer.cpp"
    "${NARENGINE_CORE_SRC_DIR}/assets/flatbuffers/flatbuffers_pmr_allocator.cpp"
    "${NARENGINE_CORE_SRC_DIR}/assets/flatbuffers/flatbuffers_asset_file_processor.cpp"
)
add_library("${NAME}_assets_fb" "${NARENGINE_LIB_TYPE}" ${NARENGINE_ASSETS_FB_SOURCES})
add_dependencies("${NAME}_assets_fb" "flatbuffers-gen-cpp" "GENERATE_flatbuffers-gen-cpp")
narengine_add_deps("${NAME}_assets_fb" "NARENGINE_ASSETS_FB")
set(NARENGINE_CORE_LIBS ${NARENGINE_CORE_LIBS} "${NAME}_assets_fb")


# Utilities
set(NARENGINE_CORE_ALL_TARGETS ${NARENGINE_CORE_ALL_TARGETS} ${NARENGINE_CORE_LIBS})
if ("${NARENGINE_BUILD_UTILS}")
    add_executable("${NAME}_asset_tool" "${NARENGINE_CORE_SRC_DIR}/utils/asset_tool.cpp")
    # target_compile_definitions("${NAME}_asset_tool" ${NARENGINE_CORE_COMMON_COMPILER_DEFS})
    target_include_directories("${NAME}_asset_tool" ${NARENGINE_CORE_COMMON_INCLUDE_DIRS})
    target_link_directories("${NAME}_asset_tool" ${NARENGINE_CORE_COMMON_LINK_DIRS})
    target_link_libraries("${NAME}_asset_tool" ${NARENGINE_CORE_COMMON_LINK_LIBS}
        PRIVATE "cxxopts::cxxopts" ${NARENGINE_CORE_LIBS}
    )
    set(NARENGINE_CORE_ALL_TARGETS ${NARENGINE_CORE_ALL_TARGETS} "${NAME}_asset_tool")
endif() # if ("${NARENGINE_BUILD_UTILS}")


# Testing
if ("${NARENGINE_BUILD_TESTS}")
    enable_testing()
    set(NARENGINE_CORE_TEST_SOURCES
        # test sources
        # "${NARENGINE_CORE_SRC_DIR}/test/some_test.cpp"
        ""
    )
    if ("${NARENGINE_ASSETS_JSON}")
        set(NARENGINE_CORE_TEST_SOURCES ${NARENGINE_CORE_TEST_SOURCES}
            "${NARENGINE_CORE_SRC_DIR}/test/assets/json_assets_test.cpp"
            "${NARENGINE_CORE_SRC_DIR}/test/assets/json_to_flatbuffers_convertor_test.cpp"
        )
    endif() # if ("${NARENGINE_ASSETS_JSON}")
    add_executable("${NAME}_core_test" ${NARENGINE_CORE_TEST_SOURCES})
    target_include_directories("${NAME}_core_test" ${NARENGINE_CORE_COMMON_INCLUDE_DIRS}
        PRIVATE ${CATCH2_INCLUDE_DIRS}
    )
    target_link_directories("${NAME}_core_test" ${NARENGINE_CORE_COMMON_LINK_DIRS})
    target_link_libraries("${NAME}_core_test" ${NARENGINE_CORE_COMMON_LINK_LIBS}
        PRIVATE ${NARENGINE_CORE_LIBS} "Catch2::Catch2WithMain"
    )
    message("PATH $$<TARGET_FILE_DIR:${NAME}_platform>")
    if("${WIN32}" AND "${CONAN_RUNTIME_LIB_DIRS}")
        set_tests_properties("${NAME}_core_test" PROPERTIES ENVIRONMENT_MODIFIED
            "PATH=path_list_prepend:${CONAN_RUNTIME_LIB_DIRS};${PROJECT_BINARY_DIR}/bin"
        )
    endif()
    file(COPY "${NARENGINE_CORE_SRC_DIR}/test/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME "${NAME}_core_test" COMMAND "${NAME}_core_test")
endif() # if ("${NARENGINE_BUILD_TESTS}")


# Installation
install(TARGETS ${NARENGINE_CORE_ALL_TARGETS} EXPORT "16nar")
install(DIRECTORY "${NARENGINE_CORE_INCLUDE_DIR}/${NAME}" DESTINATION "include")
install(FILES "${PROJECT_BINARY_DIR}/core/${NAME}/core/config.h" DESTINATION "include/${NAME}/core")
