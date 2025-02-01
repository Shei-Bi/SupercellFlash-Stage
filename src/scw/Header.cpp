#include "scw/Header.hpp"
#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include <cstdint>

using SCW::Header;

void Header::Decode(Conv3d::Reader* reader)
{
    this->Version = reader->ReadU16LittleEndian();
    this->FrameRate = reader->ReadU16LittleEndian();
    this->FirstFrame = reader->ReadU16LittleEndian();
    this->LastFrame = reader->ReadU16LittleEndian();
    reader->ReadUTF()->into(this->MaterialsFile);

    this->Unknown = reader->SkipBytes() >= 1 ? (int)reader->ReadU8() : -1;
}

void Header::Encode(Conv3d::Writer* writer)
{
    writer->WriteU16(this->Version);
    writer->WriteU16(this->FrameRate);
    writer->WriteU16(this->FirstFrame);
    writer->WriteU16(this->LastFrame);
    writer->WriteStringUTF(this->MaterialsFile);
    if (this->Unknown != -1)
        writer->WriteU8((uint8_t)this->Unknown);
}

const char* Header::Tag() { return "HEAD"; }