#include "ioUtils.h"
#include "constants.h"
#include <SDL.h>

std::string pathWithPrefDir(const std::string& path, const std::string& org, const std::string& app)
{
    std::stringstream pathWithPrefDir;
    char* prefDir = SDL_GetPrefPath(org.c_str(), app.c_str());
    pathWithPrefDir << prefDir << path;

    SDL_free(prefDir);
    return pathWithPrefDir.str();
}

std::stringstream readFile(const std::string& path, bool usePrefDir)
{
    std::string filePath{path};
    if (usePrefDir)
        filePath = pathWithPrefDir(path, ORG_NAME, APP_NAME);

    SDL_RWops *rw = SDL_RWFromFile(filePath .c_str(), "r");

    std::stringstream ss;
    // Cannot open the file
    if (rw == nullptr) {
        ss.clear(std::ios::badbit);
        return ss;
    }

    Sint64 res_size = SDL_RWsize(rw);
    char* res = (char*)malloc(res_size + 1);

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
            nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
            nb_read_total += nb_read;
            buf += nb_read;
    }
    SDL_RWclose(rw);

    // Did not read the whole file
    if (nb_read_total != res_size) {
        ss.clear(std::ios::badbit);
        return ss;
    }

    res[nb_read_total] = '\0';
    ss << res;
    free(res);
    return ss;
}

bool writeFile(const std::string& path, const std::string& content, bool usePrefDir)
{
    std::string filePath = path;
    if (usePrefDir)
        filePath = pathWithPrefDir(path, ORG_NAME, APP_NAME);

    SDL_RWops* rw = SDL_RWFromFile(filePath.c_str(), "w");
    if (rw == nullptr) {
        SDL_Log("Cannot write to %s", filePath.c_str());
        return false;
    }
    SDL_RWwrite(rw, content.c_str(), sizeof(char), content.length());
    SDL_RWclose(rw);
    return true;
}

