//
// Created by Benedikt Heuser on 16.04.24.
//

#ifndef STP_PHO_SOLVER_MAXPDBHEURISTIC_H
#define STP_PHO_SOLVER_MAXPDBHEURISTIC_H

#include "../hog2/search/LexPermutationPDB.h"
#include "../hog2/search/Heuristic.h"
#include "../hog2/environments/MNPuzzle.h"
#include "LPSolver.h"

#define GRID_SIZE 4
//#define HTYPE LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>>

template <class state, class actions, class environment>
class PHOHeuristic : public Heuristic<state> {
public:
    explicit PHOHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics, std::vector<std::vector<int>> &patterns);
    double HCost(const state &s1, const state &s2) const;
    std::vector<LexPermutationPDB<state, actions, environment>> heuristicVec;
private:
    LPSolver lpSolver;
};

template<class state, class actions, class environment>
PHOHeuristic<state, actions, environment>::PHOHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics, std::vector<std::vector<int>> &patterns)
        : heuristicVec(std::move(heuristics)) {
    int nzs = 0; // Initialize nzs here
    std::vector<double> rhs;
    for (const auto & pattern : patterns) {
        nzs += pattern.size() - 1;
        rhs.push_back(0);
    }
    lpSolver = LPSolver(patterns.size(), 15, nzs, patterns, rhs);
}


template <class state, class actions, class environment>
double PHOHeuristic<state, actions, environment>::HCost(const state &s1, const state &s2) const
{
    std::vector<double> rhs;
    for (const LexPermutationPDB<state, actions, environment> &h : heuristicVec) {
        double hCost = h.HCost(s1, s2);
        rhs.push_back(hCost);
    }
    double result = lpSolver.solve(rhs);

    return result;
}


#endif //STP_PHO_SOLVER_MAXPDBHEURISTIC_H
