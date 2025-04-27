set(NARENGINE_TOOLS_SRC_DIR "${PROJECT_SOURCE_DIR}/tools/src")
set(NARENGINE_TOOLS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/tools/include")

# Basic settings
if ("${BUILD_SHARED_LIBS}" AND "${WIN32}")
    set(NARENGINE_TOOLS_COMMON_COMPILER_DEFS ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS}
        "NARENGINE_DLL"
    )
endif() # if ("${BUILD_SHARED_LIBS}" AND "${WIN32}")
set(NARENGINE_TOOLS_COMMON_INCLUDE_DIRS
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/engine/include>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tools/include>"
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:include>"
)
set(NARENGINE_TOOLS_COMMON_LINK_DIRS "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" "$<INSTALL_INTERFACE:lib>")

# Base tools library
set(NARENGINE_TOOLS_COMPILER_DEFS PRIVATE ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS})
set(NARENGINE_TOOLS_INCLUDE_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS})
set(NARENGINE_TOOLS_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
set(NARENGINE_TOOLS_LINK_LIBS PUBLIC "glm::glm")
set(NARENGINE_TOOLS_SOURCES
    "${NARENGINE_TOOLS_SRC_DIR}/utils.cpp"
)
add_library("${NAME}_tools" "${NARENGINE_LIB_TYPE}" ${NARENGINE_TOOLS_SOURCES})
target_include_directories("${NAME}_tools" ${NARENGINE_TOOLS_INCLUDE_DIRS})
target_link_directories("${NAME}_tools" ${NARENGINE_TOOLS_LINK_DIRS})
target_compile_definitions("${NAME}_tools" ${NARENGINE_TOOLS_COMPILER_DEFS})
target_link_libraries("${NAME}_tools" ${NARENGINE_TOOLS_LINK_LIBS})
set(NARENGINE_TOOLS_ALL_TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} "${NAME}_tools")


if ("${NARENGINE_TOOLS_JSON}")
    # Json assets library
    set(NARENGINE_ASSETS_JSON_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    set(NARENGINE_ASSETS_JSON_INCLUDE_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS})
    set(NARENGINE_ASSETS_JSON_LINK_LIBS PUBLIC "${NAME}_tools" PRIVATE "stb::stb" "nlohmann_json::nlohmann_json")
    set(NARENGINE_ASSETS_JSON_COMPILER_DEFS PRIVATE ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS})
    set(NARENGINE_ASSETS_JSON_SOURCES
        "${NARENGINE_TOOLS_SRC_DIR}/assets/json/json_props_reader.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/json/json_props_writer.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/json/json_asset_reader.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/json/json_asset_writer.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/stb_impl.cpp"
    )
    add_library("${NAME}_assets_json" "${NARENGINE_LIB_TYPE}" ${NARENGINE_ASSETS_JSON_SOURCES})
    target_include_directories("${NAME}_assets_json" ${NARENGINE_ASSETS_JSON_INCLUDE_DIRS})
    target_link_directories("${NAME}_assets_json" ${NARENGINE_ASSETS_JSON_LINK_DIRS})
    target_compile_definitions("${NAME}_assets_json" ${NARENGINE_ASSETS_JSON_COMPILER_DEFS})
    target_link_libraries("${NAME}_assets_json" ${NARENGINE_ASSETS_JSON_LINK_LIBS})
    set(NARENGINE_TOOLS_ASSETS_LIBS ${NARENGINE_TOOLS_ASSETS_LIBS} "${NAME}_assets_json")
endif() # if ("${NARENGINE_TOOLS_JSON}")


