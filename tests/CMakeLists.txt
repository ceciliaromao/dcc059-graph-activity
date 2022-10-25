add_executable(DPStest DPStest.cc)
target_link_libraries(DPStest PUBLIC tdg)

add_executable(InsertEdge InsertEdge.cc)
target_link_libraries(InsertEdge PUBLIC tdg)