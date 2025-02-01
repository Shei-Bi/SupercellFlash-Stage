
#include "scw/Scene.hpp"
#include "scw/Node.h"

using SCW::Scene;

void Scene::Encode(Conv3d::Writer *writer)
{
    writer->WriteU16(this->Nodes.size());
    for (Node &node : this->Nodes)
        node.Encode(writer);
}

const char *Scene::Tag() { return "NODE"; }