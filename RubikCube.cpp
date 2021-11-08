//
// Created by ricar on 31/10/2021.
//

#include "RubikCube.h"
#include "iostream"

int RubikCube::maxShuffle = rand() % 15 + 40;

int RubikCube::windowLoop() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, (const char *) gameName, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        camera.SetCursorPosCallback(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        static double first_time = glfwGetTime();
        double last_time = glfwGetTime();
        elapsedTime = last_time - first_time;

        first_time = last_time;

        viewMat = lookAt(camera.getPos(), camera.getPos() + camera.getFront(), camera.getUp());

        /* DRAW REFERENTIAL*/
        drawReferential();

        displayFaces();

        camera.processInput(window, elapsedTime); // change ... the sort is fine but time is money

        std::cout << solver.isSolved(cubeMapping);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

RubikCube::RubikCube() : GameEngine(1920.0f, 1080.0f, "Rubik Solver"){ // TODO: CHANGE

    std::srand(time(NULL));

    char count = 0;
    for (auto & i : cubeMapping) {
        for (auto & j : i) {
            for (char & k : j) {
                k = count++;
            }
        }
    }

    camera = Camera({ 5.0f,  5.0f, 5.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }, screenWidth, screenHeight);
    // TODO : CHANGE CAMERA UP AND FRONT (see if it makes sense in this case)
    projMat = Matrix_Projection(screenHeight, screenWidth);

    createFaces();

    solver = RubikCubeSolver(cubeMapping);

}

char RubikCube::mapNormalToColor(const vec3d& normal) const{ // TODO: USE THIS THEN WHEN MAKING THE SOLVER ARRAYS

    if(normal.x <= -1.0f)
        return 'R';
    else if(normal.x >= 1.0f)
        return 'O';
    else if(normal.y >= 1.0f)
        return 'G';
    else if(normal.y <= -1.0f)
        return 'B';
    else if(normal.z <= -1.0f)
        return 'W';
    else
        return 'Y';
}

void RubikCube::createFaces() {

    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++) {
            for (int k=0; k < 3; k++) {

                {
                    Face aFace{ triangle{ 0.0f + j, 0.0f + k, 0.0f + i, 1 ,    0.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 1.0f + k, 0.0f + i ,1 },
                                triangle{ 0.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);

                }
                {
                    Face aFace{ triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    1.0f + j, 1.0f + k, 1.0f + i, 1,    0.0f + j, 1.0f + k, 1.0f + i ,1 },
                                triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 1.0f + k, 1.0f + i, 1,    0.0f + j, 0.0f + k, 1.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);

                }

               {
                    Face aFace{ triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 0.0f + k, 1.0f + i, 1,    0.0f + j, 0.0f + k, 0.0f + i ,1 },
                                triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 0.0f + k, 0.0f + i, 1,    1.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                }  {
                    Face aFace{ triangle{ 0.0f + j, 1.0f + k, 0.0f + i, 1 ,    0.0f + j, 1.0f + k, 1.0f + i, 1,    1.0f + j, 1.0f + k, 1.0f + i ,1 },
                                triangle{ 0.0f + j, 1.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 1.0f + i, 1,    1.0f + j, 1.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                }

                {
                    Face aFace{ triangle{ 0.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 1.0f + k, 1.0f + i, 1,    0.0f + j, 1.0f + k, 0.0f + i ,1 },
                                triangle{ 0.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 1.0f + k, 0.0f + i, 1,    0.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                } {
                    Face aFace{ triangle{ 1.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 1.0f + k, 1.0f + i ,1 },
                                triangle{ 1.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 1.0f + i, 1,    1.0f + j, 0.0f + k, 1.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                }
            }
        }
    }

}

vec4d RubikCube::getColor(char color) {
    switch (color) {
        case 'W':
            return vec4d{1.0f,1.0f,1.0f,1.0f};
        case 'R':
            return vec4d{1.0f,0.0f,0.0f,1.0f};
        case 'O':
            return vec4d{1.0f,0.647f,0.0f,1.0f};
        case 'Y':
            return vec4d{1.0f,1.0f,0.0f,1.0f};
        case 'B':
            return vec4d{0.0f,0.0f,1.0f,1.0f};
        case 'G':
            return vec4d{0.0f,1.0f,0.0f,1.0f};
        default:
            return vec4d{1.0f,1.0f,1.0f,1.0f};
    }
    // TODO: throw exception
}

void RubikCube::drawTriangle(const triangle &tri) {

    // draw triangle
    glBegin(GL_TRIANGLES);
    glColor4f(tri.col.R, tri.col.G, tri.col.B, tri.col.A);
    glVertex2f(tri.p[0].x, tri.p[0].y); // the triangle will have to have the information of TexCoords
    glVertex2f(tri.p[1].x, tri.p[1].y); // every cube will have a texture passed to the function
    glVertex2f(tri.p[2].x, tri.p[2].y);
    glEnd();
    glFlush();

    //drawing lines
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glVertex2f(tri.p[0].x, tri.p[0].y); glVertex2f(tri.p[1].x, tri.p[1].y);
    glVertex2f(tri.p[1].x, tri.p[1].y); glVertex2f(tri.p[2].x, tri.p[2].y);
    glVertex2f(tri.p[2].x, tri.p[2].y); glVertex2f(tri.p[0].x, tri.p[0].y);
    glEnd();
    glLineWidth(1.0);
}

void RubikCube::displayFaces() {
    triangle viewedTri, projTri;

    Plane nearPlane(vec3d{ 0.0f, 0.0f, 0.1f }, vec3d{ 0.0f,0.0f,1.0f });

    std::vector<triangle> toRender;

    fTheta += 1.0f * elapsedTime; // TODO: MAGIC NUMBER

    // shuffleCube();
    if(!inMovement)
        makeMove(solver.getNextMove());
    // makemove(getSolverMove ...)

    for(auto &face:vFaces) {
        for( auto & tri: face.tri) {


            // TODO: MAKE IT CLEANER
            if(axisInMovement == (char) 2 ) {
                makeMoveX(tri,face.id);
            } else if (axisInMovement == (char) 1) {
                makeMoveY(tri,face.id);
            }else if(axisInMovement == (char) 0 ) {
                makeMoveZ(tri,face.id);
            }

            viewedTri = tri * viewMat;

            std::vector<triangle> clipTris = nearPlane.ClipTri(viewedTri);

            for(auto & clipTri : clipTris) {

                projTri = clipTri * projMat;

                vec3d normal = getNormal(projTri);
                normalize(normal);

                if (dot(normal, projTri.p[0] - camera.getFront()) < 0.0f)
                {
                    projTri.col = face.color;
                    toRender.push_back(projTri);
                }
            }
        }
    }

    // updating the cubeMapping --- A silent bug if the cube does not move

    /*
    if(axisInMovement == (char) 2 && !inMovement) {
        changeXDimension();
    } else if (axisInMovement == (char) 1 && !inMovement) {
        changeYDimension();
    }else if(axisInMovement == (char) 0   && !inMovement ) {
        changeZDimension();
    }
    */

    sort(toRender.rbegin(), toRender.rend(), [](triangle& t1, triangle& t2)
    {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });

    for (auto it = toRender.rbegin(); it != toRender.rend(); it++) {
        std::list<triangle> listTriangles;
        listTriangles.push_back(*it);

        static Plane planeTop({ 0.0,1.0,0.0 }, { 0.0f,-1.0f,0.0f });
        static Plane planeBot({ 0.0f,-1.0f,0.0f }, { 0.0f,1.0f,0.0f });
        static Plane planeLeft({ -1.0f, 0.0f,0.0f }, { 1.0f,0.0f,0.0f });
        static Plane planeRight({ 1.0f, 0.0f,0.0f }, { -1.0f,0.0f,0.0f });
        static std::vector<Plane*> planesToClip{ &planeTop, &planeBot, &planeLeft, &planeRight };

        size_t nTri = 1;
        for (auto& i : planesToClip) {
            while (nTri > 0)
            {
                triangle next = listTriangles.front();
                listTriangles.pop_front();
                nTri--;

                std::vector<triangle> newTris = i->ClipTri(next);

                for (auto& j : newTris) {
                    listTriangles.push_back(j);
                }
            }
            nTri = listTriangles.size();
        }

        for (auto& t : listTriangles) {
            drawTriangle(t);
        }
    }
}

void RubikCube::drawReferential() const { // TODO: CHANGE -- maybe put on game engine
    static vec3d Ox{5.0f, 0.0f, 0.0f}, Oy{0.0f, 5.0f, 0.0f}, Oz{0.0f,0.0f,5.0f}, origin{};

    vec3d viewOx,viewOy, viewOz, viewOO;
    vec3d projOx,projOy, projOz, projOO;

    MultiplyMatrixVector(Ox, viewOx, viewMat);
    MultiplyMatrixVector(Oy, viewOy, viewMat);
    MultiplyMatrixVector(Oz, viewOz, viewMat);
    MultiplyMatrixVector(origin, viewOO, viewMat);

    MultiplyMatrixVector(viewOx, projOx, projMat);
    MultiplyMatrixVector(viewOy, projOy, projMat);
    MultiplyMatrixVector(viewOz, projOz, projMat);
    MultiplyMatrixVector(viewOO, projOO, projMat);


    //drawing lines
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex2f(projOO.x,projOO.y); glVertex2f(projOx.x, projOx.y);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glVertex2f(projOO.x,projOO.y); glVertex2f(projOy.x, projOy.y);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex2f(projOO.x,projOO.y); glVertex2f(projOz.x, projOz.y);
    glEnd();
    glLineWidth(10.0);

} // TODO: DO THE CLIPPING

void RubikCube::shuffleCube() {

    if ( !RubikCube::maxShuffle) {
        axisInMovement = STOP_SHUFFLE; // STOP
        return;
    }
    if (!inMovement) {
        dimBeingMoved = (char) (std::rand() % 3);
        axisInMovement = (char) (std::rand() % 3);
        inMovement = true;
    }
}

void RubikCube::makeMoveX(triangle& tri, char id) { // TODO: LET MAKE A MOVE THAT'S MORE THAN 90 DEGREES
    if(getXdimension(id) == dimBeingMoved && inMovement) {
        if (fTheta <= M_PI / 2.0f) {// TODO: MAGIC NUMBER
            // NOTE: AS THE CUBE IS NOT CENTERED IN THE AXIS
            tri = tri * Translate(0.0f, -1.5f, -1.5f);
            tri = tri * Matrix_MakeRotationX((float) elapsedTime);
            tri = tri * Translate(0.0f, 1.5f, 1.5f);

            // WE NEED A NEW MATRIX TO ROTATE
            /*Explanation:
             * the axis of the rotation is parallel to the x axis
             * in that way we bring the axis of the cube to the x axis
             * after the rotation is done we bring the axis back
             */
        } // still a slight error in the final angle compared to the 90 degrees
        else {
            fTheta = 0;
            inMovement = false; // here is the bug
            RubikCube::maxShuffle--; // this should not be here...
            tri.p[0].x = std::round(tri.p[0].x);
            tri.p[0].y = std::round(tri.p[0].y);
            tri.p[0].z = std::round(tri.p[0].z);

            tri.p[1].x = std::round(tri.p[1].x);
            tri.p[1].y = std::round(tri.p[1].y);
            tri.p[1].z = std::round(tri.p[1].z);

            tri.p[2].x = std::round(tri.p[2].x);
            tri.p[2].y = std::round(tri.p[2].y);
            tri.p[2].z = std::round(tri.p[2].z); // It works but is not quite well --- the bug above
            changeXDimension();
        }
    }
// TODO: ADD DEBUG
}// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle

void RubikCube::makeMoveY(triangle& tri, char id) {
    if(getYdimension(id) == dimBeingMoved && inMovement) {
        if (fTheta <= M_PI / 2.0f) {// TODO: MAGIC NUMBER
            tri = tri * Translate(-1.5f, 0.0f, -1.5f);
            tri = tri * Matrix_MakeRotationY((float) elapsedTime);
            tri = tri * Translate(1.5f, 0.0f, 1.5f);
        } // still a slight error in the final angle compared to the 90 degrees
        else {
            fTheta = 0;
            inMovement = false;
            RubikCube::maxShuffle--;
            tri.p[0].x = std::round(tri.p[0].x);
            tri.p[0].y = std::round(tri.p[0].y);
            tri.p[0].z = std::round(tri.p[0].z);

            tri.p[1].x = std::round(tri.p[1].x);
            tri.p[1].y = std::round(tri.p[1].y);
            tri.p[1].z = std::round(tri.p[1].z);

            tri.p[2].x = std::round(tri.p[2].x);
            tri.p[2].y = std::round(tri.p[2].y);
            tri.p[2].z = std::round(tri.p[2].z);
            changeYDimension();

        }
    }
}

void RubikCube::makeMoveZ(triangle& tri, char id) {
    if(getZdimension(id) == dimBeingMoved && inMovement) {
        if (fTheta <= M_PI / 2.0f) {// TODO: MAGIC NUMBER
            tri = tri * Translate(-1.5f, -1.5f, 0.0f);
            tri = tri * Matrix_MakeRotationZ((float) elapsedTime);
            tri = tri * Translate(1.5f, 1.5f, 0.0f);
        } // still a slight error in the final angle compared to the 90 degrees
        else {
            fTheta = 0.0f;
            inMovement = false;
            RubikCube::maxShuffle--;
            tri.p[0].x = std::round(tri.p[0].x);
            tri.p[0].y = std::round(tri.p[0].y);
            tri.p[0].z = std::round(tri.p[0].z);

            tri.p[1].x = std::round(tri.p[1].x);
            tri.p[1].y = std::round(tri.p[1].y);
            tri.p[1].z = std::round(tri.p[1].z);

            tri.p[2].x = std::round(tri.p[2].x);
            tri.p[2].y = std::round(tri.p[2].y);
            tri.p[2].z = std::round(tri.p[2].z);
            changeZDimension();

        }
    }
}

char RubikCube::getXdimension(char id) { // TODO: Try to get rid of this annoying getDimensions
    for (auto & i : cubeMapping) {
        for (int j=0; j < 3; j++) {
            for (int k=0; k < 3; k++) {
                if ( i[j][k] == (int) id)
                    return (char)j;
            }
        }
    }
   return -1;
}

char RubikCube::getYdimension(char id) {
    for (auto & i : cubeMapping) {
        for (auto & j : i) {
            for (int k=0; k < 3; k++) {
                if ( j[k] == (int) id)
                    return (char)k;
            }
        }
    }
    return -1;
}

char RubikCube::getZdimension(char id) {
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++) {
            for (int k=0; k < 3; k++) {
                if ( cubeMapping[i][j][k] == (int) id)
                    return (char)i;
            }
        }
    }
    return -1;
}

void RubikCube::changeYDimension() {

    static int dX[3][3] = {{0, 1, 2 }, { -1 , 0,  1}, { -2 , -1,   0 }};
    static int dZ[3][3] = {{2, 1, 0 }, { 1 ,  0, -1}, { 0 ,  -1,  -2 }};

    // the axis corresponds to the parallel in the cubeMapping

    char prevArray[3][3][3];

    //copying
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                prevArray[i][j][k] = cubeMapping[i][j][k];
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cubeMapping[(int)i + dX[i][j]][(int)j + dZ[i][j]][(int)dimBeingMoved] = prevArray[i][j][(int)dimBeingMoved];
        }
    }
}

void RubikCube::changeZDimension() {
    static int dX[3][3] = {{2,1,0}, {1, 0, -1}, {0, -1, -2}};
    static int dY[3][3] = {{0,-1,-2}, {1, 0, -1}, {2, 1, 0}};

    // the axis corresponds to the parallel in the cubeMapping

    char prevArray[3][3][3];

    //copying
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                prevArray[i][j][k] = cubeMapping[i][j][k];
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cubeMapping[(int)dimBeingMoved][(int)(i) + dX[i][j]][(int)j + dY[i][j]] = prevArray[(int)dimBeingMoved][i][j];
        }
    }
}

