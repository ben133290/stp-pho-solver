cmake_minimum_required(VERSION 3.23)
project(stp_pho_solver)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_OSX_ARCHITECTURES "x86_64")

add_executable(pho_solver pho/PHO-solver.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp
        pho/PHOHeuristic.h
        pho/LPSolver.cpp
        hog2/environments/STPInstances.cpp
        hog2/environments/STPInstances.h
        downward/system.cc
        downward/system_unix.cc
        pho/SumHeuristic.h)

add_executable(max_solver pho/Max-pdb-solver.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp
        pho/MaxPDBHeuristic.h)

add_executable(pdb_generator pho/PDB-generator.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp)

find_package(Threads REQUIRED)
target_link_libraries(pho_solver PUBLIC Threads::Threads)
target_link_libraries(pdb_generator PUBLIC Threads::Threads)

find_package(Cplex REQUIRED)
target_link_libraries(pho_solver PRIVATE cplex::cplex)