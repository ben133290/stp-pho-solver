cmake_minimum_required(VERSION 3.27)
project(stp_pho_solver)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_OSX_ARCHITECTURES "x86_64")


# CPLEX dependency
# include_directories(../../../../../../../../Applications/CPLEX_Studio2211/cplex/include)
# include_directories(../../../../../../../../Applications/CPLEX_Studio2211/concert/include)


add_executable(stp_pho_solver PhO/pho-solver.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp
        PhO/MaxPDBHeuristic.h)

add_executable(stp_pdb_generator PhO/pdb-generator.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp)

add_executable(cplex-solver PhO/cplexSolver.cpp)

find_package(Cplex REQUIRED)
target_link_libraries(cplex-solver PRIVATE cplex::cplex)