//
// Created by ricar on 02/11/2021.
//

#include "RubikCubeSolver.h"

bool RubikCubeSolver::isSolved(char cubeMapping[3][3][3]){
    getFaces(cubeMapping);

    if (redFace == rRedFace && blueFace == rBlueFace && greenFace == rGreenFace && orangeFace == rOrangeFace
        && whiteFace == rWhiteFace && yellowFace == rYellowFace)
        return true;
    return false;
}

RubikCubeSolver::RubikCubeSolver(char (*cubeInitialMapping)[3][3]) {

    // one of the faces -- the white?
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            whiteFace[j][k] = cubeInitialMapping[0][j][k];
        }
    }

    // one of the faces -- the yellow?
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            yellowFace[j][k] = cubeInitialMapping[2][j][k];
        }
    }

    // one of the faces -- the red?
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            redFace[j][k] = cubeInitialMapping[j][0][k];
        }
    }


    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            orangeFace[j][k] = cubeInitialMapping[j][2][k];
        }
    }

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            greenFace[j][k] = cubeInitialMapping[j][k][0];
        }
    }

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            blueFace[j][k] = cubeInitialMapping[j][k][2];
        }
    }

}

void RubikCubeSolver::getFaces(char (*cubeMapping)[3][3]) {

// one of the faces -- the white?
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            rWhiteFace[j][k] = cubeMapping[0][j][k];
        }
    }

    // one of the faces -- the yellow?
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            rYellowFace[j][k] = cubeMapping[2][j][k];
        }
    }

    // one of the faces -- the red?
    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            rRedFace[j][k] = cubeMapping[j][0][k];
        }
    }


    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            rOrangeFace[j][k] = cubeMapping[j][2][k];
        }
    }

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            rGreenFace[j][k] = cubeMapping[j][k][0];
        }
    }

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            rBlueFace[j][k] = cubeMapping[j][k][2];
        }
    }
}

Movement RubikCubeSolver::getNextMove() {
    if(movesToSolve.empty())
        return StayTheSame;
    Movement m = movesToSolve.front();
    movesToSolve.pop_front();
    return m;
}

