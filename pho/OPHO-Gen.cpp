//
// Created by heuser0000 on 7/2/24.
//

#include "../cli/CLI11.hpp"
#include "../hog2/environments/MNPuzzle.h"
#include "../hog2/environments/STPInstances.h"
#include "LPSolver.h"
#include <random>
#include <fstream>
#include "../hog2/search/LexPermutationPDB.h"
#include "../downward/system.h"

int computeNZS(std::vector<std::vector<int>> &patterns) {
    int nzs = 0;
    for (const auto & pattern : patterns) {
        nzs += pattern.size() - 1;
    }
    return nzs;
}

std::string pdbPath = "/infai/heuser0000/stp-pho-solver/PDBFILES";

int LoadSTPPDB(LexPermutationPDB<MNPuzzleState<4, 4>, slideDir, MNPuzzle<4, 4>> &pdb) {
    if (pdb.Load(pdbPath.c_str())) {
        return 0;
    }
    return 1;
}

double weighted_sum(std::vector<double> &a, std::vector<double> &b) {
    double result = 0;
    for (int i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    }
    return result;
}

void walk_random_states(const int start, const int end, const int m, std::vector<MNPuzzleState<4,4>> &sample_states) {
    sample_states.reserve((end-start) * m);
    for (int i = start; i < end; i++) {
        for (int j = 0; j < m; j++) {
            sample_states.push_back(STP::GetRandomInstance(i));
        }
    }
}

void build_random_states(const int n, std::vector<MNPuzzleState<4,4>> &sample_states) {
    for (int i = 0; i < n; i++) {
        MNPuzzleState<4,4> newstate = MNPuzzleState<4,4>();
        sample_states.push_back(newstate);
        std::vector<int> numbers(16);
        for (int j = 0; j < 16; j++) {
            numbers[j] = j;
        }

        std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
        // Copy to state
        for (int j = 0; j < 16; j++) {
            sample_states[i].puzzle[j] = numbers[j];

        }

        // Make solvable
        if (!sample_states[i].isSolvable()) {
            int temp = sample_states[i].puzzle[0];
            sample_states[i].puzzle[0] = sample_states[i].puzzle[1];
            sample_states[i].puzzle[1] = temp;
        }

        // Set blank tile
        for (int j = 0; j < 16; j++) {
            if (numbers[j] == 0) {
                sample_states[i].blank = j;
            }
        }
    }
}

void write_to_file(const std::vector<std::vector<int>> &patterns, const std::vector<std::vector<double>> &weights,
                   std::ofstream &file) {
    file << patterns.size() << std::endl;
    for (const std::vector<int>& pattern : patterns) {
        file << pattern.size() << std::endl;
        for (int tile : pattern) {
            file << tile << " ";
        }
        file << std::endl;
    }

    file << weights.size() << std::endl;
    for (const std::vector<double>& weight_line : weights) {
        for (double weight : weight_line) {
            file << weight << " ";
        }
        file << std::endl;
    }
}


int main(int argc, char *argv[]) {

    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    std::vector<std::vector<int>> patterns;
    app.add_option("-p", patterns, "Specify a pattern to use/search");
    int walklength = 100;
    app.add_option("-e", walklength, "Generate instances up to walk length");
    int num_inst_per_walk = 1;
    app.add_option("-j", num_inst_per_walk, "Intances per walk length");
    int start = 1;
    app.add_option("-s", num_inst_per_walk, "Start walklength");
    int generate_random = 0;
    app.add_option("-r", generate_random, "is random");
    std::string file_path = "/infai/heuser0000/opho.txt";
    app.add_option("-f", file_path, "file path");
    bool optimization_one = false;
    app.add_option("-o", optimization_one, "optimize set of PDBS");
    bool optimization_two = false;
    app.add_option("-O", optimization_two, "optimize set of weights");

    CLI11_PARSE(app, argc, argv);

    std::vector<MNPuzzleState<4,4>> sample_states;

    // Remember rhs values
    std::vector<std::vector<double>> pdb_heuristics_for_states;

    if (generate_random) {
        sample_states.reserve(generate_random);
        build_random_states(generate_random, sample_states);
    }
    else if (walklength && num_inst_per_walk) {
        sample_states.reserve(walklength * num_inst_per_walk);
        walk_random_states(start, walklength, num_inst_per_walk, sample_states);
    }

    LexPermutationPDB<MNPuzzleState<4, 4>, slideDir, MNPuzzle<4, 4>> pdb();
    MNPuzzle<4, 4> mnp;
    MNPuzzleState<4, 4> goal;

    LPSolver lpSolver(patterns.size(), 15, computeNZS(patterns), patterns, false);
    std::vector<std::vector<double>> weights;
    weights.reserve(sample_states.size());
    std::cout << "states:" << std::endl;
    for (MNPuzzleState<4,4> & sample_state : sample_states) {
        std::cout << sample_state << std::endl;
        std::vector<double> rhs;
        rhs.reserve(patterns.size());
        for (int j = 0; j < patterns.size(); j++) {
            // Load PDB
            LexPermutationPDB<MNPuzzleState<4, 4>, slideDir, MNPuzzle<4, 4>> permutationPdb(
                    &mnp, goal, patterns[j]);
            if (LoadSTPPDB(permutationPdb)) {
                std::cout << "ERROR: Couldn't load pattern" << std::endl;
                utils::exit_with(utils::ExitCode::SEARCH_INPUT_ERROR);
            }
            // Solve for state
            rhs.push_back(permutationPdb.HCost(sample_state, goal));
            // Free memory of PDB
            // TODO
        }
        if (optimization_two) {
            pdb_heuristics_for_states.push_back(rhs);
        }
        std::vector<double> weight_line = lpSolver.getDualSol(rhs);
        for (double weight : weight_line) {
            std::cout << weight << " ";
        }
        std::cout << std::endl;
        weights.push_back(weight_line);
    }

    double epsilon = 0.001;
    if (optimization_one) {
        for (int i = patterns.size()-1; i >= 0; i--) {
            bool has_nonzero = false;
            for (std::vector<double> weight_line : weights) {
                if (weight_line[i] > epsilon) {
                    has_nonzero = true;
                    break;
                }
            }
            if (!has_nonzero) {
                patterns.erase(patterns.begin()+i);
                for (std::vector<double> & weight_line : weights) {
                    weight_line.erase(weight_line.begin()+i);
                    if (optimization_two) {
                        pdb_heuristics_for_states.erase(pdb_heuristics_for_states.begin()+i);
                    }
                }
            }
        }
    }

    if (optimization_two) {
        for (int i = weights.size()-1; i >= 0; i--) {
            double value_curr = weighted_sum(weights[i], pdb_heuristics_for_states[i]);
            for (int j = i - 1; j >= 0; j--) {
                if (weighted_sum(weights[j], pdb_heuristics_for_states[i]) == value_curr) {
                    weights.erase(weights.begin()+i);
                    pdb_heuristics_for_states.erase(pdb_heuristics_for_states.begin()+i);
                }
            }
        }
    }

    std::ofstream output_file;
    output_file.open(file_path);
    std::cout << "saving to: " << file_path << std::endl;
    write_to_file(patterns,weights,output_file);
    output_file.close();
    return 0;
}