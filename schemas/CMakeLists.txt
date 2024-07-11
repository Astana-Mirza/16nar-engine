set(NARENGINE_SCHEMAS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/generated-includes/${NAME}/gen")
set(NARENGINE_FLATC_PATH "${FETCHCONTENT_BASE_DIR}/flatbuffers-build/flatc")

set(NARENGINE_SCHEMAS
    "${NARENGINE_SCHEMAS_DIR}/common.fbs"
    "${NARENGINE_SCHEMAS_DIR}/package_common.fbs"
    "${NARENGINE_SCHEMAS_DIR}/constructor_2d.fbs"
    "${NARENGINE_SCHEMAS_DIR}/package.fbs"
    "${NARENGINE_SCHEMAS_DIR}/resource.fbs"
)

set_source_files_properties("${NARENGINE_GEN_INCLUDE_DIR}" PROPERTIES GENERATED TRUE)

add_custom_target(generate-includes ALL DEPENDS flatbuffers "${NARENGINE_GEN_INCLUDE_DIR}")

add_custom_command(OUTPUT "${NARENGINE_GEN_INCLUDE_DIR}"
    DEPENDS "${NARENGINE_FLATC_PATH}" ${NARENGINE_SCHEMAS}
    COMMAND "${NARENGINE_FLATC_PATH}" "--cpp"
    "--scoped-enums" "--cpp-std" "c++17"
    "-I" "${NARENGINE_SCHEMAS_DIR}"
    "-o" "${NARENGINE_GEN_INCLUDE_DIR}"
    ${NARENGINE_SCHEMAS}
)

# Installation
install(DIRECTORY "${NARENGINE_GEN_INCLUDE_DIR}" DESTINATION "include/${NAME}")
