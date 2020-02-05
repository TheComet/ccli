add_executable (ccli_demo
    "src/main.c"
    "src/serial_linux.c")
target_link_libraries (ccli_demo
    PUBLIC
        ccli
        $<$<PLATFORM_ID:Linux>:util>
        $<$<PLATFORM_ID:Linux>:pthread>)
