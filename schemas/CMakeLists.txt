set(NARENGINE_SCHEMAS_DIR "${PROJECT_SOURCE_DIR}/schemas")
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/schemas/gen-cpp")

set_source_files_properties("${NARENGINE_GEN_INCLUDE_DIR}" PROPERTIES GENERATED TRUE)
set(NARENGINE_FLATBUFFERS_FLAGS "--cpp" "--scoped-enums" "--cpp-std=c++17")

set(NARENGINE_SCHEMAS
    "common.fbs"
    "package_common.fbs"
    "package.fbs"
    "resource.fbs"
)

if("${NARENGINE_BUILD_CONSTRUCTOR}")
    set(NARENGINE_SCHEMAS ${NARENGINE_SCHEMAS}
        "constructor2d_nodes.fbs"
        "constructor2d_scene.fbs"
        "constructor2d_piece.fbs"
    )
endif() # if("${NARENGINE_BUILD_CONSTRUCTOR}")

flatbuffers_generate_headers(TARGET gen-cpp
    INCLUDE "${NARENGINE_SCHEMAS_DIR}"
    INCLUDE_PREFIX "${NAME}/gen"
    SCHEMAS ${NARENGINE_SCHEMAS}
    FLAGS ${NARENGINE_FLATBUFFERS_FLAGS}
)

# Installation
install(DIRECTORY "${NARENGINE_GEN_INCLUDE_DIR}/${NAME}" DESTINATION "include")
