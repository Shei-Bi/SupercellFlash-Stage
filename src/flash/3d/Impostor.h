#pragma once
#include "Sprite3D.h"

class Impostor
{
public:
    Sprite3D* sprite3D;
    Material* impostorMaterial;
    Impostor();
    ~Impostor();
};