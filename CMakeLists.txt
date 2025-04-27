cmake_minimum_required(VERSION 3.31)
project(EC)

set(CMAKE_CXX_STANDARD 20)

add_library(MyLib AUList/AUList.cpp ASList/ASList.cpp BST/BST.cpp BST/CLQueue.cpp LLSList/LLSList.cpp LLUList/LLUList.cpp)

add_executable(EC main.cpp)

target_link_libraries(EC MyLib)
