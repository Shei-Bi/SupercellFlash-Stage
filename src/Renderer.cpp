#include <Renderer.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

int Renderer::left = 0;
int Renderer::top = 0;
int Renderer::right = 800;
int Renderer::bottom = 600;
void Renderer::setViewport(int l, int t, int r, int b) {
    left = l;
    top = t;
    right = r;
    bottom = b;
    glViewport(l, t, r, b);
}
glm::mat4 Renderer::getPixelMatrix() {
    return glm::ortho((float)left, (float)right, (float)bottom, (float)top, -1.0f, 1.0f);
}
