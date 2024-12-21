#include "MovieClipModifier.h"
#include "SupercellSWF.h"
void MovieClipModifier::load(SupercellSWF* sc, int type) {
    id = sc->readShort();
    this->type = type;
}