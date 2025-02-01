#include "scw/Node.h"
#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

using namespace SCW;

double tolerance = 1e-9;

bool areFloatsEqual(float a, float b)
{
    return std::abs((double)(a - b)) <= tolerance;
}

bool Vector3::Equals(Vector3& other)
{
    return areFloatsEqual(X, other.X) && areFloatsEqual(Y, other.Y) &&
        areFloatsEqual(Z, other.Z);
}

bool Quaternion::Equals(Quaternion& other)
{
    return areFloatsEqual(W, other.W) && Vector3::Equals(*this);
}

void KeyFrame::Decode(Conv3d::Reader* reader, uint8_t frameFlags,
    uint16_t frameIndex, std::vector<KeyFrame>& Frames)
{
    this->ID = reader->ReadU16LittleEndian();

    int v61 = (int)frameFlags;

    if (frameFlags == 0)
        v61 = -1;

    if (frameIndex == 0 || (v61 & 1) != 0)
    {
        this->Rotation.X = ((float)reader->ReadI16LittleEndian()) * 0.000030758;
        this->Rotation.Y = ((float)reader->ReadI16LittleEndian()) * 0.000030758;
        this->Rotation.Z = ((float)reader->ReadI16LittleEndian()) * 0.000030758;
        this->Rotation.W = ((float)reader->ReadI16LittleEndian()) * 0.000030758;
    }
    else
    {
        this->Rotation = Frames[0].Rotation;
    }

#define DECODE_PROP(prop, __a)                                                 \
    if (frameIndex == 0 || (v61 & __a) != 0)                                   \
        this->prop = reader->ReadFloatLittleEndian();                                      \
    else                                                                       \
        this->prop = Frames[0].prop

    DECODE_PROP(Translation.X, 2);
    DECODE_PROP(Translation.Y, 4);
    DECODE_PROP(Translation.Z, 8);

    DECODE_PROP(Scale.X, 0x10);
    DECODE_PROP(Scale.Y, 0x20);
    DECODE_PROP(Scale.Z, 0x40);

#undef DECODE_PROP
}

void KeyFrame::Encode(Conv3d::Writer* writer, uint8_t frameFlags,
    uint16_t frameIndex)
{
    writer->WriteU16(this->ID);

    int v61 = (int)frameFlags;
    if (frameFlags == 0)
        v61 = -1;

    if (frameIndex == 0 || (v61 & 1) != 0)
    {
#define ENCODE(elem)                                                           \
    writer->WriteI16((int16_t)(this->Rotation.elem / 0.000030758))
        ENCODE(X);
        ENCODE(Y);
        ENCODE(Z);
        ENCODE(W);
#undef ENCODE
    }

#define ENCODE_PROP(prop, __a)                                                 \
    if (frameIndex == 0 || (v61 & __a) != 0)                                   \
        writer->WriteFloat(this->prop);

    ENCODE_PROP(Translation.X, 2);
    ENCODE_PROP(Translation.Y, 4);
    ENCODE_PROP(Translation.Z, 8);

    ENCODE_PROP(Scale.X, 0x10);
    ENCODE_PROP(Scale.Y, 0x20);
    ENCODE_PROP(Scale.Z, 0x40);
#undef ENCODE_PROP
}

void InstanceMaterial::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);
    reader->ReadUTF()->into(this->Target);
}

void InstanceMaterial::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);
    writer->WriteStringUTF(this->Target);
}

void NodeInstance::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTFWithLength(4)->into(this->Type);
    reader->ReadUTF()->into(this->Target);

    if (this->Type == "GEOM" || this->Type == "CONT")
    {
        uint16_t count = reader->ReadU16LittleEndian();
        this->Materials.resize(count);
        for (int i = 0; i < count; i++)
            this->Materials[i].Decode(reader);
    }
    else if (this->Type == "LIGHT")
        throw std::runtime_error("not supported yet");
    else if (this->Type == "CAME")
        reader->ReadUTF()->into(this->CameraTarget);
    else
    {
        std::string error = "invalid or unsupported instance material type: ";
        error += this->Type.data();
        throw std::runtime_error(error);
    }
}

void NodeInstance::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringChars(this->Type);
    writer->WriteStringUTF(this->Target);

    if (this->Type == "GEOM" || this->Type == "CONT")
    {
        writer->WriteU16(this->Materials.size());
        for (InstanceMaterial& IM : this->Materials)
            IM.Encode(writer);
    }
    else if (this->Type == "LIGHT")
        throw std::runtime_error("not supported yet");
    else if (this->Type == "CAME")
        writer->WriteStringUTF(this->CameraTarget);
    else
    {
        std::string error = "invalid or unsupported instance material type: ";
        error += this->Type.data();
        throw std::runtime_error(error);
    }
}

void Node::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);
    reader->ReadUTF()->into(this->ParentName);

    uint16_t count = reader->ReadU16LittleEndian();
    this->Instances.resize(count);
    for (int i = 0; i < count; i++)
        this->Instances[i].Decode(reader);

    count = reader->ReadU16LittleEndian();
    this->Frames.resize(count);
    if (count > 0)
        this->FrameFlags = reader->ReadU8();

    for (int i = 0; i < count; i++)
        this->Frames[i].Decode(reader, this->FrameFlags, i, this->Frames);
}

uint8_t computeFrameFlags(std::vector<KeyFrame>&);

void Node::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);
    writer->WriteStringUTF(this->ParentName);

    writer->WriteU16(this->Instances.size());

    for (NodeInstance& NI : this->Instances)
        NI.Encode(writer);

    writer->WriteU16(this->Frames.size());

    if (this->Frames.size() > 0)
    {
        uint8_t frameFlags = computeFrameFlags(this->Frames);
        writer->WriteU8(frameFlags);

        for (int i = 0; i < this->Frames.size(); i++)
            this->Frames[i].Encode(writer, frameFlags, i);
    }
}

uint8_t computeFrameFlags(std::vector<KeyFrame>& frames)
{
    uint8_t flags = 0;

    bool Rotation = true, TranslationX = true, TranslationY = true,
        TranslationZ = true, ScaleX = true, ScaleY = true, ScaleZ = true;

    KeyFrame& firstFrame = frames[0];

    for (int i = 1; i < frames.size(); i++)
    {
        KeyFrame& frame = frames[i];

        if (Rotation && !frame.Rotation.Equals(firstFrame.Rotation))
            Rotation = false;

#define CHECK(prop, elem)                                                      \
    if (prop##elem && !areFloatsEqual(frame.prop.elem, firstFrame.prop.elem))  \
    prop##elem = false

        CHECK(Translation, X);
        CHECK(Translation, Y);
        CHECK(Translation, Z);

        CHECK(Scale, X);
        CHECK(Scale, Y);
        CHECK(Scale, Z);

#undef CHECK
    }

#define CHECK(prop, __bit)                                                     \
    if (prop)                                                                  \
    flags |= 1 << __bit

    CHECK(Rotation, 0);

    CHECK(TranslationX, 1);
    CHECK(TranslationY, 2);
    CHECK(TranslationZ, 3);

    CHECK(ScaleX, 4);
    CHECK(ScaleY, 5);
    CHECK(ScaleZ, 6);
#undef CHECK

    return flags;
}