cmake_minimum_required(VERSION 3.10)
project(glConvolution)

# 添加vcpkg工具链支持
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

message(STATUS "-----${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

add_executable(glConvolution
    main.cpp
)

target_include_directories(glConvolution PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR}
)

target_link_libraries(glConvolution
    OpenGL::GL
    GLEW::GLEW
    GLUT::GLUT
)

# 复制着色器文件
configure_file(shader.vert ${CMAKE_CURRENT_BINARY_DIR}/shader.vert COPYONLY)
configure_file(shader.frag ${CMAKE_CURRENT_BINARY_DIR}/shader.frag COPYONLY)