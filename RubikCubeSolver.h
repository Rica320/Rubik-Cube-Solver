//
// Created by ricar on 02/11/2021.
//

#ifndef RUBIK_CUBE_SOLVER_RUBIKCUBESOLVER_H
#define RUBIK_CUBE_SOLVER_RUBIKCUBESOLVER_H

#include <vector>
#include "RubikCube.h"

class RubikCubeSolver {
public:
    bool isSolved(char cubeMapping[3][3][3], std::vector<Face> vFaces) const;

private:
    char redFace[3][3];
    char orangeFace[3][3];
    char blueFace[3][3];
    char whiteFace[3][3];
    char yellowFace[3][3];
    char greenFace[3][3];

};


#endif //RUBIK_CUBE_SOLVER_RUBIKCUBESOLVER_H
