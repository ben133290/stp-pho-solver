#define OS_MAC

#include <iostream>
#include "hog2/environments/MNPuzzle.h"
#include "hog2/generic/IDAStar.h"
#include "cli/CLI11.hpp"
#include "hog2/search/LexPermutationPDB.h"

// Stuff for Building PDB
template <int width, int height>
void BuildSTPPDB();
bool additive = false; // Set to true for additive PDB
bool delta = false; // Has something to do with compression
int compression = 1; // Compression factor (probably)
int threads = std::thread::hardware_concurrency();
std::string pdbPath;
//std::vector<int> pattern;

MNPuzzle<4, 4> mnp;
MNPuzzleState<4, 4> goal;
MNPuzzleState<4, 4> input;

std::vector<slideDir> path;
std::vector<int> pattern;

IDAStar<MNPuzzleState<4, 4>, slideDir, true> idaStar;




int main(int argc, char *argv[]) {

    // Command line parsing
    CLI::App app{"Post hoc optimization solver for sliding tile puzzle"};
    argv = app.ensure_utf8(argv);

    std::vector<int> startState;
    app.add_option("-s", startState, "Specify a start state");
    app.add_option("-p", pattern, "Specify a pattern for pdb");

    CLI11_PARSE(app, argc, argv);

    // parse start state
    if (startState.size() > 16) {
        return 0;
    }

    for (int i = 0; i < startState.size(); i++) {
        input.puzzle[i] = startState[i];
    }

    std::cout << "Start state: " << input << std::endl;
    std::cout << "Pattern: ";
    for (int n : pattern) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    //
    LexPermutationPDB<MNPuzzleState<4, 4>, slideDir, MNPuzzle<4, 4>> permutationPdb(&mnp, goal, pattern);

    permutationPdb.BuildPDB(goal, threads);

    idaStar.SetHeuristic(&permutationPdb);

    idaStar.GetPath(&mnp, input, goal, path);



    return 0;
}

template <int width, int height>
void BuildSTPPDB()
{
    MNPuzzle<width, height> mnp;
    std::vector<slideDir> moves;
    goal.Reset();
    mnp.StoreGoal(goal);

    LexPermutationPDB<MNPuzzleState<width, height>, slideDir, MNPuzzle<width, height>> pdb(&mnp, goal, pattern);
    /*if (load)
    {
        if (pdb.Load(path.c_str()))
        {
            printf("Loaded successfully\n");
            pdb.PrintHistogram();

            if (compression != 1)
            {
                pdb.DivCompress(compression, true);
            }
            return;
        }
    }*/

    if (additive)
    {
        mnp.SetPattern(pattern);
        pdb.BuildAdditivePDB(goal, threads); // parallelism not fixed yet
        if (delta)
            pdb.DeltaCompress(&mnp, goal, true);
        if (compression != 1)
        {
            pdb.DivCompress(compression, true);
        }
        //pdb.Save(path.c_str());
    }
    else {
        pdb.BuildPDB(goal, threads);
        if (delta)
            pdb.DeltaCompress(&mnp, goal, true);
        if (compression != 1)
        {
            pdb.DivCompress(compression, true);
        }
        //pdb.Save(path.c_str());
    }
}
