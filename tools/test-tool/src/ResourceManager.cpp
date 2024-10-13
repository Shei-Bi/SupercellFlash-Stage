#pragma once
#include<ResourceManager.h>
#include <flash/objects/ExportName.h>
#include <flash/display_object/MovieClipOriginal.h>
#include <flash/objects/SupercellSWF.h>
#include <stdio.h>
namespace fs = std::filesystem;
std::vector<std::pair<char*, void*>> ResourceManager::Resources = std::vector<std::pair<char*, void*>>();
MovieClip* ResourceManager::getMovieClip(char* file, char* name) {
    printf("getting %s", name);
    sc::flash::SupercellSWF* swf = getSupercellSWF(file, name);
    sc::flash::MovieClipOriginal* movieClip;
    {
        for (sc::flash::ExportName& export_name : swf->exports) {
            if (export_name.name == name) {
                printf("found\n");
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
    sc::flash::SupercellSWF* swf = new sc::flash::SupercellSWF();;
    swf->load(fs::absolute(fs::path("assets") / file));
    printf("%s loaded:\nShapes Count:%d\ntextures Count:%d\nmovieclips Count:%d\ntextfields Count:%d\n", file, swf->shapes.size(), swf->textures.size(), swf->movieclips.size(), swf->textfields.size());
    Resources.push_back(std::pair<char*, void*>(file, swf));
}
sc::flash::SupercellSWF* ResourceManager::getSupercellSWF(char* name, char* needby) {
    for (std::pair<char*, void*> p : Resources) {
        if (strcmp(p.first, name) == 0) return (sc::flash::SupercellSWF*)p.second;
    }
    throw new sc::Exception("Trying to load export name '' from resource which is not yet loaded: ''");
    return nullptr;
}