if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    # Flatbuffers assets library
    set(NARENGINE_ASSETS_FB_LINK_DIRS PUBLIC ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    set(NARENGINE_ASSETS_FB_INCLUDE_DIRS
        PUBLIC ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS}
        PRIVATE "${PROJECT_BINARY_DIR}/schemas/flatbuffers-gen-cpp"
    )
    set(NARENGINE_ASSETS_FB_COMPILER_DEFS PRIVATE ${NARENGINE_TOOLS_COMMON_COMPILER_DEFS})
    set(NARENGINE_ASSETS_FB_LINK_LIBS PUBLIC "glm::glm" "flatbuffers::libflatbuffers")
    set(NARENGINE_ASSETS_FB_SOURCES
        "${NARENGINE_TOOLS_SRC_DIR}/assets/flatbuffers/flatbuffers_props_reader.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/flatbuffers/flatbuffers_props_writer.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/flatbuffers/flatbuffers_asset_reader.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/assets/flatbuffers/flatbuffers_asset_writer.cpp"
    )
    add_library("${NAME}_assets_fb" "${NARENGINE_LIB_TYPE}" ${NARENGINE_ASSETS_FB_SOURCES})
    add_dependencies("${NAME}_assets_fb" "flatbuffers-gen-cpp" "GENERATE_flatbuffers-gen-cpp")
    target_include_directories("${NAME}_assets_fb" ${NARENGINE_ASSETS_FB_INCLUDE_DIRS})
    target_link_directories("${NAME}_assets_fb" ${NARENGINE_ASSETS_FB_LINK_DIRS})
    target_compile_definitions("${NAME}_assets_fb" ${NARENGINE_ASSETS_FB_COMPILER_DEFS})
    target_link_libraries("${NAME}_assets_fb" ${NARENGINE_ASSETS_FB_LINK_LIBS})
    set(NARENGINE_TOOLS_ASSETS_LIBS ${NARENGINE_TOOLS_ASSETS_LIBS} "${NAME}_assets_fb")
endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")


set(NARENGINE_TOOLS_ALL_TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} ${NARENGINE_TOOLS_ASSETS_LIBS})
if ("${NARENGINE_BUILD_UTILS}")
    add_executable("${NAME}_asset_tool" "${NARENGINE_TOOLS_SRC_DIR}/asset_tool.cpp")
    target_include_directories("${NAME}_asset_tool" PRIVATE
        ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS} "${NARENGINE_TOOLS_INCLUDE_DIR}")
    target_link_directories("${NAME}_asset_tool" PRIVATE ${NARENGINE_TOOLS_COMMON_LINK_DIRS})
    target_link_libraries("${NAME}_asset_tool" PUBLIC "${NAME}_tools" ${NARENGINE_TOOLS_ASSETS_LIBS})
    set(NARENGINE_TOOLS_ALL_TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} "${NAME}_asset_tool")
endif() # if ("${NARENGINE_BUILD_UTILS}")


# Testing
if ("${NARENGINE_BUILD_TESTS}")
    enable_testing()
    if ("${NARENGINE_TOOLS_JSON}")
        set(NARENGINE_RESOURCES_TEST_SOURCES ${NARENGINE_RESOURCES_TEST_SOURCES}
            "${NARENGINE_TOOLS_SRC_DIR}/assets/test/json_props_test.cpp"
            "${NARENGINE_TOOLS_SRC_DIR}/assets/test/json_resources_test.cpp"
        )
        # flatbuffers resource test depends on JSON
        if ("${NARENGINE_TOOLS_FLATBUFFERS}")
            set(NARENGINE_RESOURCES_TEST_SOURCES ${NARENGINE_RESOURCES_TEST_SOURCES}
                "${NARENGINE_TOOLS_SRC_DIR}/assets/test/flatbuffers_resources_test.cpp"
            )
        endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    endif() # if ("${NARENGINE_TOOLS_JSON}")
    if ("${NARENGINE_TOOLS_FLATBUFFERS}")
        set(NARENGINE_RESOURCES_TEST_SOURCES ${NARENGINE_RESOURCES_TEST_SOURCES}
            "${NARENGINE_TOOLS_SRC_DIR}/assets/test/flatbuffers_props_test.cpp"
        )
    endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    add_executable("${NAME}_tools_assets_test" ${NARENGINE_RESOURCES_TEST_SOURCES})
    target_include_directories("${NAME}_tools_assets_test" PRIVATE
        "${NARENGINE_TOOLS_INCLUDE_DIR}" ${NARENGINE_TOOLS_COMMON_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories("${NAME}_tools_assets_test" PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries("${NAME}_tools_assets_test" PRIVATE
        "${NAME}_tools" ${NARENGINE_TOOLS_ASSETS_LIBS} "Catch2::Catch2WithMain" "nlohmann_json::nlohmann_json")
    file(COPY "${NARENGINE_TOOLS_SRC_DIR}/assets/test/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME "${NAME}_tools_assets_test" COMMAND "${NAME}_tools_assets_test")
endif() # if ("${NARENGINE_BUILD_TESTS}")


# Installation
install(TARGETS ${NARENGINE_TOOLS_ALL_TARGETS} EXPORT "16nar")
install(DIRECTORY "${NARENGINE_TOOLS_INCLUDE_DIR}/${NAME}" DESTINATION "include")
