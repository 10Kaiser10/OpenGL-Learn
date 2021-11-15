#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class FPSCamera {
private:
	glm::vec3 camPos;
	glm::vec3 camFront;
	glm::vec3 camUp;
	float camSpeed;
	GLFWwindow* window;
	double mousePosx = 0;
	double mousePosy = 0;
	double yaw = -180.0;
	double pitch = 0;
	float mouseSensitivity;
	bool firstMouse = true;
	float fov = 45.0f;

	void keyboardMovement(float deltaTime);

public:
	FPSCamera(GLFWwindow* win, float speed = 0.05f, float sensitivity = 0.01, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	void Update(float deltaTime);

	glm::mat4 getViewMatrix();

	void mousePosition(double xpos, double ypos);

	void scrollOffset(double scrollVal);

	glm::mat4 getProjectionMatrix();

	glm::vec3 getPosition();
};