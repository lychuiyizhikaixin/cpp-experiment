file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS "*.cpp")
add_library(training ${SOURCE})
target_include_directories(training PUBLIC "${CMAKE_SOURCE_DIR}/include"
                                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(training PRIVATE -Wall -Werror)
