#include "ShapeDrawBitmapCommand.h"
#include  "SupercellSWF.h"
#include "Stage.h"
void ShapeDrawBitmapCommand::load(SupercellSWF* sc, ShapeDrawBitmapCommandVertex* sharedVertexArray) {
    glImage = sc->textures[sc->readUnsignedChar()].glImage;
    vertexSize = sc->readUnsignedChar();
    vertexs = sharedVertexArray;
    // [[assume(vertexSize != 0)]];
    for (unsigned short i = 0;i < vertexSize;i++) {
        vertexs[i].x = sc->readTwip();
        vertexs[i].y = sc->readTwip();
    }
    for (unsigned short i = 0;i < vertexSize;i++) {
        vertexs[i].u = sc->readShort();
        vertexs[i].v = sc->readShort();
        // sc->readShortArray(1, &vertexs[i].u);
        // sc->readShortArray(1, &vertexs[i].v);
    }
}
bool ShapeDrawBitmapCommand::render(Matrix2x3* mat, ColorTransform* c, int rc) {
    Rect* bounds = new Rect(20480, 20480, -20480, -20480);
    for (int i = 0;i < vertexSize;i++) {
        auto& vertex = vertexs[i];
        Stage::updateBound(bounds, mat->applyX(vertex.x, vertex.y), mat->applyY(vertex.x, vertex.y));
    }
    Stage* Stage = Stage::getInstance();
    bool result;
    if (Stage->shapeStart(bounds->left, bounds->top, bounds->right, bounds->bottom, glImage, rc, &result)) {
        int triangleCount = vertexSize - 2;
        Stage->addTriangles(triangleCount);
        for (int i = 0;i < vertexSize;i++) {
            auto& vertex = vertexs[i];
            auto v = &Stage->verticesBucket[Stage->verticesBucketSize];
            v[0] = mat->applyX(vertex.x, vertex.y);
            v[1] = mat->applyY(vertex.x, vertex.y);
            v[2] = vertex.u / 65536.0f;
            v[3] = vertex.v / 65536.0f;
            v[4] = c->mulR / 255.0f;
            v[5] = c->mulG / 255.0f;
            v[6] = c->mulB / 255.0f;
            v[7] = c->alpha / 255.0f;
            v[8] = c->addR / 255.0f;
            v[9] = c->addG / 255.0f;
            v[10] = c->addB / 255.0f;
            Stage->verticesBucketSize += 11;
        }
    }
    delete bounds;
    return result;
}
// #pragma optimize( "", off )
bool ShapeDrawBitmapCommand::render9Slice(Matrix2x3* mat, ColorTransform* c, int rc, Rect* safeArea, Rect* shapeBounds, float width, float height) {
    // return render(mat, c, rc);
    Rect* bounds = new Rect();
    for (int i = 0;i < vertexSize;i++) {
        auto& vertex = vertexs[i];
        float x = vertex.x;
        float y = vertex.y;
        if (x <= safeArea->left) x = fmin(safeArea->getMidX(), shapeBounds->left + (x - shapeBounds->left) * width);
        else if (x >= safeArea->right) x = fmax(safeArea->getMidX(), shapeBounds->right + (x - shapeBounds->right) * width);
        if (y <= safeArea->top) y = fmin(safeArea->getMidY(), shapeBounds->top + (y - shapeBounds->top) * height);
        else if (y >= safeArea->bottom) y = fmax(safeArea->getMidY(), shapeBounds->bottom + (y - shapeBounds->bottom) * height);
        Stage::updateBound(bounds, mat->applyX(x, y), mat->applyY(x, y));
    }
    bool result;
    Stage* Stage = Stage::getInstance();
    if (Stage->shapeStart(bounds->left, bounds->top, bounds->right, bounds->bottom, glImage, rc, &result)) {
        int triangleCount = vertexSize - 2;
        Stage->addTriangles(triangleCount);
        auto v = &Stage->verticesBucket;
        for (int i = 0;i < vertexSize;i++) {
            auto& vertex = vertexs[i];
            float x = vertex.x;
            float y = vertex.y;
            if (x <= safeArea->left) x = fmin(safeArea->getMidX(), shapeBounds->left + (x - shapeBounds->left) * width);
            else if (x >= safeArea->right) x = fmax(safeArea->getMidX(), shapeBounds->right + (x - shapeBounds->right) * width);
            if (y <= safeArea->top) y = fmin(safeArea->getMidY(), shapeBounds->top + (y - shapeBounds->top) * height);
            else if (y >= safeArea->bottom) y = fmax(safeArea->getMidY(), shapeBounds->bottom + (y - shapeBounds->bottom) * height);
            auto v = &Stage->verticesBucket[Stage->verticesBucketSize];
            v[0] = mat->applyX(x, y);
            v[1] = mat->applyY(x, y);
            v[2] = vertex.u / 65536.0f;
            v[3] = vertex.v / 65536.0f;
            v[4] = c->mulR / 255.0f;
            v[5] = c->mulG / 255.0f;
            v[6] = c->mulB / 255.0f;
            v[7] = c->alpha / 255.0f;
            v[8] = c->addR / 255.0f;
            v[9] = c->addG / 255.0f;
            v[10] = c->addB / 255.0f;
            Stage->verticesBucketSize += 11;
        }
    }
    delete bounds;
    return result;
    return false;
}
unsigned short ShapeDrawBitmapCommand::getVertexCount() {
    return vertexSize;
}