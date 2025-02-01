#pragma once
#include<flash/MovieClip.h>
#include "flash/SupercellSWF.h"
#include <set>
#include <string>
#include "GLImage.h"
#include <map>

class ResourceManager
{
public:
    static std::filesystem::path rootPath;
    static std::filesystem::path cachePath;
    static std::map<std::string, void*> Resources;
    static std::set<std::string> ResourcesToLoad;
    static void init();
    static MovieClip* getMovieClip(const std::string& file, const std::string& name);
    static MovieClip* getMovieClip(const char* file, const char* name);
    static GLImage* getImage(const std::string& file);
    static void addFile(const char* file);
    static SupercellSWF* getSupercellSWF(const char* name, const char* needby);
    static void loadNextResource();
    static bool resourceToLoad();
};
