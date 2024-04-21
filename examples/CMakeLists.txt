include_directories("../src")

include_directories("../3dparty/stb")

FILE(GLOB CRFB_EXAMPLE_0_FILES example_0.c)
add_executable(crfb_example_0 ${CRFB_EXAMPLE_0_FILES})
target_link_libraries(crfb_example_0 crfb)
target_link_libraries(crfb_example_0 stb_image_write)

if(MINGW)
    target_link_libraries(crfb_example_0 ws2_32)
endif()