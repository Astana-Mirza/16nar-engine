function(generate_docs LANG_FULL LANG_DIR)
    set(NARENGINE_DOC_LANG_FULL "${LANG_FULL}") # for pasting into Doxyfile
    set(NARENGINE_DOC_LANG_DIR "${LANG_DIR}")
    configure_file(Doxyfile.in "${NARENGINE_DOC_LANG_DIR}/Doxyfile" @ONLY)
    add_custom_target("docs_${NARENGINE_DOC_LANG_DIR}" ALL
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/${NARENGINE_DOC_LANG_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${NARENGINE_DOC_LANG_DIR}/html" DESTINATION "res/docs")
endfunction()


generate_docs("Russian" "ru")
