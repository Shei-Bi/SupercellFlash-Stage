#pragma once
#include <ResourceManager.h>
#include <xTimer.h>
#include <flash/MovieClipOriginal.h>
#include <flash/SupercellSWF.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TextureData_generated.h>
#include "zstd.h"

#include "ScDowngrade.h"

namespace fs = std::filesystem;

#include <fstream>

std::map<std::string, void*> ResourceManager::Resources = std::map<std::string, void*>();
std::set<std::string> ResourceManager::ResourcesToLoad = std::set<std::string>();
fs::path ResourceManager::rootPath = fs::path("");
fs::path ResourceManager::cachePath = fs::path("");
// ResourceManager::rootPath = fs::path;
void ResourceManager::init() {
    // char filename[255];
    // GetModuleFileNameA(NULL, filename, 255);
    // rootPath = fs::path(filename).parent_path();
    // cachePath = fs::path(filename).parent_path() / fs::path("cache");
    // rootPath = fs::path(filename).parent_path();
    cachePath = fs::path("cache");
}
MovieClip* ResourceManager::getMovieClip(const std::string& file, const std::string& name) {
    return getMovieClip(file.c_str(), name.c_str());
}
MovieClip* ResourceManager::getMovieClip(const char* file, const char* name) {
    // printf("getting %s", name);
    SupercellSWF* swf = getSupercellSWF(file, name);
    MovieClipOriginal* movieClip;
    {
        for (unsigned short i = 0;i < swf->exportNameSize;i++) {
            if (strcmp(swf->exportNameStrings[i], name) == 0) {
                // printf("found\n");
                movieClip = swf->getOriginalMovieClip(swf->exportNameIds[i]);
                goto find;
            }
        }
        abort();
    find:;
    }
    return MovieClip::createMovieClip(movieClip, swf);
}
GLImage* ResourceManager::getImage(const std::string& file) {
    auto it = Resources.find(file);
    if (it != Resources.end()) return (GLImage*)it->second;
    else {
        addFile(file.c_str());
        while (resourceToLoad())
            loadNextResource();
        return (GLImage*)(*Resources.find(file)).second;
    }
}
void ResourceManager::addFile(const char* file) {
    auto s = std::string(file);
    if (Resources.find(s) != Resources.end()) return;
    if (ResourcesToLoad.find(s) != ResourcesToLoad.end()) return;
    ResourcesToLoad.insert(file);
}
void ResourceManager::loadNextResource() {
    if (ResourcesToLoad.size() == 0) return;
    std::string file = *ResourcesToLoad.begin();
    ResourcesToLoad.erase(file);
    auto extension = fs::path(file).extension();
    if (extension == ".sc") {
        SupercellSWF* swf = new SupercellSWF();
        unsigned long long NativeTime = xTimer::getNativeTime();
        fs::path cacheFilePath = fs::absolute(cachePath / fs::path("swfcache") / file);
        if (!fs::exists(cacheFilePath)) {
            fs::create_directories(cacheFilePath.parent_path());
            ScDowngrade((char*)fs::absolute(rootPath / fs::path("assets") / file).generic_string().c_str(), (char*)cacheFilePath.generic_string().c_str(), 9339.0f);
        }
        swf->loadInternal(cacheFilePath);
        printf("Loading SC %s took %d ms\n", file.c_str(), (int)xTimer::getPassedTimeMs(NativeTime, xTimer::getNativeTime()));
        Resources[file] = swf;
    }
    else if (extension == ".sctx") {
        GLImage* image = new GLImage();
        std::vector<const sc::texture::SCTX::MipMap*> mipmaps;

        std::ifstream is(fs::path("assets") / "sc3d" / file, std::ios_base::binary);
        is.seekg(0, std::ios::end);
        size_t length = is.tellg();
        is.seekg(0, std::ios::beg);

        unsigned int textureSize;
        is.read((char*)&textureSize, 4);
        char* buf = new char[textureSize];

        is.read(buf, textureSize);
        flatbuffers::Verifier verifier((const uint8_t*)buf, textureSize);
        if (!sc::texture::SCTX::VerifyTextureDataBuffer(verifier)) abort();

        auto* texture = sc::texture::SCTX::GetTextureData(buf);

        unsigned int mipMapDataSize;
        is.read((char*)&mipMapDataSize, 4);

        int loadedDataLength = 0;
        std::vector<int> levels;
        for (int i = 0;i < texture->levels_count();i++) {
            unsigned int mipMapDataSize;
            is.read((char*)&mipMapDataSize, 4);
            const unsigned char* buf = new unsigned char[mipMapDataSize];
            is.read((char*)buf, mipMapDataSize);
            auto mipMap = sc::texture::SCTX::GetMipMap(buf);
            mipmaps.push_back(mipMap);
            // printf("mipMap #%d offset: %d\n", i, mipMap->offset());
            // levels.push_back(mipMap->offset());
            loadedDataLength += mipMapDataSize + 4;
            // delete[] buf;
        }
        if (loadedDataLength != mipMapDataSize) abort();

        if (texture->flags() & sc::texture::SCTX::TextureFlags::TextureFlags_use_padding) {
            is.seekg(((int)is.tellg() + 15) & ~15, std::ios::beg);
        }

        int compressedSize = length - is.tellg();
        unsigned char* compressed = new unsigned char[compressedSize];
        is.read((char*)compressed, compressedSize);
        is.close();
        int decompressedSize = ZSTD_getFrameContentSize(compressed, compressedSize);
        unsigned char* decompressed = new unsigned char[decompressedSize];
        ZSTD_decompress(decompressed, decompressedSize, compressed, compressedSize);
        image->createWithFormat(texture, mipmaps, decompressed);
        delete[] compressed;
        delete[] decompressed;
        for (auto i : mipmaps) {
            delete[] flatbuffers::GetBufferStartFromRootPointer(i);
        }
        delete[] buf;

        Resources[file] = image;
    }
    else if (extension == ".scw") {
        std::ifstream is(fs::path("assets") / "sc3d" / file, std::ios_base::binary);
        is.seekg(0, std::ios::end);
        size_t length = is.tellg();
        is.seekg(0, std::ios::beg);

        unsigned char* buf = new unsigned char[length];
        is.read((char*)buf, length);
        is.close();

        SCW::File* scwFile = new SCW::File(buf, length);

        if (!scwFile->LoadSCglTF()) {
            delete scwFile;
            scwFile = new SCW::File(buf, length);
            scwFile->Load();
        }
        Resources[file] = scwFile;
    }
    else abort();

}
bool ResourceManager::resourceToLoad() {
    return ResourcesToLoad.size() != 0;
}
SupercellSWF* ResourceManager::getSupercellSWF(const char* name, const  char* needby) {
    auto s = std::string(name);
    auto it = Resources.find(s);
    if (it != Resources.end()) return (SupercellSWF*)it->second;
    abort();
    return nullptr;
}

SCW::File* ResourceManager::getSC3D(const std::string& file) {
    auto it = Resources.find(file);
    if (it != Resources.end()) return (SCW::File*)it->second;
    else {
        addFile(file.c_str());
        while (resourceToLoad())
            loadNextResource();
        return (SCW::File*)(*Resources.find(file)).second;
    }
}
