//
// Created by heuser0000 on 6/15/24.
//

#include "../hog2/environments/STPInstances.h"

int main(int argc, char *argv[]) {
    std::cout << "{";
    for (int i = 0; i < 100; i++) {
        std::cout << "{";
        MNPuzzleState<4,4> state = STP::GetRandomInstance(20);
        for(int j = 0; j < 15; j++) {
            std::cout << state.puzzle[j] << ",";
        }
        std::cout << state.puzzle[15];
        std::cout << "},";
    }
    std::cout << "}";

}
