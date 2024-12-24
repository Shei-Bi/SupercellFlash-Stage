#include "ShapeDrawBitmapCommand.h"
#include  "SupercellSWF.h"
// #pragma optimize( "t", on )
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
// #pragma optimize( "t", off )

unsigned short ShapeDrawBitmapCommand::getVertexCount() {
    return vertexSize;
}