cmake_minimum_required(VERSION 3.10)
project(picoflexx_pointcloud)

# Find OpenGL package
find_package(OpenGL REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)


include_directories(
    include
    ${OPENGL_INCLUDE_DIRS}
    /home/khadas/libroyale/include
    ${EIGEN3_INCLUDE_DIR}
)
# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/picoflexx.cpp
)

# Link OpenGL and GLUT libraries
target_link_libraries(${PROJECT_NAME}  
    PRIVATE /usr/local/lib/libroyale.so.4.24.0
    ${OPENGL_LIBRARIES} -lglut Eigen3::Eigen)

# Include OpenGL and GLUT headers
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})