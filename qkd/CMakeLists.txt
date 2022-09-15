cmake_minimum_required(VERSION 3.20.0)

project(qkd VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(-g --std=c++20 -O0 -Wall -Wpedantic -Wconversion -Wextra)

find_package(Threads)
find_package(Boost COMPONENTS log random)
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(OBJ_SRC "${SRC_DIR}/AdjacencyMatrix.cpp"
            "${SRC_DIR}/Descriptor.cpp"
            "${SRC_DIR}/DijkstraShortestPath.cpp"
            "${SRC_DIR}/FIFO.cpp"
            "${SRC_DIR}/KeyGenerationModel.cpp"
            "${SRC_DIR}/LIFO.cpp"
            "${SRC_DIR}/Module.cpp"
            "${SRC_DIR}/Network.cpp"
            "${SRC_DIR}/Path.cpp"
            "${SRC_DIR}/Pathfinder.cpp"
            "${SRC_DIR}/PriorityQueue.cpp"
            "${SRC_DIR}/PropertyTable.cpp"
            "${SRC_DIR}/QKD_Network.cpp"
            "${SRC_DIR}/QueueManager.cpp"
            "${SRC_DIR}/RandomQueue.cpp"
            "${SRC_DIR}/Request.cpp"
            "${SRC_DIR}/RequestBuilder.cpp"
            "${SRC_DIR}/RequestGenerator.cpp"
            "${SRC_DIR}/RequestQueue.cpp"
            "${SRC_DIR}/SimulationWindow.cpp"
            "${SRC_DIR}/Statistics.cpp"
            "${SRC_DIR}/Topology.cpp")
add_library(OBJ OBJECT ${OBJ_SRC})
target_link_libraries(OBJ ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${GTKMM_LIBRARIES})

add_executable(app "${SRC_DIR}/Application.cpp")
target_link_libraries(app OBJ ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${GTKMM_LIBRARIES})

