//
// Created by heuser0000 on 5/17/24.
//

#ifndef STP_PHO_SOLVER_SUMHEURISTIC_H
#define STP_PHO_SOLVER_SUMHEURISTIC_H

#include "../hog2/search/LexPermutationPDB.h"
#include "../hog2/search/Heuristic.h"
#include "../hog2/environments/MNPuzzle.h"

template <class state, class actions, class environment>
class SumHeuristic : public Heuristic<state> {
public:
    explicit SumHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics, bool verbose = false);
    double HCost(const state &s1, const state &s2) const;
    std::vector<LexPermutationPDB<state, actions, environment>> heuristicVec;
private:
    bool verbose = false;
};

template<class state, class actions, class environment>
SumHeuristic<state, actions, environment>
::SumHeuristic(std::vector<LexPermutationPDB<state, actions, environment>> &&heuristics,
               bool verbose)
        : heuristicVec(std::move(heuristics)) {
            this->verbose = verbose;
        }


template <class state, class actions, class environment>
double SumHeuristic<state, actions, environment>::HCost(const state &s1, const state &s2) const
{
    double result = 0;
    for (const LexPermutationPDB<state, actions, environment> &h : heuristicVec) {
        double hCost = h.HCost(s1, s2);
        result += hCost;
    }
    return result;
}

#endif //STP_PHO_SOLVER_SUMHEURISTIC_H
