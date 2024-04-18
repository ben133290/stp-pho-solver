#define OS_MAC

#include "pho-solver.h"

int main(int argc, char *argv[]) {

    std::vector<int> startState;

    // Command line parsing
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);
    app.add_option("-s", startState, "Specify a start state");
    app.add_option("--pdbPathPrefix", pdbPath, "Specify the path to the folder where your .pdb files are located");
    app.add_option("--pattern", pattern, "Specify a pattern to use/search");
    CLI11_PARSE(app, argc, argv);

    // parse start state
    // transfer start state to mnpuzzle datastructure
    if (startState.size() > 16) {
        std::cout << "Grid sizes > 4 are not supported (for now)" << std::endl;
        return 0;
    }
    for (int i = 0; i < startState.size(); i++) {
        input.puzzle[i] = startState[i];
        if (startState[i] == 0) { input.blank = i; }
    }

    // DEBUG
    std::cout << "Start state: " << input << std::endl;
    std::cout << "Pattern: ";
    for (int n: pattern) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    //
    LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>> permutationPdb(
            &mnp, goal, pattern);


    std::cout << "Path to pdb: " << pdbPath << std::endl;
    LoadSTPPDB<GRID_SIZE, GRID_SIZE>(permutationPdb);

    // IDAstar
    std::cout << input << std::endl << goal << std::endl;

    idaStar.SetHeuristic(&permutationPdb);
    idaStar.GetPath(&mnp, input, goal, path);

    // Print Solution
    std::cout << "Solution: \n";
    for (slideDir n: path) {
        std::cout << n << std::endl;
    }

    return 0;
}


