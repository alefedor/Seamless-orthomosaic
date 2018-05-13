cmake_minimum_required(VERSION 3.8)
project(Seamless_orthomosaic)

find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(SEND_ERROR "Failed to find OpenCV")
endif()
include_directories(${OpenCV_INCLUDE_DIR})
find_package(OpenMP REQUIRED)
if (NOT OpenMP_FOUND)
    message(SEND_ERROR "Failed to find OpenMP")
endif()
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_CXX_STANDARD 11)
include_directories(include)
include_directories(maxflow_solver)
include_directories(edison_gpu/src)

#include(${CMAKE_CURRENT_SOURCE_DIR}/edison_gpu/CMakeLists.txt)
add_subdirectory(edison_gpu)

set(MAXFLOW_SOURCE_FILES maxflow_solver/bk_maxflow/adjacency_list_new_interface/graph.cpp maxflow_solver/bk_maxflow/adjacency_list_new_interface/maxflow.cpp)
set(ENERGY_FILES
        src/energy/mincut_energy.cpp
        src/energy/mincut_energy_mht.cpp
        src/energy/chon_energy.cpp
        src/energy/chon_pixel_energy.cpp)

set(SOURCE_FILES  src/runner/runner.cpp src/visual/visualizer.cpp src/basic/image.cpp src/basic/reader.cpp src/basic/seam.cpp)
set(SOURCE_FILES ${SOURCE_FILES} ${ENERGY_FILES})
set(NEIGHBOUR_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_best_neighbour.cpp src/solutions/best_neighbour.cpp)
set(MINCUT_SOURCE_FILES ${SOURCE_FILES} ${MAXFLOW_SOURCE_FILES} src/orthomosaic_mincut.cpp src/solutions/mincut.cpp)

set(EDISON_LIBRARIES edison_gpu pthread)

add_executable(orthomosaic_best_neighbour ${NEIGHBOUR_SOURCE_FILES})
target_link_libraries(orthomosaic_best_neighbour ${OpenCV_LIBRARIES})

add_executable(orthomosaic_mincut ${MINCUT_SOURCE_FILES})
target_link_libraries(orthomosaic_mincut ${OpenCV_LIBRARIES})

set(VARIANTS_SOURCE_FILES src/variants.cpp ${SOURCE_FILES})
add_executable(variants ${VARIANTS_SOURCE_FILES})
target_link_libraries(variants ${OpenCV_LIBRARIES})

set(PAN_DIJKSTRA_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_pan_dijkstra.cpp src/solutions/pan_dijkstra.cpp src/energy/pan_pixel_energy.cpp src/energy/pan_energy.cpp src/energy/pan_differential_energy.cpp)
set(PAN_DIJKSTRA_SOURCE_FILES ${PAN_DIJKSTRA_SOURCE_FILES})
add_executable(orthomosaic_pan_dijkstra ${PAN_DIJKSTRA_SOURCE_FILES})
target_link_libraries(orthomosaic_pan_dijkstra ${EDISON_LIBRARIES})
target_link_libraries(orthomosaic_pan_dijkstra ${OpenCV_LIBRARIES})

set(MC_DIJKSTRA_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_mincut_dijkstra.cpp src/solutions/mincut_dijkstra.cpp)
add_executable(orthomosaic_mincut_dijkstra ${MC_DIJKSTRA_SOURCE_FILES})
target_link_libraries(orthomosaic_mincut_dijkstra ${OpenCV_LIBRARIES})

set(CHON_DIJKSTRA_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_chon_dijkstra.cpp src/solutions/chon_dijkstra.cpp)
add_executable(orthomosaic_chon_dijkstra ${CHON_DIJKSTRA_SOURCE_FILES})
target_link_libraries(orthomosaic_chon_dijkstra ${OpenCV_LIBRARIES})

