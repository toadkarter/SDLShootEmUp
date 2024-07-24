cmake_minimum_required(VERSION 3.10)

project(SDLBreakout)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(include)

if (WIN32)
set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/extern/SDL2-2.30.5/include")
set(SDL2_IMAGE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/extern/SDL2_image-2.8.2/include")
set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/extern/SDL2-2.30.5/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/extern/SDL2-2.30.5/lib/x64/SDL2main.lib")
set(SDL2_IMAGE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/extern/SDL2_image-2.8.2/lib/x64/SDL2_image.lib")
ELSE()
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
ENDIF()

include_directories(include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Collect all source files in src directory
file(GLOB SRC_FILES "src/*.c")

# Add executable target
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

message(STATUS "CMAKE_CONTENTS: ${CMAKE_MODULE_PATH}")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES}")
