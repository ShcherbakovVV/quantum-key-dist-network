cmake_minimum_required(VERSION 3.20.0)

project(qkd VERSION 1.0.0 LANGUAGES CXX)

set(TESTS_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(TESTS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(TESTS_SRC "${TESTS_SRC_DIR}/adjacency_matrix.cpp"
              "${TESTS_SRC_DIR}/descriptor.cpp"
              "${TESTS_SRC_DIR}/key_generation_model.cpp"
              "${TESTS_SRC_DIR}/lifo.cpp"
              "${TESTS_SRC_DIR}/module_network.cpp"
              "${TESTS_SRC_DIR}/path.cpp"
              "${TESTS_SRC_DIR}/pathfinder.cpp"
              "${TESTS_SRC_DIR}/priority_queue.cpp"
              "${TESTS_SRC_DIR}/property_table.cpp"
              "${TESTS_SRC_DIR}/queue_manager.cpp"
              "${TESTS_SRC_DIR}/request_builder.cpp"
              "${TESTS_SRC_DIR}/request.cpp"
              "${TESTS_SRC_DIR}/rng.cpp"
              "${TESTS_SRC_DIR}/topology.cpp")

find_package(Boost COMPONENTS log)


foreach(test ${TESTS_SRC})

    string(REPLACE ".cpp" "" testexepath ${test})
    file(RELATIVE_PATH testexe ${TESTS_SRC_DIR} ${testexepath})

    add_executable(${testexe} ${test})

    set_target_properties(${testexe} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_BIN_DIR})

    target_link_libraries(${testexe} OBJ ${Boost_LIBRARIES})

    add_test(NAME ${testexe} COMMAND ${testexe})

endforeach(test ${TESTS_SRC})
