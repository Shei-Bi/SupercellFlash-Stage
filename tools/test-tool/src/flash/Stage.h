#pragma once
#include <glad/glad.h>
class Stage
{
private:
    /* data */
public:
    static Stage* sm_pInstance;
    static Stage* getInstance();
    void constructInstance();
    void render(float deltaTime, bool clear) {
        if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
};
