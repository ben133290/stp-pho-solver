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

#define GRID_SIZE 4

bool additive = false; // Set to true for additive PDB
bool delta = false; // Has something to do with compression
int compression = 1; // Compression factor (probably)
int threads = std::thread::hardware_concurrency();
std::string pdbPath;
//std::vector<int> pattern;

MNPuzzle<GRID_SIZE, GRID_SIZE> mnp;
MNPuzzleState<GRID_SIZE, GRID_SIZE> goal;
MNPuzzleState<GRID_SIZE, GRID_SIZE> input;

std::vector<slideDir> path;
std::vector<int> inputpattern;
std::vector<std::vector<int>> patterns;

IDAStar<MNPuzzleState<GRID_SIZE, GRID_SIZE>, slideDir, true> idaStar;

template<int width, int height>
int LoadSTPPDB(LexPermutationPDB<MNPuzzleState<width, height>, slideDir, MNPuzzle<width, height>> &pdb) {
    if (pdb.Load(pdbPath.c_str())) {
        printf("Loaded successfully\n\n");
        //pdb.PrintHistogram();

        if (compression != 1) {
            pdb.DivCompress(compression, true);
        }
        return 0;
    }
    return 1;
}

#endif //STP_PHO_SOLVER_PHO_SOLVER_H
