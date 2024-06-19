//
// Created by Benedikt Heuser on 18.04.24.
//

#ifndef STP_PHO_SOLVER_PHO_SOLVER_H
#define STP_PHO_SOLVER_PHO_SOLVER_H

#include <iostream>
#include "../hog2/environments/MNPuzzle.h"
#include "../hog2/generic/IDAStar.h"
#include "../cli/CLI11.hpp"
#include "../hog2/search/LexPermutationPDB.h"
#include "PHOHeuristic.h"
#include "../hog2/environments/STPInstances.h"
#include <chrono>
#include "../downward/system.h"
#include "SumHeuristic.h"
#include "MaxPDBHeuristic.h"

#define GRID_SIZE 4
#define STATE MNPuzzleState<GRID_SIZE, GRID_SIZE>
#define ENVIRONMENT MNPuzzle<GRID_SIZE, GRID_SIZE>

bool additive = false; // Set to true for additive PDB
bool delta = false; // Has something to do with compression
int compression = 1; // Compression factor (probably)
int threads = std::thread::hardware_concurrency();
std::string pdbPath = "/infai/heuser0000/stp-pho-solver/PDBFILES";
//std::vector<int> pattern;

template<int width, int height>
int LoadSTPPDB(LexPermutationPDB<MNPuzzleState<width, height>, slideDir, MNPuzzle<width, height>> &pdb) {
    if (pdb.Load(pdbPath.c_str())) {
        if (compression != 1) {
            pdb.DivCompress(compression, true);
        }
        return 0;
    }
    return 1;
}

#endif //STP_PHO_SOLVER_PHO_SOLVER_H
