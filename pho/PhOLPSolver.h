//
// Created by heuser0000 on 4/28/24.
//

#ifndef STP_PHO_SOLVER_PHOLPSOLVER_H
#define STP_PHO_SOLVER_PHOLPSOLVER_H

#include <cplex.h>
#include <string>
#include <iostream>
#include <vector>


class PhOLPSolver {
public:
    static int run();
    static int populateByRow(CPXENVptr env, CPXLPptr lp, const std::vector<int> &patternBitMap, const std::vector<double> &pdbHeuristics);
    static void handle_cplex_error(CPXENVptr env, int status);
    static CPXLPptr createProblem(CPXENVptr env, const std::string &name);
    static void freeProblem(CPXENVptr env, CPXLPptr *problem);
    static int numRows;
    static int numCols;
    static int numnz;
private:

};

/* Make a call to a CPLEX API function checking its return status. */
template<typename Func, typename ... Args>
static void CPX_CALL(Func cpxfunc, CPXENVptr env, Args && ... args) {
    int status = cpxfunc(env, std::forward<Args>(args) ...);
    if (status) {
        PhOLPSolver::handle_cplex_error(env, status);
    }
}



#endif //STP_PHO_SOLVER_PHOLPSOLVER_H
