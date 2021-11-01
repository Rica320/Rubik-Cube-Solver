//
// Created by ricar on 31/10/2021.
//

#ifndef RUBIK_CUBE_SOLVER_RUBIKCUBE_H
#define RUBIK_CUBE_SOLVER_RUBIKCUBE_H

#include "GameEngine.h"
#include "Mat.h"
#include <list>

#include <vector>

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
    void drawReferential();
private:
    char cubeMapping[3][3][3]{}; // TODO: MAKE IT CLEAN
    char mapNormalToColor(const vec3d& normal) const;
    void createFaces();
    vec4d getColor(char color);

    std::vector<Face> vFaces;
};


#endif //RUBIK_CUBE_SOLVER_RUBIKCUBE_H
