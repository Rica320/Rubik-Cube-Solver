//
// Created by ricar on 31/10/2021.
//

#ifndef RUBIK_CUBE_SOLVER_GAMEENGINE_H
#define RUBIK_CUBE_SOLVER_GAMEENGINE_H

#include "Mat.h"
#include "lib/Camera/Camera.h"
#include "RubikCubeSolver.h"

#include "GLFW/glfw3.h"
#include <string>

class GameEngine {
public:
    GameEngine(float screenWidth, float screenHeight,const char * gameName) :screenWidth(screenWidth),
    screenHeight(screenHeight), gameName(gameName) {}

    virtual void drawTriangle(const triangle& tri);
    virtual int windowLoop();
    virtual void drawReferential() const {};


protected:
    const float screenWidth, screenHeight;
    const char *gameName;

    Camera camera;
    mat4x4 projMat;
    mat4x4 viewMat;

    float fTheta = 0.0f;
    double elapsedTime = 0.0;

};


#endif //RUBIK_CUBE_SOLVER_GAMEENGINE_H
