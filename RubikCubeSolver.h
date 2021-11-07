//
// Created by ricar on 02/11/2021.
//

#ifndef RUBIK_CUBE_SOLVER_RUBIKCUBESOLVER_H
#define RUBIK_CUBE_SOLVER_RUBIKCUBESOLVER_H

#include <vector>
#include "array"

enum Movement {
    U,
    D,
    L,
    R,
    F,
    B
};

class RubikCubeSolver {
public:
    explicit RubikCubeSolver(char cubeInitialMapping[3][3][3]); // the intial state


    bool isSolved(char cubeMapping[3][3][3]);

private:

    void getFaces(char cubeMapping[3][3][3]);

    std::array<std::array<char, 3>, 3> redFace;
    std::array<std::array<char, 3>, 3> orangeFace;
    std::array<std::array<char, 3>, 3> blueFace;
    std::array<std::array<char, 3>, 3> whiteFace;
    std::array<std::array<char, 3>, 3> yellowFace;
    std::array<std::array<char, 3>, 3> greenFace;
    std::array<std::array<char, 3>, 3> rRedFace;
    std::array<std::array<char, 3>, 3> rOrangeFace;
    std::array<std::array<char, 3>, 3> rBlueFace;
    std::array<std::array<char, 3>, 3> rWhiteFace;
    std::array<std::array<char, 3>, 3> rYellowFace;
    std::array<std::array<char, 3>, 3> rGreenFace;
};

#endif //RUBIK_CUBE_SOLVER_RUBIKCUBESOLVER_H
