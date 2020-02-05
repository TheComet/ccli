add_library (ccli STATIC
    "src/cli.c")
target_include_directories (ccli
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>)
