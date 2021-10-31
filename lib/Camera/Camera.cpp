#include "Camera.h"


Camera::Camera() {
	cameraPos = {    0.0f,  0.0f, 0.0f };
	cameraFront = {  0.0f,  0.0f,  0.0f };
	cameraUp = {     0.0f,  0.0f,  0.0f };

	lastX = 0; lastY = 0;
	yaw = -90.0f;
	pitch = 0.0f;
	firstMouse = true;
}

Camera::Camera(vec3d cameraPos, vec3d cameraFront, vec3d cameraUp, float Screenwidth, float Screenheight)
    :cameraPos(cameraPos), cameraUp(cameraUp), cameraFront(cameraFront)
{
	lastX = Screenwidth / 2, lastY = Screenheight / 2;
	yaw = -90.0f;
	pitch = 0;
	firstMouse = true;
}

void Camera::processInput(GLFWwindow* window, double elapsed_time, const float& nearCubey)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool toJump = false;
    // static Plane pWSPosibleMoves{ cameraPos, vec3d{0.0f,1.0f,0.0f } }; // again... a class will be helpful
    // static vec3d vMovement = pWSPosibleMoves.projectVectorToPlane(cameraFront); -> less computaion will be helpful
    vec3d vMovement = { cameraFront.x, 0.0f, cameraFront.z }; // on the light of the above
    vec3d a = cross(cameraFront, cameraUp);
    normalize(a);
    const float cameraSpeed = 2.5f * elapsed_time; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos -= (cameraSpeed * vMovement);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += (cameraSpeed * vMovement);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= (a * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += (a * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // Space was pressed or obj is already in a jump
        toJump = true;
        cameraPos.y += cameraSpeed;
    }
   // std::async(std::launch::async, &Camera::checkPlayerJump, this, cameraSpeed, toJump, nearCubey); // a lite fedback is given
   // std::async(std::launch::async, &Camera::checkPlayerGravity, this, cameraSpeed, nearCubey);
}

void Camera::checkPlayerJump(float cameraSpeed, bool Jump, float nearCubey) // this function has problems
{
    static float cY = cameraPos.y;
    static bool InJump = true;
    static bool InFall = false;

    if (cameraPos.y <= nearCubey + 2.0f) {
        InFall = false;
    }
    if (!InJump && Jump && !InFall) {
        cY = cameraPos.y;
        InJump = true;
    }
    else if (InJump && !Jump)
    {
        const static float jumpYs = 1.5;

        if (cameraPos.y < cY + jumpYs) {
            cameraPos.y += cameraSpeed * 2;
        }
        if (cameraPos.y >= cY + jumpYs)
        {
            InJump = false; // this will not work as wished
            InFall = true; //begins to fall
        }
    }
}

void Camera::checkPlayerGravity(double cameraSpeed, float nearCubey)
{
    if (cameraPos.y > nearCubey + 2.0f)
        //  if (cameraPos.y > 0.0f)

    {
        cameraPos.y -= cameraSpeed / 1.5; // TODO: better phisics
    }
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // TODO: CHANGE TO A MORE "PERSON A-LIKE" GAME
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3d direction;
    direction.x = cos(yaw * 3.14159265359f / 180.0f) * cos(pitch * 3.14159265359f / 180.0f);
    direction.y = sin(pitch * 3.14159265359f / 180.0f);
    direction.z = sin(yaw * 3.14159265359f / 180.0f) * cos(pitch * 3.14159265359f / 180.0f);
    normalize(direction);

    cameraFront = direction;
    normalize(cameraFront);
}

void Camera::processInput(GLFWwindow *window, double elapsed_time) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool toJump = false;

    vec3d vMovement = { cameraFront.x, 0.0f, cameraFront.z };
    vec3d a = cross(cameraFront, cameraUp);
    normalize(a);
    const float cameraSpeed = 2.5f * elapsed_time; // adjust accordingly

    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos -= (cameraSpeed * vMovement);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += (cameraSpeed * vMovement);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= (a * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += (a * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // Space was pressed or obj is already in a jump
        toJump = true;
        cameraPos.y += cameraSpeed; // TODO: CAMERA SHOULD BE A MORE GENERIC CLASS, using virtual, THEN WE COULD IMPLEMENT DIFFERENT INPUT PROCEDURES
    }
    */

    vec3d cProduct = cross(cameraFront, cameraUp);
    normalize(cProduct);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cProduct * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cProduct * cameraSpeed;
}
