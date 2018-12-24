#include "ioUtils.h"
#include <SDL.h>

std::stringstream readFile(const std::string& path) {
    std::stringstream ss;
    SDL_RWops *rw = SDL_RWFromFile(path.c_str(), "r");

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
