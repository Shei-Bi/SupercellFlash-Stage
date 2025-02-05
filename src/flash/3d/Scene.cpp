#include "Scene.h"
#include "glad/glad.h"
#include "glTF_generated.h"

void processNode(const sc::model::glTF::glTF* glb, const sc::model::glTF::Node* node) {
    if (node->mesh() != -1) {
        if (node->matrix() != nullptr) {
            for (int i = 0;i < 16;i++) {
                printf("%f ", node->matrix()->Get(i));
            }
            printf("\n");
        }
        else {
            if (node->scale())
                printf("scale: %f %f %f\n", node->scale()->Get(0), node->scale()->Get(1), node->scale()->Get(2));
            if (node->rotation())
                printf("rotation: %f %f %f %f\n", node->rotation()->Get(0), node->rotation()->Get(1), node->rotation()->Get(2), node->rotation()->Get(3));
            if (node->translation())
                printf("translation: %f %f %f\n", node->translation()->Get(0), node->translation()->Get(1), node->translation()->Get(2));
        }
        printf("mesh %d\n", node->mesh());
    }
    if (node->children() != nullptr) {
        for (int i = 0;i < node->children()->size();i++) {
            processNode(glb, glb->nodes()->Get(node->children()->Get(i)));
        }
    }
}
void Scene::createFromSCWFile(SCW::File* scwFile) {
    if (scwFile->Geometries.size() > 0) {
        for (SCW::Geometry* geometry : scwFile->Geometries) {
            if (geometry->Materials[0].InputsCount != 3) continue;
            Mesh* mesh = new Mesh();
            // geometry->Materials

            int pointCount = geometry->Materials[0].TrianglesCount * 3;
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
            vertices.reserve(pointCount * 8);
            indices.reserve(pointCount);
            for (int i = 0;i < pointCount;i++) {
                int verticeIndex = geometry->Materials[0].IndexBuffer[i * 3 + 0];
                int normalIndex = geometry->Materials[0].IndexBuffer[i * 3 + 1];
                int texIndex = geometry->Materials[0].IndexBuffer[i * 3 + 2];
                vertices.push_back(geometry->Vertices[0].Data[verticeIndex * 3 + 0]);
                vertices.push_back(geometry->Vertices[0].Data[verticeIndex * 3 + 1]);
                vertices.push_back(geometry->Vertices[0].Data[verticeIndex * 3 + 2]);
                vertices.push_back(geometry->Vertices[1].Data[normalIndex * 3 + 0]);
                vertices.push_back(geometry->Vertices[1].Data[normalIndex * 3 + 1]);
                vertices.push_back(geometry->Vertices[1].Data[normalIndex * 3 + 2]);
                vertices.push_back(geometry->Vertices[2].Data[texIndex * 2 + 0]);
                vertices.push_back(geometry->Vertices[2].Data[texIndex * 2 + 1]);
                indices.push_back(i);
            }
            glGenVertexArrays(1, &mesh->VAO);
            glGenBuffers(1, &mesh->VBO);
            glGenBuffers(1, &mesh->EBO);
            glBindVertexArray(mesh->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
            mesh->indicesSize = indices.size();
            mesh->indicesType = GL_UNSIGNED_INT;
            meshes.push_back(mesh);
        }
    }
    else if (scwFile->glTFFbs != nullptr) {
        std::map<int, unsigned int> vbos;
        auto glb = sc::model::glTF::GetglTF(scwFile->glTFFbs);
        for (int bIndex = 0;bIndex < glb->bufferViews()->size();bIndex++) {
            auto bv = glb->bufferViews()->Get(bIndex);
            if (bv->target() == 0)
                continue;
            // auto b = glb->buffers()->Get(bv->buffer());

            // printf("buffer: %d %d %d %d %d\n", bv->buffer(), bv->byteLength(), bv->byteOffset(), bv->byteStride(), bv->target());
            unsigned int vbo;
            glGenBuffers(1, &vbo);
            vbos[bIndex] = vbo;
            glBindBuffer(bv->target(), vbo);
            glBufferData(bv->target(), bv->byteLength(), scwFile->glTFBin + bv->byteOffset(), GL_STATIC_DRAW);
            int error = glGetError();
            if (error != 0) {
                printf("error: %d\n", error);
                abort();
            }
        }
        auto defaultScene = glb->scenes()->Get(glb->scene());
        for (int i = 0;i < defaultScene->nodes()->size();i++) {
            processNode(glb, glb->nodes()->Get(defaultScene->nodes()->Get(i)));
        }
        for (int i = 0; i < glb->meshes()->size(); i++) {
            auto m = glb->meshes()->Get(i);
            for (int j = 0;j < m->primitives()->size();j++)
            {
                Mesh* mesh = new Mesh();
                glGenVertexArrays(1, &mesh->VAO);
                glBindVertexArray(mesh->VAO);
                auto primitive = m->primitives()->Get(j);
                auto attributes = flexbuffers::GetRoot(primitive->attributes()->data(), primitive->attributes()->size()).AsMap();
                for (int k = 0;k < attributes.size();k++) {
                    auto accessor = glb->accessors()->Get(attributes.Values()[k].AsInt64());
                    glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor->bufferView()]);
                    int error = glGetError();
                    if (error != 0) {
                        printf("error: %d\n", error);
                        abort();
                    }
                    int vaa = -1;
                    if (strcmp(attributes.Keys()[k].AsKey(), "POSITION") == 0) {
                        vaa = 0;
                    }
                    else if (strcmp(attributes.Keys()[k].AsKey(), "NORMAL") == 0) {
                        vaa = 1;
                    }
                    else if (strcmp(attributes.Keys()[k].AsKey(), "TEXCOORD_0") == 0) {
                        vaa = 2;
                    }
                    else {
                        printf("unknown attribute: %s\n", attributes.Keys()[k].AsKey());
                        continue;
                    }
                    glEnableVertexAttribArray(vaa);
                    error = glGetError();
                    if (error != 0) {
                        printf("error: %d\n", error);
                        abort();
                    }
                    int size;
                    switch (accessor->type()) {
                    case sc::model::glTF::AccessorType_VEC2:
                        size = 2;
                        break;
                    case sc::model::glTF::AccessorType_VEC3:
                        size = 3;
                        break;
                    default:
                        abort();
                    }
                    auto bv = glb->bufferViews()->Get(accessor->bufferView());
                    glVertexAttribPointer(vaa, size, accessor->componentType() & 0xFFFF,/* true || */accessor->normalized(), bv->byteStride(), (void*)accessor->byteOffset());
                    error = glGetError();
                    if (error != 0) {
                        printf("error: %d\n", error);
                        abort();
                    }
                    printf("glVertexAttribPointer: %d %d %d %d %d %d\n", vaa, size, accessor->componentType() & 0xFFFF, accessor->normalized(), bv->byteStride(), accessor->byteOffset());
                    // printf("vaa: %d accessor: %d %d %d %d %d %d\n", vaa, accessor->bufferView(), accessor->byteOffset(), accessor->componentType(), accessor->count(), accessor->type(), accessor->normalized());
                    // printf("%s\n", attribute->c_str());
                    // auto accessor = glb->accessors()->Get(attribute);
                    // printf("%d\n", glb->accessors()->size());
                }
                auto indices = glb->accessors()->Get(primitive->indices());
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[indices->bufferView()]);
                glBindVertexArray(0);
                mesh->indicesSize = indices->count();
                mesh->indicesType = indices->componentType() & 0xFFFF;
                mesh->indicesOffset = indices->byteOffset();
                meshes.push_back(mesh);
            }
        }
    }
}


int Scene::getMeshCount() {
    return meshes.size();
}

Mesh* Scene::getMesh(int index) {
    return meshes[index];
}

void Scene::setTransformation(int index, const glm::mat4& matrix) {
    meshes[index]->matrix = matrix;
}
