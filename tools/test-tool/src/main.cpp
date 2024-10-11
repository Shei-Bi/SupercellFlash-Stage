#include <flash/flash.h>
#include <core/time/timer.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <flash/Stage.h>
#include <flash/Shape.h>
#include <stdio.h>

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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("C:/Users/EDY/source/repos/SupercellFlash-Stage/build/tools/test-tool/Debug/4.2.texture.vs", "C:/Users/EDY/source/repos/SupercellFlash-Stage/build/tools/test-tool/Debug/4.2.texture.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------


	if (argc <= 1) {
		return 1;
	}

	fs::path filepath = argv[1];
	if (!fs::exists(filepath)) {
		cout << "File not found";
		return 1;
	}

	/* Loading test */

	time_point loading_start = high_resolution_clock::now();
	sc::Timer loading;
	SupercellSWF swf;
	swf.load(filepath);

	cout << "Loading took: ";
	cout << loading.elapsed() << endl << endl;
	loading.reset();

	// Stage::getInstance();
	// swf.textures[0].downscaling

	cout << "Shapes Count:" << swf.shapes.size() << endl
		<< "textures Count:" << swf.textures.size() << endl
		<< "movieclips Count:" << swf.movieclips.size() << endl
		<< "textfields Count:" << swf.textfields.size() << endl;

	// load and create a texture 
	// -------------------------
	// unsigned int texture1;
	// texture 1
	// ---------
	// glGenTextures(1, &texture1);
	// glBindTexture(GL_TEXTURE_2D, texture1);
	// // set the texture wrapping parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// // set texture filtering parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // load image, create texture and generate mipmaps
	// int width, height, nrChannels;
	// stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.


	// for (int j = 0;j < swf.exports.size();j++) {
	// 	printf("%s\n", swf.exports[j].name.data());
	// }
	char* name = "emoji_colette_trixie_happy";
	// std::cin >> name;

	MovieClipOriginal movieClip;
	{
		for (ExportName& export_name : swf.exports) {
			if (export_name.name == name) {
				printf("found\n");
				movieClip = swf.getOriginalMovieClip(export_name.id, nullptr);
				goto find;
			}
		}
		throw new sc::Exception("not found");
	find:;
	}
	movieClip.createTimelineChildren(swf);
	// printf("test!\n");
	// for (int j = 0;j < movieClip.instances.size();j++) {
	// 	printf("test: %d\n", j);
	// 	if (movieClip.displayObjects[j]->is_movieclip());
	// 	// printf("TimelineChildren: %s\n", ((MovieClipOriginal*)movieClip.displayObjects[j])->name->data());
	// }

	Shape* shape = Shape::createShape((sc::flash::ShapeOriginal*)movieClip.displayObjects[1]);
	Stage::constructInstance();
	Stage::getInstance()->addChild(shape);
	// GLImage* i = new GLImage();
	// i->bind();
	// sc::flash::SWFTexture tex = swf.textures[((sc::flash::ShapeOriginal*)movieClip.displayObjects[1])->commands[0].texture_index];
	// tex.linear(!tex.linear());
	// i->createWithFormat(tex);
	// i->unbind();
	// shape->render();
	// return;
	// ShapeDrawBitmapCommand c = swf.shapes[113].commands[0];
	// printf("texture_index: %d\n", c.texture_index);
	// for (int j = 0;j < c.vertices.size();j++) {
	// 	printf("%f ", c.vertices[j].u);
	// 	printf("%f \n", c.vertices[j].v);
	// }
	// sc::RawImage* image = nullptr;
	// sc::InputFileStream image_file("awesomeface.png");
	// sc::stb::load_image(image_file, &image);
	// StraightToPremultiply(*image);

	// unsigned char* data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	// if (data)
	// {
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data());
	// glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// 	else
	// 	{
	// 		std::cout << "Failed to load texture" << std::endl;
	// 	}
	// 	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture1);

		// render container
		ourShader.use();
		// shape->render();
		Stage::getInstance()->render(0.0, true);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	// glDeleteVertexArrays(1, &VAO);
	// glDeleteBuffers(1, &VBO);
	// glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}