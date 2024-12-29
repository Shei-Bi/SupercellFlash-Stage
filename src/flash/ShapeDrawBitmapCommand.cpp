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
        std::vector<float>* v = &Stage->currentBucket->vertices;
        int required = v->size() + vertexSize * 11;
        if (required > 65535) {
            printf("Stage vertex overflow, required:%d", required);
            return true;
        }
        Stage->addTriangles(triangleCount);
        if (v->capacity() < required) v->reserve(v->capacity() + 512 * 11 * 3);
        for (int i = 0;i < vertexSize;i++) {
            auto& vertex = vertexs[i];
            v->push_back(mat->applyX(vertex.x, vertex.y));
            v->push_back(mat->applyY(vertex.x, vertex.y));
            v->push_back(vertex.u / 65536.0f);
            v->push_back(vertex.v / 65536.0f);
            v->push_back(c->mulR / 255.0f);
            v->push_back(c->mulG / 255.0f);
            v->push_back(c->mulB / 255.0f);
            v->push_back(c->alpha / 255.0f);
            v->push_back(c->addR / 255.0f);
            v->push_back(c->addG / 255.0f);
            v->push_back(c->addB / 255.0f);
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
        std::vector<float>* v = &Stage->currentBucket->vertices;
        int required = v->size() + vertexSize * 11;
        if (required > 65535) {
            printf("Stage vertex overflow, required:%d", required);
            return true;
        }
        Stage->addTriangles(triangleCount);
        if (v->capacity() < required) v->reserve(v->capacity() + 512 * 11 * 3);
        for (int i = 0;i < vertexSize;i++) {
            auto& vertex = vertexs[i];
            float x = vertex.x;
            float y = vertex.y;
            if (x <= safeArea->left) x = fmin(safeArea->getMidX(), shapeBounds->left + (x - shapeBounds->left) * width);
            else if (x >= safeArea->right) x = fmax(safeArea->getMidX(), shapeBounds->right + (x - shapeBounds->right) * width);
            if (y <= safeArea->top) y = fmin(safeArea->getMidY(), shapeBounds->top + (y - shapeBounds->top) * height);
            else if (y >= safeArea->bottom) y = fmax(safeArea->getMidY(), shapeBounds->bottom + (y - shapeBounds->bottom) * height);
            v->push_back(mat->applyX(x, y));
            v->push_back(mat->applyY(x, y));
            v->push_back(vertex.u / 65536.0f);
            v->push_back(vertex.v / 65536.0f);
            v->push_back(c->mulR / 255.0f);
            v->push_back(c->mulG / 255.0f);
            v->push_back(c->mulB / 255.0f);
            v->push_back(c->alpha / 255.0f);
            v->push_back(c->addR / 255.0f);
            v->push_back(c->addG / 255.0f);
            v->push_back(c->addB / 255.0f);
        }
    }
    delete bounds;
    return result;
}
unsigned short ShapeDrawBitmapCommand::getVertexCount() {
    return vertexSize;
}