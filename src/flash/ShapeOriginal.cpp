#include "ShapeOriginal.h"
#include "SupercellSWF.h"
#include "ShapeDrawBitmapCommandVertex.h"
#include "Shape.h"
#include "Shape9Slice.h"
void ShapeOriginal::load(SupercellSWF* sc) {
    id = sc->readShort();
    commandSize = sc->readShort();
    commands.resize(commandSize);
    unsigned short totalVertexs = sc->readShort();
    sharedVertexArray.resize(totalVertexs);
    unsigned short loadedVertexs = 0;
    unsigned short loadedCommands = 0;
    while (true) {
        unsigned char tag = sc->readUnsignedChar();
        int l = sc->readInt();
        switch (tag) {
        case 0:
            return;
        case 22:
            commands[loadedCommands].load(sc, &sharedVertexArray.data()[loadedVertexs]);
            loadedVertexs += commands[loadedCommands].getVertexCount();
            loadedCommands++;
        }
    }
}
Shape* ShapeOriginal::clone(SupercellSWF* sc, Rect* rect) {
    if (rect) return Shape9Slice::createShape(this, rect);
    else return Shape::createShape(this);
}