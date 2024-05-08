//
// Created by heuser0000 on 5/3/24.
//

#include "LPSolver.h"

int main(int argc, char *argv[]) {
    //temporary bad stuff
    std::vector<int> pattern1;
    pattern1.push_back(0);
    pattern1.push_back(1);
    pattern1.push_back(2);

    std::vector<int> pattern2;
    pattern2.push_back(0);
    pattern2.push_back(1);

    std::vector<std::vector<int>> patterns;
    patterns.push_back(pattern1);
    patterns.push_back(pattern2);

    std::vector<double> heuristics;
    heuristics.push_back(20);
    heuristics.push_back(40);
    LPSolver lpSolver(2, 2, 4, patterns);

    std::vector<double> heuristics2;
    heuristics2.push_back(10);
    heuristics2.push_back(10);

    std::vector<double> heuristics3;
    heuristics3.push_back(40);
    heuristics3.push_back(40);
    std::cout << "result: " << lpSolver.solve(heuristics2) << std::endl;

    std::cout << "result: " << lpSolver.solve(heuristics3) << std::endl;

    return 0;
}