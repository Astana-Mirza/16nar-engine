set(NARENGINE_SCHEMAS_DIR "${PROJECT_SOURCE_DIR}/schemas")
set(NARENGINE_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/schemas/gen-cpp")

set_source_files_properties("${NARENGINE_GEN_INCLUDE_DIR}" PROPERTIES GENERATED TRUE)

if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    set(NARENGINE_FLATBUFFERS_FLAGS "--cpp" "--scoped-enums" "--cpp-std=c++17")

    set(NARENGINE_FLATBUFFERS_SCHEMAS
        "flatbuffers/common.fbs"
        "flatbuffers/package_common.fbs"
        "flatbuffers/package.fbs"
        "flatbuffers/resource.fbs"
    )
    if("${NARENGINE_BUILD_CONSTRUCTOR}")
        set(NARENGINE_FLATBUFFERS_SCHEMAS ${NARENGINE_FLATBUFFERS_SCHEMAS}
            "flatbuffers/constructor2d_nodes.fbs"
            "flatbuffers/constructor2d_scene.fbs"
            "flatbuffers/constructor2d_piece.fbs"
        )
    endif() # if("${NARENGINE_BUILD_CONSTRUCTOR}")

    flatbuffers_generate_headers(TARGET gen-cpp
        INCLUDE "${NARENGINE_SCHEMAS_DIR}"
        INCLUDE_PREFIX "${NAME}/gen/flatbuffers"
        SCHEMAS ${NARENGINE_FLATBUFFERS_SCHEMAS}
        FLAGS ${NARENGINE_FLATBUFFERS_FLAGS}
    )
endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")

# Installation
install(DIRECTORY "${NARENGINE_GEN_INCLUDE_DIR}/${NAME}" DESTINATION "include")
