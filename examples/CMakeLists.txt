include_directories("../src")

include_directories("../3dparty/stb")
include_directories("../3dparty/SDL/include")

FILE(GLOB CRFB_EXAMPLE_0_FILES example_0.c)
add_executable(crfb_example_0 ${CRFB_EXAMPLE_0_FILES})
target_link_libraries(crfb_example_0 crfb)
target_link_libraries(crfb_example_0 stb_image_write)

FILE(GLOB CRFB_EXAMPLE_1_FILES example_1.c)
add_executable(crfb_example_1 ${CRFB_EXAMPLE_1_FILES})
target_link_libraries(crfb_example_1 crfb)
target_link_libraries(crfb_example_1 SDL3::SDL3)
# target_link_libraries(crfb_example_1 sdl)

if(MINGW)
    target_link_libraries(crfb_example_0 ws2_32)
    target_link_libraries(crfb_example_1 ws2_32)
endif()