#define OS_MAC

#include "PHO-solver.h"

enum HeuristicType {
    PHO,
    SUM,
    MAX
};


int main(int argc, char *argv[]) {

    std::cout << std::endl << "------------\n"
                              "DEBUG:\n"
                              "------------";
    // Command line options
    std::vector<int> startState;
    int numPatterns = 0;
    bool verbose = false;
    int korfIndex = -1;
    bool useHeuser = false;
    int random = 0;
    HeuristicType heuristicType;
    std::vector<slideDir> path;
    std::vector<int> inputpattern;
    std::vector<std::vector<int>> patterns;

    utils::register_event_handlers();

    // Command line parsing
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);
    app.add_option("-s", startState, "Specify a start state");
    app.add_option("-n", numPatterns, "Specify the number of patterns");
    app.add_option("--pdbPathPrefix", pdbPath, "Specify the path to the directory where your"
                                               " .pdb files are located")
                                               ->default_val("/infai/heuser0000/stp-pho-solver/PDBFILES/");
    app.add_option("--pattern", patterns, "Specify a pattern to use/search");
    app.add_option("-p", patterns, "Specify a pattern to use/search");
    app.add_option("-v", verbose, "If used, debugging printouts are enabled")->default_val(false);
    app.add_option("-k", korfIndex, "Use given korf instance as start state. Must be in range 0-99");
    app.add_option("-H", useHeuser, "Use given heuser instance as start state. Must be in range 0-99");
    app.add_option("-r", random, "Use random state with walk length n");
    app.add_option("--heuristic", heuristicType, "Choose heuristic: 0 = PHO, 1 = Sum, 2 = Max")->default_val(0);
    CLI11_PARSE(app, argc, argv);

    MNPuzzle<GRID_SIZE, GRID_SIZE> mnp;
    MNPuzzleState<GRID_SIZE, GRID_SIZE> goal;
    MNPuzzleState<GRID_SIZE, GRID_SIZE> input;
    IDAStar<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, false> idaStar;


    if (korfIndex > 99) {
        korfIndex = 99;
    }
    // parse start state
    // transfer start state to mnpuzzle datastructure
    if (korfIndex <= -1) {
        if (random > 0) {
            input = STP::GetRandomInstance(random);
        } else {
            if (startState.size() > 16) {
                std::cout << "ERROR: Grid sizes > 4 are not supported (for now)." << std::endl;
                return 0;
            }
            for (int i = 0; i < startState.size(); i++) {
                input.puzzle[i] = startState[i];
                if (startState[i] == 0) { input.blank = i; }
            }
            if (!input.isSolvable()) {
                std::cout << "ERROR: Problem didn't pass solvability check." << std::endl;
                utils::exit_with(utils::ExitCode::SEARCH_INPUT_ERROR);
            } else {
                std::cout << "Passed solvability check" << std::endl;
            }
        }
    } else if (useHeuser) {
        input = STP::GetHeuserInstance(korfIndex);
    } else {
        input = STP::GetKorfInstance(korfIndex);
    }

    std::cout << "Start state: " << input << std::endl;
    std::cout << "Goal state: " << goal << std::endl;

    std::vector<LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>>> heuristicVec;

    heuristicVec.reserve(patterns.size());
    for (const std::vector<int> &pattern: patterns) {
        // Load PDB
        LexPermutationPDB<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, MNPuzzle<GRID_SIZE, GRID_SIZE>> permutationPdb(
                &mnp, goal, pattern);
        if (verbose) {
            std::cout << "Path to pdb: " << permutationPdb.GetFileName(pdbPath.c_str()) << std::endl;
            std::cout << "Attempting to load pattern: ";
            for (int i: pattern) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        if (LoadSTPPDB<GRID_SIZE, GRID_SIZE>(permutationPdb)) {
            std::cout << "ERROR: Couldn't load pattern" << std::endl;
            utils::exit_with(utils::ExitCode::SEARCH_INPUT_ERROR);
        }
        heuristicVec.push_back(permutationPdb);
    }

    // INIT HEURISTIC
    Heuristic<MNPuzzleState<GRID_SIZE, GRID_SIZE>> *heuristic;
    switch (heuristicType) {
        case PHO:
            std::cout << "Heuristic: pho" << std::endl;
            heuristic = new PHOHeuristic<STATE, slideDir, ENVIRONMENT>(std::move(heuristicVec),patterns, verbose);
            break;
        case SUM:
            std::cout << "Heuristic: sum" << std::endl;
            heuristic = new SumHeuristic<STATE, slideDir, ENVIRONMENT>(std::move(heuristicVec), verbose);
            break;
        case MAX:
            std::cout << "Heuristic: max" << std::endl;
            heuristic = new MaxPDBHeuristic<STATE, slideDir, ENVIRONMENT>(std::move(heuristicVec));
            break;
        default:
            std::cout << "ERROR: Invalid Heuristic" << std::endl;
            utils::exit_with(utils::ExitCode::SEARCH_INPUT_ERROR);
    }
    
    std::cout << std::endl << "------------\n"
                              "METRICS:\n"
                              "------------\n";
    // IDASTAR
    idaStar.SetHeuristic(heuristic);
    auto start = std::chrono::system_clock::now();
    idaStar.GetPath(&mnp, input, goal, path);
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "System time: " << diff.count() << " μs" << std::endl;
    std::cout << "Solving: " << PHOHeuristic<STATE, slideDir, ENVIRONMENT>::timeSpentSolving << " μs" << std::endl;
    std::cout << "Retrieving: " << PHOHeuristic<STATE, slideDir, ENVIRONMENT>::timeSpentRetrieving << " μs" << std::endl;

    // Print Solution
    std::cout << "Solution: ";
    for (slideDir n: path) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}


