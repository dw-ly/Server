cmake_minimum_required(VERSION 3.16)
#添加引用头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/libThreadPool
    ${PROJECT_SOURCE_DIR}/libReactor
    # ${PROJECT_SOURCE_DIR}/amqp/include
    )

#添加库路径
# link_directories(${PROJECT_SOURCE_DIR}/amqp/lib)

#添加当前目录下文件
aux_source_directory(. APP_SRC)

#设置输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# add_subdirectory(net)
add_executable(server ${APP_SRC})

#链接库
target_link_libraries(server libThreadPool)
target_link_libraries(server libReactor)
target_link_libraries(server event pthread dl ssl)