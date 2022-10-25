cmake_minimum_required(VERSION 3.14)
project(teoria-dos-grafos)

include_directories(${PROJECT_SOURCE_DIR})

#Deletei o config file 
enable_testing()
add_subdirectory(tests)

file(GLOB source_files ./*.cpp ./*.h)
add_library(tdg ${source_files})
target_include_directories(tdg PUBLIC ${PROJECT_SOURCE_DIR})