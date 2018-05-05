cmake_minimum_required(VERSION 3.8)
project(Seamless_orthomosaic)

find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(SEND_ERROR "Failed to find OpenCV")
endif()
include_directories(${OpenCV_INCLUDE_DIR})

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_STANDARD 11)
include_directories(include)
include_directories(maxflow_solver)

set(SEGM_SOURCES
        edison_gpu/src/ms_filter_auto.cpp
        edison_gpu/src/ms_filter_opencl.cpp
        edison_gpu/src/ms_filter_opencl_kernel_cl.h
        edison_gpu/src/ms_filter_multithreaded.cpp
        edison_gpu/src/mean_shift.cpp
        edison_gpu/segm/ms.cpp
        edison_gpu/segm/msImageProcessor.cpp
        edison_gpu/segm/RAList.cpp
        edison_gpu/segm/rlist.cpp
        )

set(MAXFLOW_SOURCE_FILES maxflow_solver/bk_maxflow/adjacency_list_new_interface/graph.cpp maxflow_solver/bk_maxflow/adjacency_list_new_interface/maxflow.cpp)
set(ENERGY_FILES src/energy/pan_pixel_energy.cpp src/energy/pan_energy.cpp src/energy/mincut_energy.cpp src/energy/mincut_energy_mht.cpp)
set(SOURCE_FILES  src/runner/runner.cpp src/visual/visualizer.cpp src/basic/image.cpp src/basic/reader.cpp src/basic/seam.cpp)
set(SOURCE_FILES ${SOURCE_FILES} ${ENERGY_FILES})
set(NEIGHBOUR_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_best_neighbour.cpp src/solutions/best_neighbour.cpp)
set(MINCUT_SOURCE_FILES ${SOURCE_FILES} ${MAXFLOW_SOURCE_FILES} src/orthomosaic_mincut.cpp src/solutions/mincut.cpp)

add_executable(orthomosaic_best_neighbour ${NEIGHBOUR_SOURCE_FILES})
target_link_libraries(orthomosaic_best_neighbour ${OpenCV_LIBRARIES})

add_executable(orthomosaic_mincut ${MINCUT_SOURCE_FILES})
target_link_libraries(orthomosaic_mincut ${OpenCV_LIBRARIES})

set(VARIANTS_SOURCE_FILES src/variants.cpp ${SOURCE_FILES})
add_executable(variants ${VARIANTS_SOURCE_FILES})
target_link_libraries(variants ${OpenCV_LIBRARIES})

set(PAN_DIJKSTRA_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_pan_dijkstra.cpp src/solutions/pan_dijkstra.cpp)
add_executable(orthomosaic_pan_dijkstra ${PAN_DIJKSTRA_SOURCE_FILES})
target_link_libraries(orthomosaic_pan_dijkstra ${OpenCV_LIBRARIES})

set(MC_DIJKSTRA_SOURCE_FILES ${SOURCE_FILES} src/orthomosaic_mincut_dijkstra.cpp src/solutions/mincut_dijkstra.cpp)
add_executable(orthomosaic_mincut_dijkstra ${MC_DIJKSTRA_SOURCE_FILES})
target_link_libraries(orthomosaic_mincut_dijkstra ${OpenCV_LIBRARIES})

