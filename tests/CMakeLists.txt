add_executable(DPStest DPStest.cpp)
target_link_libraries(DPStest PUBLIC tdg)

add_executable(removeEdge removeEdge.cpp)
target_link_libraries(removeEdge PUBLIC tdg)