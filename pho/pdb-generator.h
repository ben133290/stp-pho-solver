//
// Created by Benedikt Heuser on 16.04.24.
//

#ifndef STP_PHO_SOLVER_PDB_GENERATOR_H
#define STP_PHO_SOLVER_PDB_GENERATOR_H

#include <iostream>
#include "../hog2/environments/MNPuzzle.h"
#include "../cli/CLI11.hpp"
#include "../hog2/search/LexPermutationPDB.h"

#define GRID_SIZE 4

// Stuff for Building PDB
template<int width, int height>
void BuildSTPPDB();

bool additive = false; // Set to true for additive PDB
bool delta = false; // Has something to do with compression
int compression = 1; // Compression factor (probably)
int threads = std::thread::hardware_concurrency();
std::string pdbPath;

MNPuzzle<GRID_SIZE, GRID_SIZE> mnp;
MNPuzzleState<GRID_SIZE, GRID_SIZE> goal;

std::vector<int> pattern;

void initCLI(char **&argv, CLI::App &app);

template<int width, int height>
void BuildSTPPDB() {
    MNPuzzle<width, height> mnp;
    std::vector<slideDir> moves;
    goal.Reset();
    mnp.StoreGoal(goal);

    LexPermutationPDB<MNPuzzleState<width, height>, slideDir, MNPuzzle<width, height>> pdb(&mnp, goal, pattern);

    if (additive) {
        mnp.SetPattern(pattern);
        pdb.BuildAdditivePDB(goal, threads); // parallelism not fixed yet
        if (delta)
            pdb.DeltaCompress(&mnp, goal, true);
        if (compression != 1) {
            pdb.DivCompress(compression, true);
        }
        pdb.Save(pdbPath.c_str());
    } else {
        pdb.BuildPDB(goal, threads);
        if (delta)
            pdb.DeltaCompress(&mnp, goal, true);
        if (compression != 1) {
            pdb.DivCompress(compression, true);
        }
        pdb.Save(pdbPath.c_str());
    }
}

#endif //STP_PHO_SOLVER_PDB_GENERATOR_H
