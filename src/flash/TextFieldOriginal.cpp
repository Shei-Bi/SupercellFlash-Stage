#include "TextFieldOriginal.h"
#include "SupercellSWF.h"
void TextFieldOriginal::load(SupercellSWF* sc) {
    id = sc->readShort();
    int stack;
    delete[] sc->readAscii(&stack, NULL);
    sc->readInt();
    sc->readBool();
    sc->readBool();
    sc->readBool();
    sc->readBool();
    sc->readUnsignedChar();
    sc->readUnsignedChar();
    float left = (float)(short)sc->readShort();
    float top = (float)(short)sc->readShort();
    float right = (float)(short)sc->readShort();
    float bottom = (float)(short)sc->readShort();
    bounds = Rect(left, top, right, bottom);
    sc->readBool();
    delete[] sc->readAscii(&stack, NULL);
}

TextField* TextFieldOriginal::clone(SupercellSWF*, Rect*) {
    return TextField::createTextField(this);
}