void RubikCube::changeXDimension() {
    static int dX[3][3] = {{0, 1, 2 }, {-1, 0, 1}, {-2, -1, 0}};
    static int dY[3][3] = {{2, 1, 0 }, {1, 0, -1}, { 0, -1, -2}};

    // the axis corresponds to the parallel in the cubeMapping

    char prevArray[3][3][3];

    //copying
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                prevArray[i][j][k] = cubeMapping[i][j][k];
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cubeMapping[(int)i + dX[i][j]][(int)dimBeingMoved][(int)j + dY[i][j]] = prevArray[i][(int)dimBeingMoved][j];
        }
    }
}

void RubikCube::makeMove(Movement move) { // the moves notation will be interpreted in the camera direction 0 (initial pos)
    if(move == F) {
        dimBeingMoved =  2;
        axisInMovement = 0;
        inMovement = true;
    }
    else if(move == Bc) {
        dimBeingMoved =  0;
        axisInMovement = 0;
        inMovement = true;
    }
    else if(move == Rc) {
        dimBeingMoved =  0;
        axisInMovement = 2;
        inMovement = true;
    }
    else if(move == L) {
        dimBeingMoved =  2;
        axisInMovement = 2;
        inMovement = true;
    }
    else if(move == Uc) {
        dimBeingMoved =  2;
        axisInMovement = 1;
        inMovement = true;
    }
    else if(move == D) {
        dimBeingMoved =  0;
        axisInMovement = 1;
        inMovement = true;
    } else {
        // not valid Move

    }

    // switch ...

    /*
    switch (move) {
        case F:
            break;
        case Bc:
            break;
        case Rc:
            break;
        case L:
            break;
        case Uc:
            break;
        case D:
            break;
        default:
            break;
    }*/

}
