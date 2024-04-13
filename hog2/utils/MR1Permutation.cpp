//
//  MR1Permutation.cpp
//  hog2 glut
//
//  Created by Nathan Sturtevant on 6/19/16.
//  Copyright © 2016 University of Denver. All rights reserved.
//

#include "MR1Permutation.h"
#include <string.h>

uint64_t MR1KPermutation::Rank(int *locs, int *dual, int distinctSize, int puzzleSize) const
{
	uint64_t result2 = 0;
	uint64_t multiplier = 1;
	for (int i = 0; i < distinctSize; i++)
	{
		int tmp = dual[i];
		unsigned int tmp2 = locs[i];
		
		result2 += (tmp-i)*multiplier;
		multiplier *= (puzzleSize-i);
		
		if (tmp2 < puzzleSize)
		{
			swap(locs[i], locs[dual[i]]);
			swap(dual[tmp2], dual[i]);
		}
	}
	
	return result2;
}

uint64_t MR1KPermutation::Rank(uint8_t *locs, uint8_t *dual, int distinctSize, int puzzleSize) const
{
	uint64_t result2 = 0;
	uint64_t multiplier = 1;
	for (int i = 0; i < distinctSize; i++)
	{
		int tmp = dual[i];
		unsigned int tmp2 = locs[i];
		
		result2 += (tmp-i)*multiplier;
		multiplier *= (puzzleSize-i);
		
		if (tmp2 < puzzleSize)
		{
			swap(locs[i], locs[dual[i]]);
			swap(dual[tmp2], dual[i]);
		}
	}
	
	return result2;
}

/**
 * Given the hash returns the state and its dual
 */
void MR1KPermutation::Unrank(uint64_t hash, int *puzzle, int *dual, int distinctSize, int puzzleSize) const
{
//	std::vector<int> &dual = dualCache[threadID];
//	dual.resize(puzzleSize); // vector for distinct item locations
	for (int x = 0; x < puzzleSize; x++)
		dual[x] = x;

	int last = (puzzleSize-distinctSize);
	memset(puzzle, 0xFF, puzzleSize*sizeof(puzzle[0]));

	for (int i = 0; i < distinctSize; i++)
	{
		swap(dual[i+hash%(puzzleSize-i)], dual[i]);
		hash = hash/(puzzleSize-i);
		puzzle[dual[i]] = i;//distinct[i-last-1];
		//printf("Setting %d to be %d\n", dual[i-1], i-last-1);
	}
}