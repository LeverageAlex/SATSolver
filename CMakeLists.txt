cmake_minimum_required(VERSION 3.23)
project(SATSolver)

set(CMAKE_CXX_STANDARD 17)

add_executable(SATSolver main.cpp Tree/AbstractNode.h Tree/Node.cpp Tree/Node.h Tree/ValueNode.cpp Tree/ValueNode.h)
