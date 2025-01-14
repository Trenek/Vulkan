cmake_minimum_required(VERSION 3.10)

project(Test VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

find_package(Vulkan REQUIRED)
if (NOT WIN32)
    find_package(glfw3)
endif ()

set(DIRECTORIES
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/camera"
    "${CMAKE_SOURCE_DIR}/source/graphics"
        "${CMAKE_SOURCE_DIR}/source/graphics/buffers"
        "${CMAKE_SOURCE_DIR}/source/graphics/setup"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/window"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/bufferOperations"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/commonOperations"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/imageOperations"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/depthHelpers"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/queueFamilyIndices"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/swapChain"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/swapChainSupportDetails"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/deltaTime"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/windowControl"
        "${CMAKE_SOURCE_DIR}/source/graphics/model"
            "${CMAKE_SOURCE_DIR}/source/graphics/model/builder"
                "${CMAKE_SOURCE_DIR}/source/graphics/model/builder/loader"
                "${CMAKE_SOURCE_DIR}/source/graphics/model/builder/type"
            "${CMAKE_SOURCE_DIR}/source/graphics/model/functions"
    "${CMAKE_SOURCE_DIR}/source/basicThings"
    "${CMAKE_SOURCE_DIR}/source/state"
    "${CMAKE_SOURCE_DIR}/source/button"
    "${CMAKE_SOURCE_DIR}/source/army"
        "${CMAKE_SOURCE_DIR}/source/army/cohord"
)

include_directories(${DIRECTORIES})

list(TRANSFORM DIRECTORIES APPEND "/*.c" OUTPUT_VARIABLE SOURCE_FILES)
file(GLOB SOURCE_FILES ${SOURCE_FILES})

add_executable(Test ${SOURCE_FILES})

set(FLAGS
    -O3
    -g
    -W
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wunused
    -Werror
    -Wno-comment
    -pedantic
    -pedantic-errors
    -D_GLIBCXX_DEBUG
    -D_GLIBCXX_DEBUG_PEDANTIC
    -D_UCRT
)

target_compile_options(Test PUBLIC ${FLAGS})

get_target_property(MAIN_CFLAGS Test COMPILE_OPTIONS)

add_custom_command(TARGET Test POST_BUILD
    COMMAND echo built with the flags: ${MAIN_CFLAGS}
)

set(INCLUDE_DIRECTORIES
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/dependencies/cglm/include"
    "${CMAKE_SOURCE_DIR}/dependencies/stb"
    "${CMAKE_SOURCE_DIR}/dependencies/tinyobjloader"
    "${CMAKE_SOURCE_DIR}/dependencies/cgltf"
)

set(LINK_LIBRARIES
    "${Vulkan_LIBRARIES}"
)

if (WIN32)
    set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES}
        "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.4/include"
    )

    set(LINK_LIBRARIES
        ${LINK_LIBRARIES}
        "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.4/src/libglfw3.a"
        -lucrt
    )
else ()
    set(LINK_LIBRARIES
        ${LINK_LIBRARIES}
        glfw
        -lm
    )
endif()

target_include_directories(Test PRIVATE ${INCLUDE_DIRECTORIES})
target_link_libraries(Test ${LINK_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)

add_custom_target(Shaders DEPENDS
    ${CMAKE_SOURCE_DIR}/shaders/shader.frag
    ${CMAKE_SOURCE_DIR}/shaders/shader2.frag
    ${CMAKE_SOURCE_DIR}/shaders/shader.vert
    ${CMAKE_SOURCE_DIR}/textures
    ${CMAKE_SOURCE_DIR}/models
)

add_custom_command(TARGET Shaders POST_BUILD
    COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/shader.vert -o ${CMAKE_BINARY_DIR}/shaders/vert.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/shader.frag -o ${CMAKE_BINARY_DIR}/shaders/frag.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/shader2.frag -o ${CMAKE_BINARY_DIR}/shaders/frag2.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/shader2d.frag -o ${CMAKE_BINARY_DIR}/shaders/frag2d.spv
    COMMAND glslc ${CMAKE_SOURCE_DIR}/shaders/shader2d.vert -o ${CMAKE_BINARY_DIR}/shaders/vert2d.spv
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textures ${CMAKE_BINARY_DIR}/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models ${CMAKE_BINARY_DIR}/models
)

add_dependencies(Test Shaders)
