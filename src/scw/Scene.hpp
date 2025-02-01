#ifndef SCWSCENE_H
#define SCWSCENE_H

#include "file/Writer.hpp"
#include "scw/Node.h"
#include "scw/Property.hpp"
#include <vector>

namespace SCW
{

class Scene : public Property
{
  public:
    std::vector<Node> Nodes = {};

    const char *Tag();
    void Encode(Conv3d::Writer *);
};

} // namespace SCW

#endif // SCWSCENE_H