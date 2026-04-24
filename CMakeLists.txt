cmake_minimum_required(VERSION 3.23)

project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

include (FetchContent)

FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.12.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tracy)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logging/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Threading/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/IO/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL/Render/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL/Render/Render3d/*.cpp

    
)

include_directories(
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL3/include"
    "${CMAKE_SOURCE_DIR}/3rdParty/spdlog/include"
    "${CMAKE_SOURCE_DIR}/3rdParty/"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/Core"
    "${CMAKE_SOURCE_DIR}/src/Logging"
    "${CMAKE_SOURCE_DIR}/src/Graphics"
    "${CMAKE_SOURCE_DIR}/src/Graphics/OpenGL"
    "${CMAKE_SOURCE_DIR}/src/Graphics/OpenGL/Render"
    "${CMAKE_SOURCE_DIR}/src/Common"
    "${CMAKE_SOURCE_DIR}/src/App"
    "${CMAKE_SOURCE_DIR}/src/Errors"
    
)

add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/spdlog")
add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/GL")


add_executable(Engine ${ENGINE_SOURCES})



target_link_libraries(Engine 
    gl3w
    TracyClient
    glm::glm
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL3/lib/x64/SDL3.lib"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine)







