add_executable(DPStest DPStest.cpp)
target_link_libraries(DPStest PUBLIC tdg)

add_executable(BPStest BPStest.cpp)
target_link_libraries(BPStest PUBLIC tdg)

add_executable(removeEdge removeEdge.cpp)
target_link_libraries(removeEdge PUBLIC tdg)

add_executable(InsertEdge InsertEdge.cpp)
target_link_libraries(InsertEdge PUBLIC tdg)

add_executable(writingDots writingDots.cpp)
target_link_libraries(writingDots PUBLIC tdg)
