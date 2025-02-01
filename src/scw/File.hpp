#ifndef SCWFILE_H
#define SCWFILE_H

#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Camera3D.hpp"
#include "scw/Geometry.hpp"
#include "scw/Header.hpp"
#include "scw/Material.hpp"
#include "scw/Scene.hpp"
#include <cstdint>
#include <vector>

#include "glTF_generated.h"

#include "flash/3d/Mesh.h"

namespace SCW
{

  class File : public Scene
  {
  private:
    Conv3d::Reader reader = {};

  public:
    // Mesh* getMesh(Geometry*);

    Header Header;
    std::vector<Material*> Materials = {};
    std::vector<Geometry*> Geometries = {};
    std::vector<Camera3D*> Cameras = {};

    const char* glTFJson = nullptr;
    unsigned char* glTFFbs = nullptr;
    unsigned char* glTFBin = nullptr;
    // int glTFBinSize = 0;

    void Load();
    bool LoadSCglTF();
    Conv3d::Writer* Encode();

    File(uint8_t*, int);
    File();

    ~File();
  };

} // namespace SCW

#endif // SCWFILE_H