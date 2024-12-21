#include "ShapeDrawBitmapCommand.h"
#include  "SupercellSWF.h"
void ShapeDrawBitmapCommand::load(SupercellSWF* sc, ShapeDrawBitmapCommandVertex* sharedVertexArray) {
    glImage = sc->textures[sc->readUnsignedChar()].glImage;
    vertexSize = sc->readUnsignedChar();
    vertexs = sharedVertexArray;
    for (unsigned short i = 0;i < vertexSize;i++) {
        vertexs[i].x = sc->readTwip();
        vertexs[i].y = sc->readTwip();
    }
    for (unsigned short i = 0;i < vertexSize;i++) {
        vertexs[i].u = sc->readShort();
        vertexs[i].v = sc->readShort();
    }
}
unsigned short ShapeDrawBitmapCommand::getVertexCount() {
    return vertexSize;
}