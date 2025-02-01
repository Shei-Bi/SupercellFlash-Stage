#ifndef TEXTFIELDORIGINAL_H
#define TEXTFIELDORIGINAL_H
#include "DisplayObjectOriginal.h"
#include "TextField.h"

class TextFieldOriginal :public DisplayObjectOriginal {
public:
    void load(SupercellSWF* sc);
    TextField* clone(SupercellSWF*, Rect*);

    Rect bounds;
};
#endif