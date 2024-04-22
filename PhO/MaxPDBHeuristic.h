//
// Created by Benedikt Heuser on 16.04.24.
//

#ifndef STP_PHO_SOLVER_MAXPDBHEURISTIC_H
#define STP_PHO_SOLVER_MAXPDBHEURISTIC_H

#include "../hog2/search/LexPermutationPDB.h"
#include "../hog2/search/Heuristic.h"
#include "../hog2/environments/MNPuzzle.h"

#define GRID_SIZE 4
//#define HTYPE LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>>

template <class state, class actions, class environment>
class MaxPDBHeuristic : public Heuristic<state> {
public:
    explicit MaxPDBHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &heuristics);
    double HCost(const state &s1, const state &s2) const;
    std::vector<LexPermutationPDB<state, actions, environment>> heuristicVec;

};

template<class state, class actions, class environment>
MaxPDBHeuristic<state, actions, environment>::MaxPDBHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &heuristics) {
    for (LexPermutationPDB<state, actions, environment> &h : heuristics) {
        heuristicVec.push_back(h);
    }
}

template <class state, class actions, class environment>
double MaxPDBHeuristic<state, actions, environment>::HCost(const state &s1, const state &s2) const
{
    double currentMax = 0;
    for (const LexPermutationPDB<state, actions, environment> &h : heuristicVec) {
        double hCost = h.HCost(s1, s2); // TODO: Figure out how to get it to call lexpdb hcost function
        if (hCost > currentMax) {
            currentMax = hCost;
        }
    }

    return currentMax;
}


#endif //STP_PHO_SOLVER_MAXPDBHEURISTIC_H
