cmake_minimum_required(VERSION 3.8)
project(Seamless_orthomosaic)

find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(SEND_ERROR "Failed to find OpenCV")
endif()
include_directories(${OpenCV_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)
include_directories(include)

set(SOURCE_FILES src/orthomosaic.cpp src/visual/visualizer.cpp src/solutions/best_neighbour.cpp src/basic/image.cpp src/basic/reader.cpp src/basic/seam.cpp)
add_executable(orthomosaic ${SOURCE_FILES})
target_link_libraries(orthomosaic ${OpenCV_LIBRARIES})

set(VARIANTS_SOURCE_FILES src/variants.cpp src/basic/reader.cpp src/basic/image.cpp src/basic/seam.cpp)
add_executable(variants ${VARIANTS_SOURCE_FILES})
target_link_libraries(variants ${OpenCV_LIBRARIES})

