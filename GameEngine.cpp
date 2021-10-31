//
// Created by ricar on 31/10/2021.
//

#include "GameEngine.h"

void GameEngine::drawTriangle(const triangle &tri) {
    glBegin(GL_TRIANGLES);
    // glColor4f(tri.col.R, tri.col.G, tri.col.B, tri.col.A);
    glVertex2f(tri.p[0].x, tri.p[0].y); // the triangle will have to have the information of TexCoords
    glVertex2f(tri.p[1].x, tri.p[1].y); // every cube will have a texture passed to the function
    glVertex2f(tri.p[2].x, tri.p[2].y);
    glEnd();
    glFlush();

}

int GameEngine::windowLoop() {
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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glBegin(GL_TRIANGLES);
        glColor3f(0.1, 0.2, 0.3);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(0.0f, 1.0f);
        glEnd();
        glFlush();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

