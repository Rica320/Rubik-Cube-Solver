#pragma once

#include "GLFW/glfw3.h"

#include "Mat.h"

#include <future>

class Camera
{
private:
	vec3d cameraPos;
	vec3d cameraFront;
	vec3d cameraUp;
	float lastX, lastY;
	float yaw;
	float pitch;
	bool firstMouse;

	void checkPlayerJump(float cameraSpeed, bool Jump, float nearCubey); // TODO: these two should not be here -> in a player class
	void checkPlayerGravity(double cameraSpeed, float nearCubey);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void CallbackStatic(GLFWwindow* window,
		double xpos,
		double ypos)
	{
		Camera* that = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		that->mouse_callback(window, xpos, ypos);
	}
public:
	Camera();
	Camera(vec3d cameraPos, vec3d cameraFront, vec3d cameraUp, float Screenwidth, float Screenheight);
	void processInput(GLFWwindow* window, double elapsed_time, const float& nearCubey); // TODO: THIS IS NOT GOOD
    void processInput(GLFWwindow* window, double elapsed_time);
    void SetCursorPosCallback(GLFWwindow* window) {
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, CallbackStatic);
	}
	vec3d getPos() const { return cameraPos; }
	vec3d getFront()const { return cameraFront; }
	vec3d getUp() const { return cameraUp; }


};

