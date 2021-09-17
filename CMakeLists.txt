cmake_minimum_required(VERSION 3.20)
project(board)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(graphics)
add_subdirectory(chess_engine)
add_executable(
        ${PROJECT_NAME} main.cpp
        ChessButton.cpp
        Game.cpp
        Window.cpp
        Box.cpp Button.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} graphics chess_engine)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures)