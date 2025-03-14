#pragma once
#include <flash/Stage.h>
#include <flash/MovieClipModifier.h>
#include <flash/DisplayObject.h>
#include "MovieClipModifierOriginal.h"
#include "MovieClipModifier.h"

MovieClipModifier::MovieClipModifier(MovieClipModifierOriginal* original) {
    type = original->type;
}

bool MovieClipModifier::render(Matrix2x3* mat, ColorTransform* c, int, float) {
    Stage* Stage = Stage::getInstance();
    if (!Stage->isCalculatingBounds) {
        switch (type) {
        case 38:
            Stage->setStencilRenderingState(2);
            break;
        case 39:
            Stage->setStencilRenderingState(3);
            break;
        case 40:
            Stage->setStencilRenderingState(4);
            break;
        }
    }
    return false;
}

bool MovieClipModifier::collisionRender(Matrix2x3* mat) {
    return false;
}
