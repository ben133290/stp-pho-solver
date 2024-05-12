#define OS_MAC

#include "PHO-solver.h"

int main(int argc, char *argv[]) {

    std::vector<int> startState;
    int numPatterns = 0;
    bool verbose = false;
    int korfIndex = -1;

    // Command line parsing
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);
    app.add_option("-s", startState, "Specify a start state");
    app.add_option("-n", numPatterns, "Specify the number of patterns");
    app.add_option("--pdbPathPrefix", pdbPath, "Specify the path to the directory where your .pdb files are located");
    app.add_option("--pattern", patterns, "Specify a pattern to use/search");
    app.add_option("-p", patterns, "Specify a pattern to use/search");
    app.add_option("-v", verbose, "If used, debugging printouts are enabled");
    app.add_option("-k", korfIndex, "Use given korf instance as start state. Must be in range 0-99");
    CLI11_PARSE(app, argc, argv);

    if (korfIndex > 99) {
        korfIndex = 99;
    }
    // parse start state
    // transfer start state to mnpuzzle datastructure
    if (korfIndex <= -1) {
        if (startState.size() > 16) {
            std::cout << "Grid sizes > 4 are not supported (for now)" << std::endl;
            return 0;
        }
        for (int i = 0; i < startState.size(); i++) {
            input.puzzle[i] = startState[i];
            if (startState[i] == 0) { input.blank = i; }
        }
    } else {
        input = STP::GetKorfInstance(korfIndex);
    }

    std::cout << "Start state: " << input << std::endl; // DEBUG

    std::vector<LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>>> heuristicVec;

    heuristicVec.reserve(patterns.size());
    for (const std::vector<int> &pattern: patterns) {
        // Load PDB
        LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>> permutationPdb(
                &mnp, goal, pattern);
        if (verbose) {
            std::cout << "Path to pdb: " << pdbPath << permutationPdb.GetFileName(pdbPath.c_str()) << std::endl;
            std::cout << "Attempting to load pattern: ";
            for (int i: pattern) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        if (LoadSTPPDB<GRID_SIZE, GRID_SIZE>(permutationPdb)) {
            std::cout << "ERROR: Couldn't load pattern" << std::endl;
            return 1;
        }
        heuristicVec.push_back(permutationPdb);
    }

    PHOHeuristic<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>> phoHeuristic(std::move(heuristicVec), patterns, verbose);

    // IDAstar 37454
    std::cout << input << std::endl << goal << std::endl;
    idaStar.SetHeuristic(&phoHeuristic);

    auto start = std::chrono::system_clock::now();
    idaStar.GetPath(&mnp, input, goal, path);
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Total solve time: " << diff.count() << " microseconds" << std::endl;

    // Print Solution

    std::cout << "Solution: ";
    for (slideDir n: path) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}


