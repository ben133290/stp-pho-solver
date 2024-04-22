cmake_minimum_required(VERSION 3.27)
project(stp_pho_solver)

set(CMAKE_CXX_STANDARD 17)

add_executable(stp_pho_solver PhO/pho-solver.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp
        PhO/MaxPDBHeuristic.h)

add_executable(stp_pdb_generator PhO/pdb-generator.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp)
