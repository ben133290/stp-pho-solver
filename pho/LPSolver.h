//
// Created by heuser0000 on 4/28/24.
//

#ifndef STP_PHO_SOLVER_LPSOLVER_H
#define STP_PHO_SOLVER_LPSOLVER_H

#include <cplex.h>
#include <string>
#include <iostream>
#include <vector>

class LPSolver {
public:
    LPSolver(int numRows, int numCols, int numnz, const std::vector<std::vector<int>> &nzBitMap);

    double solve(const std::vector<double> &rhs);

    static void handle_cplex_error(CPXENVptr env, int status);

    virtual ~LPSolver();

private:
    CPXENVptr env;
    CPXLPptr lp;
    int numRows;
    int numCols;
    int numnz;

    CPXLPptr createProblem(const std::string &name);

    void freeProblem(CPXLPptr *problem);

    int init(const std::vector<std::vector<int>> &patterns); // TODO: Remove arg and set rhs to 0 instead
};

/* Make a call to a CPLEX API function checking its return status. */
template<typename Func, typename ... Args>
static void CPX_CALL(Func cpxfunc, CPXENVptr env, Args &&... args) {
    int status = cpxfunc(env, std::forward<Args>(args) ...);
    if (status) {
        LPSolver::handle_cplex_error(env, status);
    }
}


#endif //STP_PHO_SOLVER_LPSOLVER_H
