//
// Created by Benedikt Heuser on 16.04.24.
//

#ifndef STP_PHO_SOLVER_MAXPDBHEURISTIC_H
#define STP_PHO_SOLVER_MAXPDBHEURISTIC_H

#include "hog2/search/Heuristic.h"

template <class state>
class MaxPDBHeuristic : Heuristic<state> {
public:
    explicit MaxPDBHeuristic(std::vector<Heuristic<state>> &heuristics);
    double HCost(const state &s1, const state &s2) const;
    std::vector<Heuristic<state>> heuristicVec;

};

template<class state>
MaxPDBHeuristic<state>::MaxPDBHeuristic(std::vector<Heuristic<state>> &heuristics) {
    for (Heuristic<state> &h : heuristics) {
        heuristicVec.push_back(h);
    }
}

template <class state>
double MaxPDBHeuristic<state>::HCost(const state &s1, const state &s2) const
{
    double currentMax = 0;
    for (Heuristic<state> &h : heuristicVec) {
        double hCost = h.HCost(s1, s2);
        if (hCost > currentMax) {
            currentMax = hCost;
        }
    }

    return currentMax;
}


#endif //STP_PHO_SOLVER_MAXPDBHEURISTIC_H
