
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <flash/Stage.h>
// #include <flash/Shape.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ResourceManager.h>
#include <GameMain.h>
#include <xTimer.h>

// #include <core/stb/stb.h>
// #include <core/image/raw_image.h>
#include <learnopengl_s.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace fs = std::filesystem;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (Stage::getInstance())  Stage::getInstance()->init(0, 0, 800, 600);
}
int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	unsigned long long programStartTime = xTimer::getNativeTime();
	GameMain::constructInstance();
	GameMain::getInstance()->init();
	// sc::flash::SupercellSWF swf;
	// swf.load_internal("C:/Users/EDY/Pictures/sc background_basic.sc", false);
	// ResourceManager::addFile("sc/ui.sc");
	ResourceManager::addFile("sc/loading.sc");
	ResourceManager::loadNextResource();
	ResourceManager::addFile("sc/background_toystory.sc");
	ResourceManager::loadNextResource();

	glfwMaximizeWindow(window);
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	Stage::getInstance()->init(0, 0, width, height);
	unsigned long long programPrevTime = xTimer::getNativeTime();
	GameMain* GameMain = GameMain::getInstance();
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		glActiveTexture(GL_TEXTURE0);

		unsigned long long now = xTimer::getNativeTime();
		float dT = fmaxf(0.0, xTimer::getPassedTimeMs(programPrevTime, now) * 0.001f);
		float dTSS = fmaxf(0.0, xTimer::getPassedTimeMs(programStartTime, now) * 0.001f);
		programPrevTime = now;
		GameMain->update(dTSS, dT);
		GameMain->draw(1.0f);
		printf("fps: %f\n", 1.0f / dT);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}