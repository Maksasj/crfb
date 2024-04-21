include_directories("../src")

FILE(GLOB CRFP_EXAMPLE_0_FILES example_0.c)
add_executable(crfp_example_0 ${CRFP_EXAMPLE_0_FILES})
target_link_libraries(crfp_example_0 crfp)

FILE(GLOB CRFP_EXAMPLE_2_FILES example_2.c)
add_executable(crfp_example_2 ${CRFP_EXAMPLE_2_FILES})
target_link_libraries(crfp_example_2 crfp)

if(MINGW)
    target_link_libraries(crfp_example_0 ws2_32)
    target_link_libraries(crfp_example_2 ws2_32)
endif()