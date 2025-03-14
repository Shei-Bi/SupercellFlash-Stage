// #include <string>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <flash/Stage.h>
// // #include <flash/Shape.h>
// #include <stdio.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <ResourceManager.h>
// #include <GameMain.h>
// #include <xTimer.h>

// // #include <core/stb/stb.h>
// // #include <core/image/raw_image.h>
// #include <Shader.h>
// #include <iostream>
// #include <chrono>
// #include "csv/CSVNode.hpp"

// using namespace std;
// using namespace std::chrono;

// namespace fs = std::filesystem;

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow* window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// // ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
// 	if (Stage::getInstance())  Stage::getInstance()->init(0, 0, 800, 600);
// }
// int main(int argc, char* argv[])
// {
// 	glfwInit();
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// 	// glfwWindowHint(GLFW_SAMPLES, 4);

// #ifdef __APPLE__
// 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif
// 	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
// 	if (window == NULL)
// 	{
// 		std::cout << "Failed to create GLFW window" << std::endl;
// 		glfwTerminate();
// 		return -1;
// 	}
// 	glfwMakeContextCurrent(window);
// 	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// 	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
// 	{
// 		std::cout << "Failed to initialize GLAD" << std::endl;
// 		return -1;
// 	}
// 	// if (glfwExtensionSupported("GL_KHR_texture_compression_astc_ldr")) {
// 	// 	printf("GL_KHR_texture_compression_astc_ldr supported\n");
// 	// }
// 	// else printf("GL_KHR_texture_compression_astc_ldr not supported\n");
// 	// return 0;
// 	// ResourceManager::loadNextResource();
// 	// ResourceManager::addFile("sc/background_toystory.sc");
// 	// ResourceManager::loadNextResource();

// 	// glfwSetWindowOpacity(window, 0.5f);

// 	// glfwMaximizeWindow(window);
// 	int width = 1920;
// 	int height = 1080;

// 	// glfwSetWindowSize(window, width, height);
// 	unsigned int fbo;
// 	glGenFramebuffers(1, &fbo);
// 	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

// 	unsigned int texColorBuffer;
// 	glGenTextures(1, &texColorBuffer);
// 	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glBindTexture(GL_TEXTURE_2D, 0);

// 	//将它附加到当前绑定的帧缓冲对象
// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

// 	unsigned int stencilTex;
// 	glGenTextures(1, &stencilTex);
// 	glBindTexture(GL_TEXTURE_2D, stencilTex);
// 	glTexImage2D(
// 		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
// 		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
// 	);

// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTex, 0);
// 	glBindTexture(GL_TEXTURE_2D, 0);

// 	// glfwGetWindowSize(window, &width, &height);
// 	Stage::constructInstance();
// 	auto Stage = Stage::getInstance();
// 	Stage->init(0, 0, width, height);
// 	Stage->forceNewBucket = true;

// 	auto csvTable = CSVNode::fromFile("assets/csv_logic/themes.csv")->table;
// 	int* buffer = new int[width * height];
// 	for (int i = 0;i < csvTable->getRowCount();i++) {
// 		std::string Name = csvTable->getValue("Name", i);
// 		std::string FileName = csvTable->getValue("FileName", i);
// 		std::string ExportName = csvTable->getValue("ExportName", i);
// 		printf("Starting to bake %s\n", Name.c_str());
// 		if (FileName.size() <= 1 || !fs::exists(fs::path("assets") / FileName)) {
// 			printf("File not found: %s\n", FileName.c_str());
// 			continue;
// 		}
// 		ResourceManager::addFile(FileName.c_str());
// 		ResourceManager::loadNextResource();
// 		if (!ResourceManager::getSupercellSWF(FileName.c_str(), "")->hasExportName((char*)ExportName.c_str())) {
// 			printf("Export not found: %s\n", ExportName.c_str());
// 			continue;
// 		}
// 		auto background = ResourceManager::getMovieClip(FileName, ExportName);
// 		background->setChildVisible("button_cctv", false);
// 		// printf("%d\n", ResourceManager::getSupercellSWF("sc/background_vp.sc", "bgr_vp")->textures[1].pixel_format());
// 		Stage->addChild(background);

// 		MovieClip* bg_colour = background->getMovieClipByName("bg_colour");
// 		// bg_colour->moveThisToTopLayer();
// 		// bg_colour->setY(-300.0f);
// 		// bg_colour->setAlpha(0.0f);
// 		if (!bg_colour) bg_colour = background;
// 		float scaleX = (Stage->matrixX + 4) / bg_colour->getWidth();
// 		float scaleY = (Stage->matrixY + 4) / bg_colour->getHeight();
// 		if (scaleX > 1.0 || scaleY > 1.0) bg_colour->setScale(fmax(scaleX, scaleY));
// 		background->setXY(Stage->matrixX / 2, Stage->matrixY / 2);


// 		char cmd[255];
// 		sprintf(cmd, "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s %dx%d -i - "
// 			"-threads 0 -preset medium -y -pix_fmt yuv420p -crf 18 -vf vflip output/%s.mp4", width, height, Name.c_str());
// 		FILE* ffmpeg = _popen(cmd, "wb");
// 		glActiveTexture(GL_TEXTURE0);
// 		for (int i = 0;i < 60 * 240;i++) {
// 			Stage->render(1 / 60.0f, true);
// 			// glfwSwapBuffers(window);
// 			// glfwPollEvents();
// 			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
// 			fwrite(buffer, sizeof(int), width * height, ffmpeg);
// 		}
// 		_pclose(ffmpeg);
// 		delete background;
// 		printf("Finished baking %s\n", Name.c_str());
// 	}
// 	glfwTerminate();
// 	return 0;
// }