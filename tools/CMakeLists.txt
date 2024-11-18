set(NARENGINE_TOOLS_SRC_DIR "${PROJECT_SOURCE_DIR}/tools/src")
set(NARENGINE_TOOLS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/tools/include")
set(NARENGINE_ENGINE_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/engine/include"
    "${PROJECT_BINARY_DIR}/engine"
)
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/schemas/gen-cpp")

if ("${BUILD_SHARED_LIBS}")
    if ("${WIN32}")
        set(NARENGINE_TOOLS_COMPILER_DEFS ${NARENGINE_TOOLS_COMPILER_DEFS}
            "NARENGINE_DLL"
        )
    endif() # if ("${WIN32}")
endif() # if ("${BUILD_SHARED_LIBS}")

set(NARENGINE_TOOLS_LINK_LIBS "flatbuffers::libflatbuffers" "stb::stb" "nlohmann_json::nlohmann_json" "glm::glm")
set(NARENGINE_TOOLS_LINK_DIRS "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" "$<INSTALL_INTERFACE:lib>")
set(NARENGINE_TOOLS_SOURCES
    "${NARENGINE_TOOLS_SRC_DIR}/json_asset_reader.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/json_asset_writer.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers_asset_reader.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers_asset_writer.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/binary_utils.cpp"
)
add_library("${NAME}_tools" "${NARENGINE_LIB_TYPE}" ${NARENGINE_TOOLS_SOURCES})
add_dependencies("${NAME}_tools" "gen-cpp" "GENERATE_gen-cpp")
target_include_directories("${NAME}_tools"
    PRIVATE "${NARENGINE_TOOLS_INCLUDE_DIR}" "${NARENGINE_GEN_INCLUDE_DIR}"
    PUBLIC "$<BUILD_INTERFACE:${NARENGINE_ENGINE_INCLUDE_DIRS}>" "$<INSTALL_INTERFACE:include>")
target_link_directories("${NAME}_tools" PUBLIC ${NARENGINE_TOOLS_LINK_DIRS})
target_compile_definitions("${NAME}_tools" PRIVATE ${NARENGINE_TOOLS_COMPILER_DEFS})
target_link_libraries("${NAME}_tools" PRIVATE ${NARENGINE_TOOLS_LINK_LIBS})
set(NARENGINE_TOOLS_TARGETS ${NARENGINE_TOOLS_TARGETS} "${NAME}_tools")

if ("${NARENGINE_BUILD_UTILS}")
    add_executable("${NAME}_asset_tool" "${NARENGINE_TOOLS_SRC_DIR}/asset_tool.cpp")
    target_include_directories("${NAME}_asset_tool" PRIVATE
        ${NARENGINE_ENGINE_INCLUDE_DIRS} "${NARENGINE_TOOLS_INCLUDE_DIR}")
    target_link_directories("${NAME}_asset_tool" PRIVATE ${NARENGINE_TOOLS_LINK_DIRS})
    target_link_libraries("${NAME}_asset_tool" PUBLIC "${NAME}_tools")
    set(NARENGINE_TOOLS_TARGETS ${NARENGINE_TOOLS_TARGETS} "${NAME}_asset_tool")
endif() # if ("${NARENGINE_BUILD_UTILS}")


# Testing
if ("${NARENGINE_BUILD_TESTS}")
    enable_testing()
    add_executable("${NAME}_tools_assets_test"
        "${NARENGINE_TOOLS_SRC_DIR}/test/json_resources_test.cpp"
        "${NARENGINE_TOOLS_SRC_DIR}/test/flatbuffers_resources_test.cpp"
    )
    target_include_directories("${NAME}_tools_assets_test" PRIVATE
        "${NARENGINE_TOOLS_INCLUDE_DIR}" ${NARENGINE_ENGINE_INCLUDE_DIRS} ${CATCH2_INCLUDE_DIRS})
    target_link_directories("${NAME}_tools_assets_test" PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries("${NAME}_tools_assets_test" PRIVATE
        "${NAME}_tools" ${NARENGINE_TOOLS_LINK_LIBS} "Catch2::Catch2WithMain")
    file(COPY "${NARENGINE_TOOLS_SRC_DIR}/test/data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME "${NAME}_tools_assets_test" COMMAND "${NAME}_tools_assets_test")
endif() # if ("${NARENGINE_BUILD_TESTS}")


# Installation
install(TARGETS ${NARENGINE_TOOLS_TARGETS} EXPORT "16nar")
install(DIRECTORY "${NARENGINE_TOOLS_INCLUDE_DIR}/${NAME}" DESTINATION "include")
