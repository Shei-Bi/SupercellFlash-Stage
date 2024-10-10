#pragma once
#include <flash/DisplayObject.h>
#include <flash/display_object/ShapeOriginal.h>
#include <flash/display_object/ShapeDrawBitmapCommand.h>
#include <flash/types/SWFContainer.hpp>
#include <glad/glad.h>

class Shape :public DisplayObject
{
private:
    sc::flash::SWFVector<sc::flash::ShapeDrawBitmapCommand>* commands = nullptr;

    /* data */
public:
    static unsigned int* createTriangleIndices(int triangleCount) {
        unsigned int* indices = new unsigned int[triangleCount * 3];
        for (int i = 0; i < triangleCount; i++) {
            indices[i * 3] = 0;
            indices[i * 3 + 1] = i + 1;
            indices[i * 3 + 2] = i + 2;
        }
        return indices;
    }
    static Shape* createShape(sc::flash::ShapeOriginal* original) {
        Shape* shape = new Shape();
        shape->commands = &original->commands;
        return shape;
    }
    bool render() {
        // float vertices[] = {
        //     // positions          // colors           // texture coords
        //       -0.540000 ,-1.000000,    0.444144,  0.628641, // top right
        //      0.640000 ,-1.000000,   0.404913 ,0.628641, // bottom right
        //     0.940000 ,0.020000,   0.394934 ,0.602075, // bottom left
        //      0.940000 ,0.460000,   0.394934 ,0.590616,  // top left 
        //      0.240000 ,0.960000,  0.418204 ,0.577600  // top left 
        // };
        // unsigned int indices[] = {
        //     // 注意索引从0开始! 
        //     // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        //     // 这样可以由下标代表顶点组合成矩形

        //     0, 1, 2, // 第一个三角形
        //     0, 2, 3,  // 第二个三角形
        //     0,3,4
        // };
        // int triangleCount = 3;

        unsigned int VBO, VAO, EBO;

        sc::flash::ShapeDrawBitmapCommand command = (*commands)[0];
        float* vertices = new float[command.vertices.size() * 4];
        int triangleCount = command.vertices.size() - 2;
        unsigned int* indices = createTriangleIndices(triangleCount);
        for (int i = 0;i < command.vertices.size();i++) {
            sc::flash::ShapeDrawBitmapCommandVertex vertex = command.vertices[i];
            vertices[i * 4] = vertex.x / 25;
            vertices[i * 4 + 1] = vertex.y / 25;
            vertices[i * 4 + 2] = vertex.u;
            vertices[i * 4 + 3] = vertex.v;
        }
        for (int i = 0;i < command.vertices.size() * 4;i++) {
            printf("vertice[%d]: %f\n", i, vertices[i]);
        }
        for (int i = 0;i < triangleCount * 3;i++) {
            printf("indices[%d]: %d\n", i, indices[i]);
        }
        // for (sc::flash::ShapeDrawBitmapCommand command : *commands) {
        //     unsigned int* indices = createTriangleIndices(command.vertices.size() - 2);

        // }
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, command.vertices.size() * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_INT, 0);
        return true;
    }
};