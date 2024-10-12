set(NARENGINE_TOOLS_SRC_DIR "${PROJECT_SOURCE_DIR}/tools")
set(NARENGINE_TOOLS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/tools")
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
    "${NARENGINE_TOOLS_SRC_DIR}/flatbuffers_asset_writer.cpp"
    "${NARENGINE_TOOLS_SRC_DIR}/data_read_utils.cpp"
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


add_executable("${NAME}_asset_tool" "${NARENGINE_TOOLS_SRC_DIR}/asset_tool.cpp")
target_include_directories("${NAME}_asset_tool" PRIVATE
    ${NARENGINE_ENGINE_INCLUDE_DIRS} "${NARENGINE_TOOLS_INCLUDE_DIR}")
target_link_directories("${NAME}_asset_tool" PRIVATE ${NARENGINE_TOOLS_LINK_DIRS})
target_link_libraries("${NAME}_asset_tool" PUBLIC "${NAME}_tools")
set(NARENGINE_TOOLS_TARGETS ${NARENGINE_TOOLS_TARGETS} "${NAME}_asset_tool")

install(TARGETS ${NARENGINE_TOOLS_TARGETS} EXPORT "16nar")
install(DIRECTORY "${NARENGINE_TOOLS_SRC_DIR}"
    DESTINATION "include/${NAME}" FILES_MATCHING PATTERN "*.h")
