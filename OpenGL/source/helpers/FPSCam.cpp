#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <FPSCam.h>


void FPSCamera::keyboardMovement(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camPos += deltaTime * camSpeed * camFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camPos -= deltaTime * camSpeed * camFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camPos += deltaTime * camSpeed * glm::normalize(glm::cross(camUp, camFront));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camPos -= deltaTime * camSpeed * glm::normalize(glm::cross(camUp, camFront));
	}
}

FPSCamera::FPSCamera(GLFWwindow* win, float speed, float sensitivity, glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
	camPos = pos;
	camFront = front;
	camUp = up;
	camSpeed = speed;
	window = win;
	mouseSensitivity = sensitivity;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void FPSCamera::Update(float deltaTime)
{
	keyboardMovement(deltaTime);
}

glm::mat4 FPSCamera::getViewMatrix()
{
	return glm::lookAt(camPos, camPos + camFront, camUp);
}

void FPSCamera::mousePosition(double xpos, double ypos)
{
	if (firstMouse)
	{
		firstMouse = false;
		mousePosx = xpos;
		mousePosy = ypos;
	}

	double changeX = mousePosx - xpos;
	double changeY = mousePosy - ypos;
	mousePosx = xpos;
	mousePosy = ypos;

	pitch += mouseSensitivity * changeY;
	yaw += mouseSensitivity * changeX;

	if (pitch > 89)pitch = 89;
	if (pitch < -89)pitch = -89;

	camFront.x = (float) (cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
	camFront.y = (float) sin(glm::radians(pitch));
	camFront.z = (float) (cos(glm::radians(pitch)) * cos(glm::radians(yaw)));
}

void FPSCamera::scrollOffset(double scrollVal)
{
	fov -= (float)scrollVal;

	if (fov < 1) fov = 1;
	if (fov > 150) fov = 150;
}

glm::mat4 FPSCamera::getProjectionMatrix()
{
	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(fov), 640.0f / 640.0f, 0.1f, 100.0f);
	return proj;
}