#include "scw/Camera3D.hpp"
#include "file/Reader.hpp"
#include "file/Writer.hpp"

using SCW::Camera3D;

void Camera3D::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);

    this->Yfox = reader->ReadFloatLittleEndian();
    this->Xfov = reader->ReadFloatLittleEndian();

    this->AspectRatio = reader->ReadFloatLittleEndian();

    this->ZNear = reader->ReadFloatLittleEndian();
    this->ZFar = reader->ReadFloatLittleEndian();
}

void Camera3D::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);
    writer->WriteFloat(this->Yfox);
    writer->WriteFloat(this->Xfov);
    writer->WriteFloat(this->AspectRatio);
    writer->WriteFloat(this->ZNear);
    writer->WriteFloat(this->ZFar);
}

const char* Camera3D::Tag() { return "CAME"; }