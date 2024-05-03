cmake_minimum_required(VERSION 3.23)
project(stp_pho_solver)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_OSX_ARCHITECTURES "x86_64")


# CPLEX dependency
# include_directories(../../../../../../../../Applications/CPLEX_Studio2211/cplex/include)
# include_directories(../../../../../../../../Applications/CPLEX_Studio2211/concert/include)

add_executable(cplextest pho/cplextest.cpp
        pho/LPSolver.cpp)

add_executable(stp_pho_solver pho/pho-solver.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp
        pho/PHOHeuristic.h
        pho/LPSolver.cpp)

add_executable(max_pdb_solver pho/max-pdb-solver.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp
        pho/MaxPDBHeuristic.h)

add_executable(stp_pdb_generator pho/pdb-generator.cpp
        hog2/utils/Timer.cpp
        hog2/utils/NBitArray.cpp)

find_package(Threads REQUIRED)
target_link_libraries(stp_pho_solver PUBLIC Threads::Threads)
target_link_libraries(stp_pdb_generator PUBLIC Threads::Threads)

add_executable(lp-solver pho/LPSolver.cpp)

find_package(Cplex REQUIRED)
target_link_libraries(lp-solver PRIVATE cplex::cplex)
target_link_libraries(stp_pho_solver PRIVATE cplex::cplex)
target_link_libraries(cplextest PRIVATE cplex::cplex)