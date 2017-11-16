cmake_minimum_required(VERSION 3.8)
project(FastCopy)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp producer.cpp producer.h consumer.cpp consumer.h sem_lock.h sem_lock.cpp)
add_executable(FastCopy ${SOURCE_FILES})
target_link_libraries(FastCopy pthread)