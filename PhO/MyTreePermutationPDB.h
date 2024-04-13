//
// Created by Benedikt Heuser on 11.04.24.
//

#ifndef STP_PHO_SOLVER_MYTREEPERMUTATIONPDB_H
#define STP_PHO_SOLVER_MYTREEPERMUTATIONPDB_H

#include "hog2/search/TreePermutationPDB.h"

/**
 * This class does the basic permutation calculation in lexicographical order.
 * It uses the tree permutation scheme from Bonet
 * http://aaaipress.org/Papers/Workshops/2008/WS-08-10/WS08-10-004.pdf
 * to do this in n log n time instead of n^2 time.
 */
template <class state, class action, class environment, int bits = 8>
class MyTreePermutationPDB : public TreePermutationPDB<state, action, environment, bits> {
public:
    state GetStateFromAbstractState(state &s) { return s; }

private:

};


#endif //STP_PHO_SOLVER_MYTREEPERMUTATIONPDB_H
