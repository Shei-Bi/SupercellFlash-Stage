#pragma once
#include<flash/MovieClip.h>
#include<flash/objects/SupercellSWF.h>
#include <set>

class ResourceManager
{
public:
    static std::filesystem::path rootPath;
    static std::filesystem::path cachePath;
    static std::vector<std::pair<char*, void*>> Resources;
    static std::set<char*> ResourcesToLoad;
    static void init();
    static MovieClip* getMovieClip(char* file, char* name);
    static void addFile(char* file);
    static sc::flash::SupercellSWF* getSupercellSWF(char* name, char* needby);
    static void loadNextResource();
    static bool resourceToLoad();
};
