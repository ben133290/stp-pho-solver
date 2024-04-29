//
// Created by heuser0000 on 4/28/24.
//

#include "PhOLPSolver.h"



using namespace std;

int PhOLPSolver::numRows;
int PhOLPSolver::numCols;
int PhOLPSolver::numnz;

int PhOLPSolver::run() {
    int status = 0;
    CPXENVptr env = CPXopenCPLEX(&status);
    CPXLPptr lp = createProblem(env, "name");

    //temporary bad stuff
    std::vector<int> patternMap;
    patternMap.push_back(2);
    patternMap.push_back(1);
    patternMap.push_back(0);
    patternMap.push_back(1);
    std::vector<double> heuristics;
    heuristics.push_back(20);
    heuristics.push_back(10);

    CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);
    populateByRow(env, lp, patternMap, heuristics);
    CPX_CALL(CPXlpopt, env, lp);
    double value;
    CPX_CALL(CPXgetobjval, env, lp, &value);
    std::cout << "result: " << value << std::endl;

    freeProblem(env, &lp);
    status = CPXcloseCPLEX(&env);
    return 0;
}

/* To populate by row, we first create the columns, and then add the
   rows.  */
int PhOLPSolver::populateByRow(CPXENVptr env, CPXLPptr lp,
                               const std::vector<int>& patternBitMap,
                               const std::vector<double> &pdbHeuristics)
{
    int      status    = 0;
    double   obj[numCols];
    double   lb[numCols];
    double   ub[numCols];
    char     *colname[numCols];
    int      rmatbeg[numRows];
    int      rmatind[numnz];
    double   rmatval[numnz];
    double   rhs[numRows];
    char     sense[numRows];
    char     *rowname[numRows];

    status = CPXchgobjsen (env, lp, CPX_MIN);  /* Problem is minimization */
    if ( status ) return status;

    /* Now create the new columns.  First, populate the arrays. */

    // Set Objective Function
    for (int col = 0; col < numCols; col++) {
        obj[col] = 1.0;
        lb[col] = 0.0;
        ub[col] = CPX_INFBOUND;
        colname[col] = "col";
    }



    status = CPXnewcols (env, lp, numCols, obj, lb, ub, NULL, colname);
    if ( status ) return status;

    /* Now add the constraints.  */
    int counter = 0;
    for (int constr = 0; constr < 2; constr++ ) {
        rmatbeg[constr] = (constr * numCols);
        rowname[constr] = "constr";

        for (int col = 0; col < numCols; col++) {
            rmatind[col + (constr * numCols)] = col;
            rmatval[col + (constr * numCols)] = patternBitMap[counter];
            counter++;
        }

        sense[constr] = 'U'; // Upper bound means LHS >= RHS
        rhs[constr] = pdbHeuristics[constr];
    }

    status = CPXaddrows (env, lp, 0, numRows, numnz, rhs, sense, rmatbeg,
                         rmatind, rmatval, NULL, rowname);
    return (status);

}  /* END populateByRow */

void PhOLPSolver::handle_cplex_error(CPXENVptr env, int status) {
    abort();
}

CPXLPptr PhOLPSolver::createProblem(CPXENVptr env, const string &name) {
    int status = 0;
    CPXLPptr problem = CPXcreateprob(env, &status, name.c_str());
    if (status) {
        handle_cplex_error(env, status);
    }
    return problem;
}


void PhOLPSolver::freeProblem(CPXENVptr env, CPXLPptr *problem) {
    CPX_CALL(CPXfreeprob, env, problem);
}

int main(int argc, char *argv[]) {
    PhOLPSolver::numRows = 2;
    PhOLPSolver::numCols = 2;
    PhOLPSolver::numnz = 4;
    return PhOLPSolver::run();
}
