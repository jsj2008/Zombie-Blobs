cmake_minimum_required(VERSION 2.6)
project(City)

FILE(GLOB SOURCES "*.cpp" "*.hpp")

Find_Package(SDL REQUIRED)
Find_Package(OpenGL REQUIRED)

include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
link_libraries(${SDL_LIBRARY} ${OPENGL_LIBRARIES} BulletDynamics BulletCollision LinearMath)

add_executable(city ${SOURCES})
