//
// Created by heuser0000 on 4/28/24.
//

#include "LPSolver.h"


LPSolver::LPSolver(int numRows, int numCols, int numnz, const std::vector<std::vector<int>> &patterns, bool verbose) {
    this->numRows = numRows;
    this->numCols = numCols;
    this->numnz = numnz;
    this->verbose = verbose;
    int status = 0;
    env = CPXopenCPLEX(&status);
    lp = createProblem("PhOLP");
    CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_OFF); // Console output stuff
    init(patterns);
}

/* To populate by row, we first create the columns, and then add the
   rows.  */
int LPSolver::init(const std::vector<std::vector<int>> &patterns) {
    std::cout << "Initializing Solver\n";
    int status;
    double obj[numCols];
    double lb[numCols];
    double ub[numCols];
    int rmatbeg[numRows];
    int rmatind[numnz];
    double rmatval[numnz];
    double rhs[numRows];
    char sense[numRows];

    indices.reserve(patterns.size());
    for (int i = 0; i < patterns.size(); i++) {
        indices.push_back(i);
    }
    status = CPXchgobjsen(env, lp, CPX_MIN);  /* Problem is minimization */
    if (status) return status;

    /* COLUMNS */
    for (int col = 0; col < numCols; col++) {
        obj[col] = 1.0;
        lb[col] = 0.0;
        ub[col] = CPX_INFBOUND;
    }
    status = CPXnewcols(env, lp, numCols, obj, lb, ub, nullptr, nullptr);
    if (status) return status;

    /* ROWS */
    int counter = 0;
    for (int row = 0; row < numRows; row++) {
        rmatbeg[row] = counter;
        const std::vector<int> &pattern = patterns[row];
        for (int i: pattern) {
            if (i != 0) {
                rmatval[counter] = 1.0;
                rmatind[counter] = i - 1;
                counter++;
            }
        }
        sense[row] = 'G'; // Upper bound means LHS >= RHS
        rhs[row] = 0;
    }

    status = CPXaddrows(env, lp, 0, numRows, numnz, rhs, sense, rmatbeg,
                        rmatind, rmatval, nullptr, nullptr);
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
    std::cout << "An Error occured in a CPLEX Call. Error Code: " << status << std::endl;
    abort();
}

double LPSolver::solve(const std::vector<double> &rhs) {

    // SET RHS
    CPX_CALL(CPXchgrhs, env, lp, rhs.size(), indices.data(), rhs.data());

    // SOLVE
    CPX_CALL(CPXlpopt, env, lp);

    // GET RESULT
    double value;
    CPX_CALL(CPXgetobjval, env, lp, &value);
    return value;
}

LPSolver::~LPSolver() {
    std::cout << "Destructing Solver" << std::endl;
    freeProblem(&lp);
    CPXcloseCPLEX(&env);
}

void LPSolver::writeProblem() {
    const char * filename_str = "/infai/heuser0000/stp-pho-solver/build/problem.lp";
    CPX_CALL(CPXwriteprob, env, lp, filename_str, nullptr);
}
