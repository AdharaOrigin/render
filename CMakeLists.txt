cmake_minimum_required(VERSION 3.5)
project(Render)

set(CMAKE_CXX_STANDARD 14)
set(OPENGL_DEPENDENCIES -lglfw3 -lXrandr -lX11 -lXxf86vm -lpthread -lXcursor -lXinerama -ldl -lassimp)
# excluded: -lGL -lm -lXi

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_TEST test/main.cpp)
set(SRC_LIB lib/glad.c lib/stb_image.h lib/stb_image.cpp)
set(SRC_RENDER src/main.cpp src/shader.h src/shader.cpp src/objects.h src/objects.cpp src/mesh.cpp src/mesh.h src/model.cpp src/model.h)

add_executable(Render ${SRC_RENDER} ${SRC_LIB})
target_link_libraries(Render ${OPENGL_DEPENDENCIES})
set_target_properties(Render PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(GTests ${SRC_TEST})
set_target_properties(GTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/test")
