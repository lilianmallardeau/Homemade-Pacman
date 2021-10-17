cmake_minimum_required(VERSION 3.15)
project(Homemade_Pacman)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

add_executable(Homemade_Pacman src/main.cpp src/opengl/Renderer.cpp src/opengl/Renderer.h src/opengl/Shader.cpp src/opengl/Shader.h src/opengl/VertexBuffer.cpp src/opengl/VertexBuffer.h src/opengl/IndexBuffer.cpp src/opengl/IndexBuffer.h src/opengl/BufferLayout.cpp src/opengl/BufferLayout.h src/opengl/VertexArray.cpp src/opengl/VertexArray.h src/parameters.h src/opengl/Program.cpp src/opengl/Program.h src/game/Map.cpp src/game/Map.h src/game/Game.cpp src/game/Game.h src/game/MovableEntity.cpp src/game/MovableEntity.h src/game/Ghost.cpp src/game/Ghost.h src/game/Pacman.cpp src/game/Pacman.h src/opengl/Texture.cpp src/opengl/Texture.h src/opengl/opengl_abstraction.hpp)

target_include_directories(Homemade_Pacman PRIVATE lib/)

target_link_libraries(Homemade_Pacman
	PRIVATE
	glad
	glfw
	glm
	OpenGL::GL)
