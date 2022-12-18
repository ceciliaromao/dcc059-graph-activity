add_executable(DPStest DPStest.cpp)
target_link_libraries(DPStest PUBLIC tdg)

add_executable(BPStest BPStest.cc)
target_link_libraries(BPStest PUBLIC tdg)

add_executable(removeEdge removeEdge.cpp)
target_link_libraries(removeEdge PUBLIC tdg)

add_executable(InsertEdge InsertEdge.cpp)
target_link_libraries(InsertEdge PUBLIC tdg)

add_executable(TestComplement TestComplement.cpp)
target_link_libraries(TestComplement PUBLIC tdg)

add_executable(TestSubjacent TestSubjacent.cpp)
target_link_libraries(TestSubjacent PUBLIC tdg)

add_executable(Dijikstra Dijkstra.cpp)
target_link_libraries(Dijikstra PUBLIC tdg)

add_executable(Union Union.cpp)
target_link_libraries(Union PUBLIC tdg)

add_executable(FloydWarshall FloydWarshall.cpp)
target_link_libraries(FloydWarshall PUBLIC tdg)
