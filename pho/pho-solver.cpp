#define OS_MAC

#include "pho-solver.h"

int main(int argc, char *argv[]) {

    std::vector<int> startState;

    // Command line parsing
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);
    app.add_option("-s", startState, "Specify a start state");
    app.add_option("-n", startState, "Specify the number of patterns");
    app.add_option("--pdbPathPrefix", pdbPath, "Specify the path to the folder where your .pdb files are located");
    app.add_option("--pattern", patterns, "Specify a pattern to use/search");
    app.add_option("-p", patterns, "Specify a pattern to use/search");
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

    std::vector<LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>>> heuristicVec;

    heuristicVec.reserve(patterns.size());
    for (const std::vector<int> &pattern: patterns) {
        // Load PDB
        LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>> permutationPdb(
                &mnp, goal, pattern);
        std::cout << "Path to pdb: " << pdbPath << permutationPdb.GetFileName(pdbPath.c_str()) << std::endl;
        std::cout << "Attempting to load pattern: ";
        for (int i : pattern) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        LoadSTPPDB<GRID_SIZE, GRID_SIZE>(permutationPdb);
        if (permutationPdb.)

        heuristicVec.push_back(permutationPdb);
    }
    MaxPDBHeuristic<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>> maxPdbHeuristic(std::move(heuristicVec));

    // IDAstar 37454
    std::cout << input << std::endl << goal << std::endl;
    idaStar.SetHeuristic(&maxPdbHeuristic);
    idaStar.GetPath(&mnp, input, goal, path);

    // Print Solution
    std::cout << "Solution: \n";
    for (slideDir n: path) {
        std::cout << n << std::endl;
    }

    return 0;
}


