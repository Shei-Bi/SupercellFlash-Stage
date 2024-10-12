#pragma once
#include<flash/MovieClip.h>
#include<flash/objects/SupercellSWF.h>

class ResourceManager
{
public:
    static std::vector<std::pair<char*, void*>> Resources;
    static MovieClip* getMovieClip(char* file, char* name);
    static void addFile(char* file);
    static sc::flash::SupercellSWF* getSupercellSWF(char* name, char* needby);
};
