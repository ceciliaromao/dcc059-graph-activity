add_executable(DPStest DPStest.cc)
target_link_libraries(DPStest PUBLIC tdg)

add_executable(BPStest BPStest.cc)
target_link_libraries(BPStest PUBLIC tdg)