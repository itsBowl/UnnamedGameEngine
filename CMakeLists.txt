cmake_minimum_required(VERSION 3.23)

project(GameEngine)

set(CMAKE_CXX_STANDARD 23)
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

FetchContent_Declare(
    box3d
    GIT_REPOSITORY https://github.com/erincatto/box3d.git
    GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(box3d)

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp


    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logging/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Threading/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/IO/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Buffers/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL/Render/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL/*/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Render/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Shader/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Texture/*.cpp
    

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Assets/Camera/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Asset/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Asset/Mesh/*.cpp

    
)



if (WIN32)
    file(GLOB DX12_SOURCES CMAKE_CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DirectX12/*.cpp
        
    )
    #list(APPEND ENGINE_SOURCES ${DX12_SOURCES})
endif()

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
    "${CMAKE_SOURCE_DIR}/src/Core/Assets/Camera"
    "${CMAKE_SOURCE_DIR}/src/nanotime"
    
)

add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/spdlog")
add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/GL")


add_executable(Engine ${ENGINE_SOURCES})

target_include_directories(Engine PRIVATE 
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/include"
)

target_precompile_headers(Engine PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/src/PCH.hpp>")



target_link_libraries(Engine 
    gl3w
    TracyClient
    glm::glm
    box3d::box3d
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL3/lib/x64/SDL3.lib"
)

if(WIN32)
    target_link_libraries(Engine
        d3d12.lib
        dxgi.lib
        d3dcompiler.lib  
    )
    target_compile_definitions(Engine PRIVATE DX_12_SUPPORT)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG:Debug>")
    set(ASSIMP_NAME "assimp-vc143-mtd")
else()
    set(ASSIMP_NAME "assimp-vc143-mt")
endif()

set(ASSIMP_NAME "assimp-vc143-mt$<$<CONFIG:Debug>:d>")

target_link_libraries(Engine
    "${CMAKE_SOURCE_DIR}/3rdParty/assimp/lib/$<CONFIG>/${ASSIMP_NAME}.lib"    
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine)

add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/3rdParty/assimp/bin/$<CONFIG>/${ASSIMP_NAME}.dll"
        "$<TARGET_FILE_DIR:Engine>"
)







