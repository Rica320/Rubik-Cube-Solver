//
// Created by ricar on 31/10/2021.
//

#include "RubikCube.h"

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
        double elapsedTime = last_time - first_time;

        first_time = last_time;

        viewMat = lookAt(camera.getPos(), camera.getPos() + camera.getFront(), camera.getUp());

        /* DRAW REFERENTIAL*/
        drawReferential();

        displayFaces();

        camera.processInput(window, elapsedTime); // change ... the sort is fine but time is money

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

RubikCube::RubikCube() : GameEngine(1920.0f, 1080.0f, "Rubik Solver"){ // TODO: CHANGE

    char count = 0;
    for (auto & i : cubeMapping) {
        for (auto & j : i) {
            for (char & k : j) {
                k = count++;
            }
        }
    }

    camera = Camera({ 0.0f,  8.0f, -5.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f }, screenWidth, screenHeight);

    projMat = Matrix_Projection(screenHeight, screenWidth);

    createFaces();
}

char RubikCube::mapNormalToColor(const vec3d& normal) const{

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

                /*
                if (i == 0) {
                    Face aFace{ triangle{ 0.0f + j, 0.0f + k, 0.0f + i, 1 ,    0.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 1.0f + k, 0.0f + i ,1 },
                                triangle{ 0.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);

                } else if (i == 2) {
                    Face aFace{ triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    1.0f + j, 1.0f + k, 1.0f + i, 1,    0.0f + j, 1.0f + k, 1.0f + i ,1 },
                                triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 1.0f + k, 1.0f + i, 1,    0.0f + j, 0.0f + k, 1.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);

                }

                if ( j == 0 ) {
                    Face aFace{ triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 0.0f + k, 1.0f + i, 1,    0.0f + j, 0.0f + k, 0.0f + i ,1 },
                                triangle{ 1.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 0.0f + k, 0.0f + i, 1,    1.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                } else if( j == 2) {
                    Face aFace{ triangle{ 0.0f + j, 1.0f + k, 0.0f + i, 1 ,    0.0f + j, 1.0f + k, 1.0f + i, 1,    1.0f + j, 1.0f + k, 1.0f + i ,1 },
                                triangle{ 0.0f + j, 1.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 1.0f + i, 1,    1.0f + j, 1.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                }

                if ( k == 0) {
                    Face aFace{ triangle{ 0.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 1.0f + k, 1.0f + i, 1,    0.0f + j, 1.0f + k, 0.0f + i ,1 },
                                triangle{ 0.0f + j, 0.0f + k, 1.0f + i, 1 ,    0.0f + j, 1.0f + k, 0.0f + i, 1,    0.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                }else if ( k == 2) {
                    Face aFace{ triangle{ 1.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 1.0f + k, 1.0f + i ,1 },
                                triangle{ 1.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 1.0f + i, 1,    1.0f + j, 0.0f + k, 1.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);
                }
                 */
                // TODO: SOMETHING IS WRONG ABOVE



               {
                    Face aFace{ triangle{ 0.0f + j, 0.0f + k, 0.0f + i, 1 ,    0.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 1.0f + k, 0.0f + i ,1 },
                                triangle{ 0.0f + j, 0.0f + k, 0.0f + i, 1 ,    1.0f + j, 1.0f + k, 0.0f + i, 1,    1.0f + j, 0.0f + k, 0.0f + i ,1 } };

                    aFace.id = cubeMapping[i][j][k];
                    aFace.color = getColor(mapNormalToColor(getNormal(aFace.tri[0])));

                    vFaces.push_back(aFace);

                }  {
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
    for(auto &face:vFaces) {
        for( auto & tri: face.tri) {
            triangle aTri = tri;

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

void RubikCube::drawReferential() { // TODO: CHANGE -- maybe put on game engine
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



}
