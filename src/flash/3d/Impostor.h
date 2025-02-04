#pragma once
#include "Sprite3D.h"
#include "RenderTarget.h"

class Impostor
{
public:
    Sprite3D* sprite3D;
    Material* impostorMaterial;
    Impostor(RenderTarget* renderTarget);
    ~Impostor();
    void render();
};