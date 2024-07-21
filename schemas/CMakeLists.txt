set(NARENGINE_SCHEMAS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/schemas/gen-cpp")

set(NARENGINE_SCHEMAS
    "common.fbs"
    "package_common.fbs"
    "constructor_2d.fbs"
    "package.fbs"
    "resource.fbs"
)

set_source_files_properties("${NARENGINE_GEN_INCLUDE_DIR}" PROPERTIES GENERATED TRUE)

flatbuffers_generate_headers(TARGET gen-cpp
    INCLUDE "${NARENGINE_SCHEMAS_DIR}"
    INCLUDE_PREFIX "${NAME}/gen"
    SCHEMAS ${NARENGINE_SCHEMAS}
    FLAGS "--cpp" "--scoped-enums" "--cpp-std=c++17"
)

# Installation
install(DIRECTORY "${NARENGINE_GEN_INCLUDE_DIR}/${NAME}" DESTINATION "include")
