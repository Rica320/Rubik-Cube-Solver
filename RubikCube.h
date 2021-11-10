//
// Created by ricar on 31/10/2021.
//

#ifndef RUBIK_CUBE_SOLVER_RUBIKCUBE_H
#define RUBIK_CUBE_SOLVER_RUBIKCUBE_H

#define _USE_MATH_DEFINES
#define STOP_SHUFFLE -1

#include "GameEngine.h"
#include "Mat.h"
#include "RubikCubeSolver.h"

#include <list>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iterator>


struct Face {
    triangle tri[2];
    vec4d color{};
    char id{-1}; // the id that identifies the Cube in which the face is
}; // probably shouldn't be here

class RubikCube : public GameEngine{
public:
    RubikCube(); // initial position will be 0, the center of the universe
    int windowLoop() override;
    void drawTriangle(const triangle& tri) override;
    void displayFaces();
    void drawReferential() const override;
    void shuffleCube();
    void makeMoveX(triangle& tri, char id);
    void makeMoveY(triangle& tri, char id);
    void makeMoveZ(triangle& tri, char id);
    void makeMove(Movement move);
    char getXdimension(char id);
    char getYdimension(char id);
    char getZdimension(char id);
    void changeXDimension();
    void changeYDimension();
    void changeZDimension();

private:
    char cubeMapping[3][3][3]{}; // TODO: MAKE IT CLEAN
    char mapNormalToColor(const vec3d& normal) const;
    void createFaces();
    vec4d getColor(char color);

    std::vector<Face> vFaces;
    RubikCubeSolver solver;

    bool inMovement = false;
    char dimBeingMoved = NULL;
    char axisInMovement = NULL;

    static int maxShuffle;

};


#endif //RUBIK_CUBE_SOLVER_RUBIKCUBE_H
