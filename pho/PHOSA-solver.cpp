#define OS_MAC

#include "PHOSA-solver.h"


void load_pdb_heuristics(bool verbose, std::vector<std::vector<int>> &patterns, MNPuzzle<4, 4> &mnp,
                         const MNPuzzleState<4, 4> &goal,
                         std::vector<LexPermutationPDB<MNPuzzleState<4, 4>, slideDir, MNPuzzle<4, 4>>> &heuristicVec) {
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
}

void load_patterns_and_weights(std::vector<std::vector<int>> &patterns, std::vector<std::vector<double>> &weights,
                               const std::string& offline_path) {
    std::ifstream file;
    file.open(offline_path);
    int num_patterns;
    file >> num_patterns;
    patterns.reserve(num_patterns);
    for (int i = 0; i < num_patterns; i++) {
        int num_tiles;
        file >> num_tiles;
        std::vector<int> pattern;
        pattern.reserve(num_tiles);
        for (int j = 0; j < num_tiles; j++) {
            int tile;
            file >> tile;
            pattern.push_back(tile);
        }
        patterns.push_back(pattern);
    }

    int num_weights;
    file >> num_weights;
    weights.reserve(num_weights);
    for (int i = 0; i < num_weights; i++) {
        std::vector<double> weight_line;
        weight_line.reserve(num_patterns);
        for (int j = 0; j < num_patterns; j++) {
            double weight;
            file >> weight;
            weight_line.push_back(weight);
        }
        weights.push_back(weight_line);
    }

}

int main(int argc, char *argv[]) {
    // INIT
    std::vector<int> startState;
    int numPatterns = 0;
    bool verbose = false;
    int korfIndex = -1;
    bool useHeuser = false;
    int random = 0;
    std::vector<slideDir> path;
    std::vector<std::vector<int>> patterns;
    std::vector<std::vector<double>> weights;
    MNPuzzle<GRID_SIZE, GRID_SIZE> mnp;
    MNPuzzleState<GRID_SIZE, GRID_SIZE> goal;
    MNPuzzleState<GRID_SIZE, GRID_SIZE> input;
    IDAStar<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, false> idaStar;
    utils::register_event_handlers();

    // CLI
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);
    app.add_option("-s", startState, "Specify a start state");
    app.add_option("--pdbPathPrefix", pdbPath, "Specify the path to the directory where your"
                                               " .pdb files are located")
                                               ->default_val("/infai/heuser0000/stp-pho-solver/PDBFILES/");
    app.add_option("-v", verbose, "If used, debugging printouts are enabled")->default_val(false);
    app.add_option("-k", korfIndex, "Use given korf instance as start state. Must be in range 0-99");
    app.add_option("-H", useHeuser, "Use given heuser instance as start state. Must be in range 0-99");
    app.add_option("-r", random, "Use random state with walk length n");
    CLI11_PARSE(app, argc, argv);



    // START STATE PARSING
    if (korfIndex > 99) {
        korfIndex = 99;
    }
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

    load_patterns_and_weights(patterns, weights, "/infai/heuser0000/stp-pho-solver/opho.txt");
    load_pdb_heuristics(verbose, patterns, mnp, goal, heuristicVec);


    // INIT HEURISTIC
    PHOSAHeuristic<STATE, slideDir, ENVIRONMENT> heuristic(std::move(heuristicVec), std::move(weights));
    std::cout << "Heuristic: phosa" << std::endl;
    
    std::cout << std::endl << "------------\n"
                              "METRICS:\n"
                              "------------\n";
    // IDASTAR
    idaStar.SetHeuristic(&heuristic);
    auto start = std::chrono::system_clock::now();
    idaStar.GetPath(&mnp, input, goal, path);
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "System time: " << diff.count() << " μs" << std::endl;

    // Print Solution
    std::cout << "Solution: ";
    for (slideDir n: path) {
        std::cout << n << " ";
    }
    std::cout << std::endl;


    return 0;

}


