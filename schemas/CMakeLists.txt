set(NARENGINE_SCHEMAS_DIR "${PROJECT_SOURCE_DIR}/schemas")

if ("${NARENGINE_TOOLS_FLATBUFFERS}")
    set(NARENGINE_FLATBUFFERS_FLAGS "--cpp" "--scoped-enums" "--cpp-std=c++17")
    set(NARENGINE_FB_GEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/schemas/flatbuffers-gen-cpp")

    set_source_files_properties("${NARENGINE_FB_GEN_INCLUDE_DIR}" PROPERTIES GENERATED TRUE)

    set(NARENGINE_FLATBUFFERS_SCHEMAS
        "flatbuffers/asset.fbs"
        "flatbuffers/resource.fbs"
    )
    if("${NARENGINE_BUILD_CONSTRUCTOR2D}")
        set(NARENGINE_FLATBUFFERS_SCHEMAS ${NARENGINE_FLATBUFFERS_SCHEMAS}
            "flatbuffers/constructor2d.fbs"
        )
    endif() # if("${NARENGINE_BUILD_CONSTRUCTOR2D}")

    flatbuffers_generate_headers(TARGET flatbuffers-gen-cpp
        INCLUDE "${NARENGINE_SCHEMAS_DIR}"
        INCLUDE_PREFIX "${NAME}/gen/flatbuffers"
        SCHEMAS ${NARENGINE_FLATBUFFERS_SCHEMAS}
        FLAGS ${NARENGINE_FLATBUFFERS_FLAGS}
    )

    # Installation
    install(DIRECTORY "${NARENGINE_FB_GEN_INCLUDE_DIR}/${NAME}" DESTINATION "include")
endif() # if ("${NARENGINE_TOOLS_FLATBUFFERS}")
