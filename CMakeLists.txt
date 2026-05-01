cmake_minimum_required(VERSION 3.16)
project(RenderDemo)


file(GLOB RENDERER_SRC
    "src/*.cpp"
)

add_executable(RenderDemo ${RENDERER_SRC} glad/src/glad.c)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

target_include_directories(RenderDemo PUBLIC include)
target_include_directories(RenderDemo PUBLIC glad/include)

# OpenGL Mathematics
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)
#####################

target_link_libraries(RenderDemo ${SDL2_LIBRARIES} glm::glm)
