//
// Created by heuser0000 on 4/28/24.
//

#include "LPSolver.h"

LPSolver::LPSolver(int numRows, int numCols, int numnz,
                   const std::vector<std::vector<int>> &patterns,
                   const std::vector<double> &rhs) {
    this->numRows = numRows;
    this->numCols = numCols;
    this->numnz = numnz;
    int status = 0;
    env = CPXopenCPLEX(&status);
    lp = createProblem("PhOLP");
    CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);
    init(patterns, rhs);
}

/* To populate by row, we first create the columns, and then add the
   rows.  */
int LPSolver::init(const std::vector<std::vector<int>> &patterns,
                   const std::vector<double> &pdbHeuristics) {
    int status;
    double obj[numCols];
    double lb[numCols];
    double ub[numCols];
    char *colname[numCols];
    int rmatbeg[numRows];
    int rmatind[numnz];
    double rmatval[numnz];
    double rhs[numRows];
    char sense[numRows];
    char *rowname[numRows];

    status = CPXchgobjsen(env, lp, CPX_MIN);  /* Problem is minimization */
    if (status) return status;

    /* COLUMNS */
    for (int col = 0; col < numCols; col++) {
        obj[col] = 1.0;
        lb[col] = 0.0;
        ub[col] = CPX_INFBOUND;
        colname[col] = "col";
    }
    status = CPXnewcols(env, lp, numCols, obj, lb, ub, NULL, colname);
    if (status) return status;

    /* ROWS */
    int counter = 0;
    for (int row = 0; row < numRows; row++) {
        rmatbeg[row] = counter;
        rowname[row] = "row";
        std::vector<int> pattern = patterns[row];
        for (int i: pattern) {
            rmatval[counter] = 1.0;
            rmatind[counter] = i;
            counter++;
        }
        sense[row] = 'U'; // Upper bound means LHS >= RHS
        rhs[row] = pdbHeuristics[row];
    }

    status = CPXaddrows(env, lp, 0, numRows, numnz, rhs, sense, rmatbeg,
                        rmatind, rmatval, NULL, rowname);
    return (status);

}  /* END populateByRow */

void LPSolver::freeProblem(CPXLPptr *problem) {
    CPX_CALL(CPXfreeprob, env, problem);
}

CPXLPptr LPSolver::createProblem(const std::string &name) {
    int status = 0;
    CPXLPptr problem = CPXcreateprob(env, &status, name.c_str());
    if (status) {
        handle_cplex_error(env, status);
    }
    return problem;
}

void LPSolver::handle_cplex_error(CPXENVptr env, int status) {
    std::cout << "An Error occured in a CPLEX Call :(" << std::endl;
    abort();
}

double LPSolver::solve(const std::vector<double> &rhs) {

    // SET RHS
    std::vector<int> indices;
    indices.reserve(rhs.size());
    for (int i = 0; i < rhs.size(); i++) {
        indices.push_back(i);
    }
    CPX_CALL(CPXchgrhs, env, lp, rhs.size(), indices.data(), rhs.data());

    // SOLVE
    CPX_CALL(CPXlpopt, env, lp);

    // GET RESULT
    double value;
    CPX_CALL(CPXgetobjval, env, lp, &value);
    std::cout << "result: " << value << std::endl;

    freeProblem(&lp);
    CPXcloseCPLEX(&env);

    return value;
}

int main(int argc, char *argv[]) {
    //temporary bad stuff
    std::vector<int> pattern1;
    pattern1.push_back(0);
    pattern1.push_back(1);

    std::vector<int> pattern2;
    pattern2.push_back(0);

    std::vector<std::vector<int>> patterns;
    patterns.push_back(pattern1);
    patterns.push_back(pattern2);

    std::vector<double> heuristics;
    heuristics.push_back(20);
    heuristics.push_back(10);
    LPSolver lpSolver(2, 2, 4, patterns, heuristics);

    std::vector<double> heuristics2;
    heuristics2.push_back(10);
    heuristics2.push_back(10);
    std::cout << "result: " << lpSolver.solve(heuristics2) << std::endl;
    return 0;
}
