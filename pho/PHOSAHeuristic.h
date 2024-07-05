//
// Created by Benedikt Heuser on 16.04.24.
//

#ifndef STP_PHO_SOLVER_PHOSAHEURISTIC_H
#define STP_PHO_SOLVER_PHOSAHEURISTIC_H

#include "../hog2/search/LexPermutationPDB.h"
#include "../hog2/search/Heuristic.h"
#include "../hog2/environments/MNPuzzle.h"
#include "../hog2/utils/FPUtil.h"
#include "../hog2/environments/STPInstances.h"
#include <cmath>

#define GRID_SIZE 4

template <class state, class actions, class environment>
class PHOSAHeuristic : public Heuristic<state> {
public:
    explicit PHOSAHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics,
                            std::vector<std::vector<double>> &&weights);
    double HCost(const state &s1, const state &s2) const;
private:
    std::vector<LexPermutationPDB<state, actions, environment>> pdb_heuristics;
    std::vector<std::vector<double>> weights;
    double weighted_sum(const std::vector<double>& weight_vec, const state &puzzle_state, const state &s2) const;
    MNPuzzleState<4,4> goal;
};

template<class state, class actions, class environment>
double PHOSAHeuristic<state, actions, environment>::weighted_sum(
        const std::vector<double>& weight_vec,
        const state &puzzle_state,
        const state &s2) const {
    double result = 0;
    for (int i = 0; i < weight_vec.size(); i++) {
        result += weight_vec[i] * pdb_heuristics[i].HCost(puzzle_state, s2);
    }
    return result;
}

template<class state, class actions, class environment>
PHOSAHeuristic<state, actions, environment>::PHOSAHeuristic(
        std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics,
        std::vector<std::vector<double>> &&weights
        ) : pdb_heuristics(std::move(heuristics)), weights(std::move(weights)) {}

template <class state, class actions, class environment>
double PHOSAHeuristic<state, actions, environment>::HCost(const state &s1, const state &s2) const
{
    double best = 0;
    for (const std::vector<double> &weight_vec : weights) {
        double h = weighted_sum(weight_vec, s1, s2);
        best = max(best, h);
    }
    return std::ceil(best - 0.00001);
}

#endif
