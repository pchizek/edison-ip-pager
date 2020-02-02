add_library(serial-io serial-io.cpp)

target_include_directories( serial-io
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                            )
