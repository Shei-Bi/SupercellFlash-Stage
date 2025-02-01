#include "scw/Material.hpp"
#include "file/Reader.hpp"
#include "file/Writer.hpp"

using SCW::Material, SCW::Variable, SCW::RGBA;

void RGBA::Decode(Conv3d::Reader* reader)
{
#define DECODE_PARAM(param) this->colors.param = reader->ReadU8()

    DECODE_PARAM(r);
    DECODE_PARAM(g);
    DECODE_PARAM(b);
    DECODE_PARAM(a);

#undef DECODE_PARAM
}

void RGBA::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringChars(this->colors.bytes, 4);
}

void Variable::Decode(Conv3d::Reader* reader)
{
    this->UseText2D = reader->ReadBool();
    if (this->UseText2D)
        reader->ReadUTF()->into(this->Texture2D);
    else
        this->Color.Decode(reader);
}

void Variable::Encode(Conv3d::Writer* writer)
{
    if (writer->WriteBool(this->UseText2D))
        writer->WriteStringUTF(this->Texture2D);
    else
        this->Color.Encode(writer);
}

void Material::Decode(Conv3d::Reader* reader)
{
    reader->ReadUTF()->into(this->Name);
    reader->ReadUTF()->into(this->ShaderFile);

    this->BlendMode = reader->ReadU8();

    // uint8_t idk = reader->ReadU8();
    // uint32_t idk2 = reader->ReadU32();

    // std::cout << "idk1: " << idk << ", idk2: " << idk2 << "\n";

    this->Variables.Ambient.Decode(reader);
    this->Variables.Diffuse.Decode(reader);
    this->Variables.Specular.Decode(reader);

    reader->ReadUTF()->into(this->Variables.StencilTex2D);

    reader->ReadUTF()->into(this->Variables.NormalTex2D);

    // if m.SCWFile.Version >= 2 {
    // 	if m.Variables.NormalTex2D, err = reader.ReadUTF(); err != nil {
    // 		return
    // 	}
    // }

    this->Variables.Colorize.Decode(reader);
    this->Variables.Emission.Decode(reader);

    reader->ReadUTF()->into(this->Variables.OpacityTex2D);
    this->Variables.Opacity = reader->ReadFloatLittleEndian();

    this->Variables.Unk = reader->ReadFloatLittleEndian();

    reader->ReadUTF()->into(this->Variables.LightmapTex2D);
    reader->ReadUTF()->into(this->Variables.LightmapSpecularTex2D);

    reader->ReadUTF()->into(this->Variables.Unk2);
    // if m.SCWFile.Version >= 2 {
    // 	if m.Variables.Unk2, err = reader.ReadUTF(); err != nil {
    // 		return
    // 	}
    // }

    this->ShaderConfig = reader->ReadU32LittleEndian();

    if ((this->ShaderConfig & 0x8000) != 0)
        for (int i = 0; i < 4; i++)
            this->StencilScaleOffset[i] = reader->ReadFloatLittleEndian();
}

void Material::Encode(Conv3d::Writer* writer)
{
    writer->WriteStringUTF(this->Name);
    writer->WriteStringUTF(this->ShaderFile);

    writer->WriteU8(this->BlendMode);

    this->Variables.Ambient.Encode(writer);
    this->Variables.Diffuse.Encode(writer);
    this->Variables.Specular.Encode(writer);

    writer->WriteStringUTF(this->Variables.StencilTex2D);
    writer->WriteStringUTF(this->Variables.NormalTex2D);

    this->Variables.Colorize.Encode(writer);
    this->Variables.Emission.Encode(writer);

    writer->WriteStringUTF(this->Variables.OpacityTex2D);
    writer->WriteFloat(this->Variables.Opacity);

    writer->WriteFloat(this->Variables.Unk);

    writer->WriteStringUTF(this->Variables.LightmapTex2D);
    writer->WriteStringUTF(this->Variables.LightmapSpecularTex2D);

    writer->WriteStringUTF(this->Variables.Unk2);

    writer->WriteU32(this->ShaderConfig);

    if ((this->ShaderConfig & 0x8000) != 0)
        for (int i = 0; i < 4; i++)
            writer->WriteFloat(this->StencilScaleOffset[i]);
}

const char* Material::Tag() { return "MATE"; }