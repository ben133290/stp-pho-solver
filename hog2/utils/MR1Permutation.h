//
//  MR1Permutation.h
//  hog2 glut
//
//  Created by Nathan Sturtevant on 6/19/16.
//  Copyright © 2016 University of Denver. All rights reserved.
//

#ifndef MR1Permutation_h
#define MR1Permutation_h

#include <cstdio>
#include <vector>
#include <cstdint>

template <typename T>
inline void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

class MR1KPermutation {
public:
	uint64_t Rank(int *locs, int *dual, int distinctSize, int puzzleSize) const;
	void Unrank(uint64_t hash, int *items, int *dual, int k, int N) const;
	uint64_t Rank(uint8_t *items, uint8_t *dual, int k, int N) const;
};

#endif /* MR1Permutation_h */
