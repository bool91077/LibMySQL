cmake_minimum_required(VERSION 3.0)
project(MySQLConnectorExample)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 MySQL Connector/C++ 库
find_package(MySQLConnectorCpp REQUIRED)

# 包含 MySQL 头文件目录
include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIRS})

# 添加可执行文件
add_executable(main main.cpp MySQLConnector.cpp MySQLConnector.h)

# 链接 MySQL Connector/C++ 库
target_link_libraries(main ${MYSQLCONNECTORCPP_LIBRARIES})
