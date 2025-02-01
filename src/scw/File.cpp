#include "scw/File.hpp"
#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Camera3D.hpp"
#include "scw/Geometry.hpp"
#include "scw/Material.hpp"
#include "scw/Property.hpp"
#include "scw/Scene.hpp"
#include "scw/Wend.hpp"
#include "util/CRC32.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "glTF_generated.h"

using namespace SCW;

using Conv3d::String;

File::File(uint8_t* data, int size) : reader(data, size), Scene() {}

void File::Load()
{
    String magic;
    reader.ReadUTFWithLength(4)->into(magic);

    if (magic != "SC3D")
        throw std::runtime_error("invalid SC3D magic");

    Conv3d::Reader* reader = &this->reader;

    uint32_t length = 0;

    bool end = false;

    String prop = {};
    while (!end)
    {
        length = reader->ReadU32LittleEndian();
        prop.clear();
        reader->ReadUTFWithLength(4)->into(prop);

        reader->SetSkipBytes((int)length);

        if (prop == "HEAD")
        {
            this->Header.Decode(reader);
        }
        else if (prop == "MATE")
        {
            Material* material = new Material();
            this->Materials.push_back(material);
            material->Decode(reader);
        }
        else if (prop == "GEOM")
        {
            Geometry* geometry = new Geometry();
            this->Geometries.push_back(geometry);
            geometry->Decode(reader);
        }
        else if (prop == "CAME")
        {
            Camera3D* camera = new Camera3D();
            this->Cameras.push_back(camera);
            camera->Decode(reader);
        }
        else if (prop == "NODE")
        {
            uint16_t nodesCount = reader->ReadU16LittleEndian();
            this->Nodes.resize(nodesCount);
            for (int i = 0; i < nodesCount; i++)
                this->Nodes[i].Decode(reader);
        }
        else if (prop == "WEND")
        {
            end = true;
        }
        else
        {
            std::cout << "new prop: " << prop.data() << "\n";
            end = true;
            reader->SetSkipBytes(0);
        }

        if (reader->SkipBytes() != 0)
        {
            std::string error =
                "failed to parse SCW model property infully, chunk: ";

            std::cout << "skip bytes: " << reader->SkipBytes() << "\n";

            error += prop.data();
            error += ", bytes left: ";
            error += std::to_string(reader->SkipBytes());
            throw std::runtime_error(error.data());
        }

        uint32_t crc = reader->ReadU32LittleEndian(); // crc32(prop + buffer)
        // std::cout << "crc of " << prop.data() << ": " << crc << "\n";
    }
}

bool SCW::File::LoadSCglTF()
{
    String magic;
    reader.ReadUTFWithLength(4)->into(magic);

    if (magic != "glTF")
        return false;

    Conv3d::Reader* reader = &this->reader;

    int version = reader->ReadU32();
    if (version != 2)
        return false;

    reader->ReadU32();
    uint32_t length = 0;

    bool end = false;

    String prop = {};
    while (reader->HasData(4))
    {
        length = reader->ReadU32();
        prop.clear();
        reader->ReadUTFWithLength(4)->into(prop);

        if (prop == "FLA2") {
            if (glTFFbs || glTFJson) abort();
            glTFFbs = new unsigned char[length];
            memcpy(glTFFbs, reader->Read(length), length);
        }
        else if (prop == "BIN") {
            if (glTFBin) abort();
            glTFBin = new unsigned char[length];
            memcpy(glTFBin, reader->Read(length), length);
        }
        else if (prop == "JSON") {
            if (glTFJson || glTFFbs) abort();
            glTFJson = new char[length];
            memcpy((void*)glTFJson, reader->Read(length), length);
        }
        else {
            abort();
        }
    }
    return true;
}
void EncodeSc3dProperty(SCW::Property* property, Conv3d::Writer* writer)
{
    Conv3d::Writer* w = new Conv3d::Writer();
    w->WriteStringChars(property->Tag(), 4);
    property->Encode(w);

    uint8_t* bytes = w->Bytes();
    size_t size = w->Size();

    CRC32 crc32 = CRC32();
    crc32.Update(bytes, size);

    uint32_t checksum = crc32.GetValue();

    writer->WriteU32(size - 4); // size - len(property->Tag())
    writer->WriteBytes(bytes, size);

    delete w;

    writer->WriteU32(checksum);
}

Conv3d::Writer* File::Encode()
{
    Conv3d::Writer* writer = new Conv3d::Writer();
    writer->WriteStringChars("SC3D", 4);

    EncodeSc3dProperty(&this->Header, writer);

    for (Material* Mat : this->Materials)
        EncodeSc3dProperty(Mat, writer);

    for (Camera3D* Cam : this->Cameras)
        EncodeSc3dProperty(Cam, writer);

    for (Geometry* Geo : this->Geometries)
        EncodeSc3dProperty(Geo, writer);

    EncodeSc3dProperty(this, writer); // Scene

    Wend wend = {};
    EncodeSc3dProperty(&wend, writer);

    return writer;
}

File::~File()
{
    for (Material* Mat : this->Materials)
        delete Mat;

    for (Geometry* Geo : this->Geometries)
        delete Geo;

    for (Camera3D* Cam : this->Cameras)
        delete Cam;
}

File::File() {}