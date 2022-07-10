cmake_minimum_required(VERSION 3.16)
set(LIB_SRC CThreadPool.cpp CTimeWhell.cpp CTimer.cpp CWhellMgr.cpp)
add_library(libThreadPool ${LIB_SRC}) #.a
#add_library(libhello SHARED ${LIB_SRC}) #.so
#设置输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set_target_properties(libThreadPool PROPERTIES OUTPUT_NAME "threadpool")
target_link_libraries(libThreadPool pthread)
#SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
