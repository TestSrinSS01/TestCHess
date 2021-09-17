cmake_minimum_required(VERSION 3.18)
add_library(glfw SHARED IMPORTED)
add_library(glew SHARED IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/glfw3.dll" IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/glfw3dll.lib")
set_target_properties(glew PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/glew32.dll" IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/glew32.lib")
add_library(graphics
    Shader.cpp
    stb_image.cpp
    Texture.cpp
    VertexArray.cpp
    VertexBuffer.cpp
)
target_include_directories(graphics PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(graphics glfw glew opengl32)