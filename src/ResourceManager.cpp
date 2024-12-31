#pragma once
#include <ResourceManager.h>
#include <xTimer.h>
#include <flash/MovieClipOriginal.h>
#include <flash/SupercellSWF.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "ScDowngrade.h"

namespace fs = std::filesystem;

std::vector<std::pair<const char*, void*>> ResourceManager::Resources = std::vector<std::pair<const char*, void*>>();
std::set<const char*> ResourceManager::ResourcesToLoad = std::set<const char*>();
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
void ResourceManager::addFile(const char* file) {
    for (std::pair<const char*, void*> p : Resources) {
        if (strcmp(p.first, file) == 0) return;
    }
    for (const char* p : ResourcesToLoad) {
        if (strcmp(p, file) == 0) return;
    }
    ResourcesToLoad.insert(file);
    // sc::flash::SupercellSWF* swf = new sc::flash::SupercellSWF();;
    // swf->load(fs::absolute(rootPath / fs::path("assets") / file));
    // printf("%s loaded:\nShapes Count:%d\ntextures Count:%d\nmovieclips Count:%d\ntextfields Count:%d\n", file, swf->shapes.size(), swf->textures.size(), swf->movieclips.size(), swf->textfields.size());
    // Resources.push_back(std::pair<char*, void*>(file, swf));
}
void ResourceManager::loadNextResource() {
    if (ResourcesToLoad.size() == 0) return;
    const  char* file = *ResourcesToLoad.begin();
    ResourcesToLoad.erase(file);
    SupercellSWF* swf = new SupercellSWF();
    unsigned long long NativeTime = xTimer::getNativeTime();
    fs::path cacheFilePath = fs::absolute(cachePath / fs::path("swfcache") / file);
    if (!fs::exists(cacheFilePath)) {
        fs::create_directories(cacheFilePath.parent_path());
        ScDowngrade((char*)fs::absolute(rootPath / fs::path("assets") / file).generic_string().c_str(), (char*)cacheFilePath.generic_string().c_str(), 9339.0f);
    }
    swf->loadInternal(cacheFilePath);
    printf("Loading SC %s took %d ms\n", file, (int)xTimer::getPassedTimeMs(NativeTime, xTimer::getNativeTime()));
    // printf("%s loaded:\nShapes Count:%d\ntextures Count:%d\nmovieclips Count:%d\ntextfields Count:%d\n", file, swf->shapes.size(), swf->textures.size(), swf->movieclips.size(), swf->textfields.size());
    Resources.push_back(std::pair<const char*, void*>(file, swf));
}
bool ResourceManager::resourceToLoad() {
    return ResourcesToLoad.size() != 0;
}
SupercellSWF* ResourceManager::getSupercellSWF(const char* name, const  char* needby) {
    for (std::pair<const char*, void*> p : Resources) {
        if (strcmp(p.first, name) == 0) return (SupercellSWF*)p.second;
    }
    abort();
    return nullptr;
}