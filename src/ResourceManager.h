#pragma once
#include<flash/MovieClip.h>
#include "flash/SupercellSWF.h"
#include <set>

class ResourceManager
{
public:
    static std::filesystem::path rootPath;
    static std::filesystem::path cachePath;
    static std::vector<std::pair<const char*, void*>> Resources;
    static std::set<const char*> ResourcesToLoad;
    static void init();
    static MovieClip* getMovieClip(const char* file, const char* name);
    static void addFile(const char* file);
    static SupercellSWF* getSupercellSWF(const char* name, const char* needby);
    static void loadNextResource();
    static bool resourceToLoad();
};
