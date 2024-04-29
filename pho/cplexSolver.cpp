//
// Created by Benedikt Heuser on 22.04.24.
//

#include "cplexSolver.h"
#include <cplex.h>
#include <string>
#include <iostream>

using namespace std;

#define NUMROWS    2
#define NUMCOLS    3
#define NUMNZ      6

/* To populate by row, we first create the columns, and then add the
   rows.  */

static int
populatebyrow (CPXENVptr env, CPXLPptr lp)
{
    int      status    = 0;
    double   obj[NUMCOLS];
    double   lb[NUMCOLS];
    double   ub[NUMCOLS];
    char     *colname[NUMCOLS];
    int      rmatbeg[NUMROWS];
    int      rmatind[NUMNZ];
    double   rmatval[NUMNZ];
    double   rhs[NUMROWS];
    char     sense[NUMROWS];
    char     *rowname[NUMROWS];

    status = CPXchgobjsen (env, lp, CPX_MAX);  /* Problem is maximization */
    if ( status )  goto TERMINATE;

    /* Now create the new columns.  First, populate the arrays. */

    obj[0] = 1.0;      obj[1] = 2.0;          obj[2] = 3.0;

    lb[0] = 0.0;       lb[1] = 0.0;           lb[2] = 0.0;
    ub[0] = 40.0;      ub[1] = CPX_INFBOUND;  ub[2] = CPX_INFBOUND;

    colname[0] = "x1"; colname[1] = "x2";     colname[2] = "x3";

    status = CPXnewcols (env, lp, NUMCOLS, obj, lb, ub, NULL, colname);
    if ( status )  goto TERMINATE;

    /* Now add the constraints.  */

    rmatbeg[0] = 0;     rowname[0] = "c1";

    rmatind[0] = 0;     rmatind[1] = 1;    rmatind[2] = 2;    sense[0] = 'L';
    rmatval[0] = -1.0;  rmatval[1] = 1.0;  rmatval[2] = 1.0;  rhs[0]   = 20.0;

    rmatbeg[1] = 3;     rowname[1] = "c2";
    rmatind[3] = 0;     rmatind[4] = 1;    rmatind[5] = 2;    sense[1] = 'L';
    rmatval[3] = 1.0;   rmatval[4] = -3.0; rmatval[5] = 1.0;  rhs[1]   = 30.0;

    status = CPXaddrows (env, lp, 0, NUMROWS, NUMNZ, rhs, sense, rmatbeg,
                         rmatind, rmatval, NULL, rowname);
    if ( status )  goto TERMINATE;

    TERMINATE:

    return (status);

}  /* END populateByRow */

void handle_cplex_error(CPXENVptr env, int status) {
    abort();
}

/* Make a call to a CPLEX API function checking its return status. */
template<typename Func, typename ... Args>
static void CPX_CALL(Func cpxfunc, CPXENVptr env, Args && ... args) {
    int status = cpxfunc(env, std::forward<Args>(args) ...);
    if (status) {
        handle_cplex_error(env, status);
    }
}

static CPXLPptr createProblem(CPXENVptr env, const string &name) {
    int status = 0;
    CPXLPptr problem = CPXcreateprob(env, &status, name.c_str());
    if (status) {
        handle_cplex_error(env, status);
    }
    return problem;
}


static void freeProblem(CPXENVptr env, CPXLPptr *problem) {
    CPX_CALL(CPXfreeprob, env, problem);
}

int main(int argc, char *argv[]) {
    int status = 0;
    CPXENVptr env = CPXopenCPLEX(&status);
    CPXLPptr lp = createProblem(env, "name");

    CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);
    populateByRow(env, lp);
    CPX_CALL(CPXlpopt, env, lp);
    double value;
    CPX_CALL(CPXgetobjval, env, lp, &value);
    std::cout << "result: " << value;

    freeProblem(env, &lp);
    status = CPXcloseCPLEX(&env);

    return 0;

}
