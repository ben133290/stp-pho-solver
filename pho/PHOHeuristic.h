//
// Created by Benedikt Heuser on 16.04.24.
//

#ifndef STP_PHO_SOLVER_PHOHEURISTIC_H
#define STP_PHO_SOLVER_PHOHEURISTIC_H

#include "../hog2/search/LexPermutationPDB.h"
#include "../hog2/search/Heuristic.h"
#include "../hog2/environments/MNPuzzle.h"
#include "LPSolver.h"

#define GRID_SIZE 4

template <class state, class actions, class environment>
class PHOHeuristic : public Heuristic<state> {
public:
    explicit PHOHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics,
                          std::vector<std::vector<int>> &patterns,
                          bool verbose);
    double HCost(const state &s1, const state &s2) const;
    std::vector<LexPermutationPDB<state, actions, environment>> heuristicVec;
private:
    mutable LPSolver lpSolver;
    int computeNZS(std::vector<std::vector<int>> &patterns);
    std::vector<double> computeRHS(std::vector<std::vector<int>> &patterns);
};

template<class state, class actions, class environment>
std::vector<double> PHOHeuristic<state, actions, environment>::computeRHS(std::vector<std::vector<int>> &patterns) {
    return std::vector<double>(patterns.size());
}

template<class state, class actions, class environment>
int PHOHeuristic<state, actions, environment>::computeNZS(std::vector<std::vector<int>> &patterns) {
    int nzs = 0; // Initialize nzs here
    for (const auto & pattern : patterns) {
        nzs += pattern.size() - 1;
    }
    return nzs;
}

template<class state, class actions, class environment>
PHOHeuristic<state, actions, environment>
        ::PHOHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics,
                       std::vector<std::vector<int>> &patterns,
                       bool verbose)
        : heuristicVec(std::move(heuristics)), lpSolver(patterns.size(), 15, computeNZS(patterns), patterns, verbose) {}


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

#endif //STP_PHO_SOLVER_PHOHEURISTIC_H
