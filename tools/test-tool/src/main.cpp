#include <flash/flash.h>
#include <core/time/timer.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <flash/Stage.h>
#include <flash/Shape.h>
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
#include<core/exception/exception.h>

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace sc::flash;

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
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
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
	char* filename = "sc/ui_new.sc";
	ResourceManager::addFile(filename);

	Stage::getInstance()->init(0, 0, 800, 600);
	MovieClip* e = ResourceManager::getMovieClip(filename, "ingame_hud_top");
	// MovieClip* f = ResourceManager::getMovieClip(filename, "emoji_colette_trixie_happy");
	MovieClip* f = ResourceManager::getMovieClip(filename, "emoji_colette_trixie_happy");
	MovieClip* g = ResourceManager::getMovieClip(filename, "text_buble_small");
	// Stage::getInstance()->addChild(e);
	Stage::getInstance()->addChild(f);
	// Stage::getInstance()->addChild(g);
	f->setXY(100.0, 200.0);
	g->setXY(300.0, 300.0);

	float xxxx = 400.0;
	float yyyy = 300.0;
	float ssss = 1.0;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	unsigned long long programPrevTime = xTimer::getNativeTime();
	GameMain* GameMain = GameMain::getInstance();
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			yyyy -= 20.0;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			yyyy += 20.0;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			xxxx -= 20.0;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			xxxx += 20.0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			ssss *= 1.1;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			ssss /= 1.1;
		f->setXY(xxxx, yyyy);
		f->setScale(ssss);

		glActiveTexture(GL_TEXTURE0);

		unsigned long long now = xTimer::getNativeTime();
		float dT = fmaxf(0.0, xTimer::getPassedTimeMs(programPrevTime, now) * 0.001f);
		float dTSS = fmaxf(0.0, xTimer::getPassedTimeMs(programStartTime, now) * 0.001f);
		programPrevTime = now;
		GameMain->update(dTSS, dT);
		GameMain->draw(1.0f);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}