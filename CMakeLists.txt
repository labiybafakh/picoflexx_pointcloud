cmake_minimum_required(VERSION 3.10)
project(picoflexx_pointcloud)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Find packages
find_package(royale REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Iridescence REQUIRED)
find_package(PCL REQUIRED COMPONENTS common filters)


# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/picoflexx.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${royale_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    Iridescence::Iridescence
    ${PCL_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    royale::royale
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    Eigen3::Eigen
    ${PCL_LIBRARIES}
)