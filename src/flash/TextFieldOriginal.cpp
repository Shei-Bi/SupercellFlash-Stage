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
    sc->readShort();
    sc->readShort();
    sc->readShort();
    sc->readShort();
    sc->readBool();
    delete[] sc->readAscii(&stack, NULL);
}