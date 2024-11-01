#pragma once
#include<ResourceManager.h>
#include <xTimer.h>
#include <flash/objects/ExportName.h>
#include <flash/display_object/MovieClipOriginal.h>
#include <flash/objects/SupercellSWF.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <compression/compression.h>
#include "flash/types/SWFStream.hpp"

namespace fs = std::filesystem;

std::vector<std::pair<char*, void*>> ResourceManager::Resources = std::vector<std::pair<char*, void*>>();
std::set<char*> ResourceManager::ResourcesToLoad = std::set<char*>();
fs::path ResourceManager::rootPath = fs::path("");
fs::path ResourceManager::cachePath = fs::path("");
// ResourceManager::rootPath = fs::path;
void ResourceManager::init() {
    char filename[255];
    GetModuleFileNameA(NULL, filename, 255);
    rootPath = fs::path(filename).parent_path();
    cachePath = fs::path(filename).parent_path() / fs::path("cache");
}

MovieClip* ResourceManager::getMovieClip(char* file, char* name) {
    // printf("getting %s", name);
    sc::flash::SupercellSWF* swf = getSupercellSWF(file, name);
    sc::flash::MovieClipOriginal* movieClip;
    {
        for (sc::flash::ExportName& export_name : swf->exports) {
            if (export_name.name == name) {
                // printf("found\n");
                movieClip = &swf->getOriginalMovieClip(export_name.id, nullptr);
                goto find;
            }
        }
        throw new sc::Exception("not found");
    find:;
    }
    return MovieClip::createMovieClip(movieClip, swf);
}
void ResourceManager::addFile(char* file) {
    ResourcesToLoad.insert(file);
    // sc::flash::SupercellSWF* swf = new sc::flash::SupercellSWF();;
    // swf->load(fs::absolute(rootPath / fs::path("assets") / file));
    // printf("%s loaded:\nShapes Count:%d\ntextures Count:%d\nmovieclips Count:%d\ntextfields Count:%d\n", file, swf->shapes.size(), swf->textures.size(), swf->movieclips.size(), swf->textfields.size());
    // Resources.push_back(std::pair<char*, void*>(file, swf));
}
void ResourceManager::loadNextResource() {
    if (ResourcesToLoad.size() == 0) return;
    char* file = *ResourcesToLoad.begin();
    ResourcesToLoad.erase(file);
    sc::flash::SupercellSWF* swf = new sc::flash::SupercellSWF();
    unsigned long long NativeTime = xTimer::getNativeTime();
    fs::path cacheFilePath = fs::absolute(cachePath / fs::path("swfcache") / file);
    if (!fs::exists(cacheFilePath)) {
        fs::create_directories(cacheFilePath.parent_path());

        sc::flash::SWFStream stream;
        fs::path scPath = fs::absolute(rootPath / fs::path("assets") / file);
        stream.open_file(scPath);
        stream.save_file(cacheFilePath, static_cast<sc::compression::flash::Signature>(-1));//raw

        fs::path common_file_path = scPath.parent_path() / scPath.stem().concat("_tex.sc");
        if (fs::exists(common_file_path)) {
            stream.open_file(common_file_path);
            stream.save_file(cacheFilePath.parent_path() / cacheFilePath.stem().concat("_tex.sc"), static_cast<sc::compression::flash::Signature>(-1));//raw
        }
    }
    swf->load(cacheFilePath);
    printf("Loading SC %s took %d ms\n", file, (int)xTimer::getPassedTimeMs(NativeTime, xTimer::getNativeTime()));
    // printf("%s loaded:\nShapes Count:%d\ntextures Count:%d\nmovieclips Count:%d\ntextfields Count:%d\n", file, swf->shapes.size(), swf->textures.size(), swf->movieclips.size(), swf->textfields.size());
    Resources.push_back(std::pair<char*, void*>(file, swf));
}
bool ResourceManager::resourceToLoad() {
    return ResourcesToLoad.size() != 0;
}
sc::flash::SupercellSWF* ResourceManager::getSupercellSWF(char* name, char* needby) {
    for (std::pair<char*, void*> p : Resources) {
        if (strcmp(p.first, name) == 0) return (sc::flash::SupercellSWF*)p.second;
    }
    throw new sc::Exception("Trying to load export name '' from resource which is not yet loaded: ''");
    return nullptr;
}