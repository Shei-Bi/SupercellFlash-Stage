#include "MovieClipModifierOriginal.h"
#include "MovieClipModifier.h"
#include "SupercellSWF.h"

void MovieClipModifierOriginal::load(SupercellSWF* sc, int type) {
    id = sc->readShort();
    this->type = type;
}

DisplayObject* MovieClipModifierOriginal::clone(SupercellSWF*, Rect*) {
    return new MovieClipModifier(this);
}
