#include "scw/Geometry.hpp"
#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>

using namespace SCW;

float& Matrix4x4::operator()(int x, int y) { return this->elems[x][y]; }

void Matrix4x4::Transpose()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            float temp = this->elems[i][j];
            this->elems[i][j] = this->elems[j][i];
            this->elems[j][i] = temp;
        }
    }
}

void Matrix4x4::Decode(Conv3d::Reader* reader)
{
    for (int i = 0; i < 16; i++)
        this->elems[i / 4][i % 4] = reader->ReadFloatLittleEndian();
    this->Transpose();
}

void Matrix4x4::Encode(Conv3d::Writer* writer)
{
    Matrix4x4 matrix = *this;
    matrix.Transpose();
    for (int i = 0; i < 16; i++)
        writer->WriteFloat(matrix.elems[i / 4][i % 4]);
}

void Weight::Decode(Conv3d::Reader* reader)
{
    for (int i = 0; i < 4; i++)
        this->Joints[i] = reader->ReadU8();

    for (int i = 0; i < 4; i++)
        this->Weights[i] = reader->ReadU16LittleEndian();
}

void Weight::Encode(Conv3d::Writer* writer)
{
    writer->WriteBytes(this->Joints, 4);
    for (int i = 0; i < 4; i++)
        writer->WriteU16(this->Weights[i]);
}

void SourceArray::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);
    this->Index = reader->ReadU8();
    this->SourceIndex = reader->ReadU8();
    this->Stride = reader->ReadU8();

    this->Scale = reader->ReadFloatLittleEndian();
    uint32_t count = reader->ReadU32LittleEndian();

    count *= (uint32_t)this->Stride;
    this->Data.resize(count);

    for (int i = 0; i < count; i++)
    {
        int16_t val = reader->ReadI16LittleEndian();
        this->Data[i] = (double)val * (double)this->Scale;
    }
}

void SourceArray::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);
    writer->WriteU8(this->Index);
    writer->WriteU8(this->SourceIndex);
    writer->WriteU8(this->Stride);

    writer->WriteFloat(this->Scale);

    uint32_t count = this->Data.size();
    count /= (uint32_t)this->Stride;

    writer->WriteU32(count);

    for (double data : this->Data)
    {
        int16_t val = (int16_t)(data / (double)this->Scale);
        writer->WriteI16(val);
    }
}

void IndexArray::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);

    this->TrianglesCount = reader->ReadU32LittleEndian();

    this->InputsCount = reader->ReadU8();
    this->IndexBufferSize = reader->ReadU8();

    uint32_t totalIndices = 3 * TrianglesCount * (uint32_t)InputsCount;

    this->IndexBuffer.resize(totalIndices);

    for (int i = 0; i < totalIndices; i++)
    {
        switch (this->IndexBufferSize)
        {
        case 1:
            this->IndexBuffer[i] = (uint32_t)reader->ReadU8();
            break;
        case 2:
            this->IndexBuffer[i] = (uint32_t)reader->ReadU16LittleEndian();
            break;
        case 4:
            this->IndexBuffer[i] = reader->ReadU32LittleEndian();
            break;
        default:
            std::string error = "unsupported index buffer size: ";
            error += std::to_string(this->IndexBufferSize);
            throw std::runtime_error(error);
        }
    }
}

void IndexArray::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);

    writer->WriteU32(this->TrianglesCount);
    writer->WriteU8(this->InputsCount);
    writer->WriteU8(this->IndexBufferSize);

    for (uint32_t v : this->IndexBuffer)
    {
        switch (this->IndexBufferSize)
        {
        case 1:
            writer->WriteU8(v);
            break;
        case 2:
            writer->WriteU16(v);
            break;
        case 4:
            writer->WriteU32(v);
            break;
        default:
            std::string error = "unsupported index buffer size: ";
            error += std::to_string(this->IndexBufferSize);
            throw std::runtime_error(error);
        }
    }
}

void Geometry::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);
    reader->ReadUTF()->into(this->Group);

    // if g.SCWFile.Version <= 1 {
    // 	if err = g.IgnoredMatrix.Decode(reader); err != nil {
    // 		return
    // 	}
    // }

    uint8_t verticesCount = reader->ReadU8();
    this->Vertices.resize(verticesCount);

    for (int i = 0; i < verticesCount; i++)
        this->Vertices[i].Decode(reader);

    if ((this->HasBindMatrix = reader->ReadBool()))
        this->BindMatrix.Decode(reader);

    uint8_t skinsCount = reader->ReadU8();

    this->Skins.Joints.resize(skinsCount);
    this->Skins.InverseBindMatrices.resize(skinsCount);

    for (int i = 0; i < skinsCount; i++)
    {
        reader->ReadUTF()->into(this->Skins.Joints[i]);
        this->Skins.InverseBindMatrices[i].Decode(reader);
    }

    uint32_t skinWeightsCount = reader->ReadU32LittleEndian();

    this->SkinWeights.resize(skinWeightsCount);

    for (int i = 0; i < skinWeightsCount; i++)
        this->SkinWeights[i].Decode(reader);

    uint8_t indexesCount = reader->ReadU8();
    for (int i = 0; i < indexesCount; i++)
    {
        IndexArray IA = {};
        IA.Decode(reader);
        this->Materials.push_back(IA);
    }
}

void Geometry::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);
    writer->WriteStringUTF(this->Group);

    writer->WriteU8(this->Vertices.size());

    for (SourceArray& sourceArray : this->Vertices)
        sourceArray.Encode(writer);

    if (writer->WriteBool(this->HasBindMatrix))
        this->BindMatrix.Encode(writer);

    writer->WriteU8(this->Skins.Joints.size());

    for (int i = 0; i < this->Skins.Joints.size(); i++)
    {
        writer->WriteStringUTF(this->Skins.Joints[i]);
        this->Skins.InverseBindMatrices[i].Encode(writer);
    }

    writer->WriteU32(this->SkinWeights.size());
    for (Weight& skinWeight : this->SkinWeights)
        skinWeight.Encode(writer);

    writer->WriteU8(this->Materials.size());
    for (IndexArray& IA : this->Materials)
        IA.Encode(writer);
}

const char* Geometry::Tag() { return "GEOM"; }