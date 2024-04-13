/*
 *  Driver.cpp
 *  hog
 *
 *  Created by Nathan Sturtevant on 10/1/19.
 *
 *  This file is part of HOG.
 */

#include <cstring>
#include "../search/PermutationPDB.h"
#include "../search/LexPermutationPDB.h"
#include "Driver.h"
#include "../environments/MNPuzzle.h"
//#include "RubiksCube.h"
#include "../utils/Timer.h"
//#include "TopSpin.h"
//#include "STPInstances.h"
//#include "TOH.h"

template <int width, int height>
void BuildSTPPDB();
void BuildRC();
template <int tiles>
void BuildTS();
void BuildTOH();

enum Domain {
	kRC,
	kSTP44,
	kSTP55,
	kTS16,
	kTOH
};
Domain domain = kSTP44;
bool additive = false;
bool load = false;
bool delta = false;
std::vector<int> pattern;
std::string path;
int threads = std::thread::hardware_concurrency();
int compression = 1;

int MyCLHandler(char *argument[], int maxNumArgs) // TODO: Make options settable
{
	if (strcmp(argument[0], "-domain") == 0)
	{
		if (maxNumArgs <= 1)
		{
			printf("Need domain: STP44 STP55 RC TS16 TOH\n");
			exit(0);
		}
		if (strcmp(argument[1], "RC") == 0)
			domain = kRC;
		else if (strcmp(argument[1], "STP44") == 0)
			domain = kSTP44;
		else if (strcmp(argument[1], "STP55") == 0)
			domain = kSTP55;
		else if (strcmp(argument[1], "TS16") == 0)
			domain = kTS16;
		else if (strcmp(argument[1], "TOH") == 0)
			domain = kTOH;
		return 2;
	}
	if (strcmp(argument[0], "-additive") == 0)
	{
		additive = true;
		return 1;
	}
	if (strcmp(argument[0], "-delta") == 0)
	{
		delta = true;
		return 1;
	}
	if (strcmp(argument[0], "-load") == 0)
	{
		load = true;
		return 1;
	}
	if (strcmp(argument[0], "-threads") == 0)
	{
		if (maxNumArgs > 1)
			threads = atoi(argument[1]);
		else
			printf("Error processing thread count; defaulting to %d\n", threads);
		return 2;
	}
	if (strcmp(argument[0], "-compress") == 0)
	{
		if (maxNumArgs > 1)
			compression = atoi(argument[1]);
		else
			printf("Error processing compression factor; defaulting to %d\n", compression);
		return 2;
	}
	if (strcmp(argument[0], "-path") == 0)
	{
		path = argument[1];
		return 2;
	}
	
	if (strcmp(argument[0], "-pattern") == 0)
	{
		int cnt = 1;
		maxNumArgs--;
		while (maxNumArgs > 0 && argument[cnt][0] != '-')
		{
			maxNumArgs--;
			pattern.push_back(atoi(argument[cnt]));
			cnt++;
		}
		return cnt;
	}

	return 1;
}

template <int width, int height>
void BuildSTPPDB()
{
	MNPuzzle<width, height> mnp;
	MNPuzzleState<width, height> goal;
	std::vector<slideDir> moves;
	goal.Reset();
	mnp.StoreGoal(goal);

	LexPermutationPDB<MNPuzzleState<width, height>, slideDir, MNPuzzle<width, height>> pdb(&mnp, goal, pattern);
	if (load)
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
	}
	
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
		pdb.Save(path.c_str());
	}
	else {
		pdb.BuildPDB(goal, threads);
		if (delta)
			pdb.DeltaCompress(&mnp, goal, true);
		if (compression != 1)
		{
			pdb.DivCompress(compression, true);
		}
		pdb.Save(path.c_str());
	}
}

