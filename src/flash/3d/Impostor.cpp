#include "Impostor.h"
#include "ImpostorMaterial.h"
#include "flash/Stage.h"

static const GLfloat g_quad_vertex_buffer_data[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f
};
static const GLubyte g_quad_index_buffer_data[] = {
    0, 1, 2,
    2, 3, 0
};
Impostor::Impostor(RenderTarget* renderTarget) {
    sprite3D = new Sprite3D();
    Mesh* mesh = new Mesh();
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_quad_index_buffer_data), g_quad_index_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    mesh->indicesOffset = 0;
    mesh->indicesSize = 6;
    mesh->indicesType = GL_UNSIGNED_BYTE;
    mesh->material = new ImpostorMaterial();
    mesh->material->shader = Stage::getInstance()->impostor_outline_shader;
    mesh->material->setDiffuseTex(&renderTarget->texture);
    sprite3D->scene.meshes.push_back(mesh);
}

void Impostor::render() {
    sprite3D->render(glm::mat4(1.0f));
}
