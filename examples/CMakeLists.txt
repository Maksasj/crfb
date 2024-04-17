include_directories("../src")

FILE(GLOB CRFP_EXAMPLE_0_FILES example_0.c)
add_executable(crfp ${CRFP_EXAMPLE_0_FILES})
target_link_libraries(crfp_example_0 crfp)

if(MINGW)
    target_link_libraries(crfp_example_0 ws2_32)
endif()