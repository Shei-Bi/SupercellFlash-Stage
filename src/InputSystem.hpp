#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "stdio.h"
#include "Touch.h"
#include "IInputListener.h"
#include <vector>
class InputSystem {
public:
    int leftBtnState;
    std::vector<IInputListener*> listeners;
    InputSystem() {
        leftBtnState = GLFW_RELEASE;
    }
    void update() {
        int state = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT);
        if (leftBtnState == GLFW_RELEASE && (state == GLFW_PRESS || state == GLFW_REPEAT)) {
            Touch touch;
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            touch.x = xpos;
            touch.y = ypos;
            printf("pressed %f   %f\n", touch.x, touch.y);
            for (IInputListener* i : listeners) if (i->touchPressed(touch)) break;
        }
        if ((leftBtnState == GLFW_PRESS || leftBtnState == GLFW_REPEAT) && state == GLFW_RELEASE) {
            printf("released\n");
        }
        leftBtnState = state;
        // ;glfwGetCursorPos(window, &xpos, &ypos);
    }
};