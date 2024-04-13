#define OS_MAC

#include <iostream>
#include "hog2/environments/MNPuzzle.h"
#include "hog2/generic/IDAStar.h"
#include "cli/CLI11.hpp"
#include "hog2/search/LexPermutationPDB.h"

MNPuzzle<4, 4> mnp;
MNPuzzleState<4, 4> goal;
MNPuzzleState<4, 4> input;

std::vector<slideDir> path;
std::vector<int> distincts;

IDAStar<MNPuzzleState<4, 4>, slideDir, true> idaStar;

LexPermutationPDB<MNPuzzleState<4, 4>, slideDir, MNPuzzle<4, 4>> permutationPdb(&mnp, goal, distincts);


int main(int argc, char *argv[]) {

    // Command line parsing
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);

    std::vector<int> startState;
    app.add_option("-s", startState, "Specify a start state");

    CLI11_PARSE(app, argc, argv);

    // parse start state
    if (startState.size() > 16) {
        return 0;
    }

    for (int i = 0; i < startState.size(); i++) {
        input.puzzle[i] = startState[i];
    }

    std::cout << "Start state: " << input << std::endl;


    permutationPdb.BuildPDB(goal);

    idaStar.SetHeuristic(&permutationPdb);

    //idaStar.GetPath(&mnp, input, goal, path);



    return 0;
}
