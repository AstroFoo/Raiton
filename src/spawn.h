#ifndef SPAWN_H
#define SPAWN_H

#include <string>

struct Spawn
{
    float time; // seconds
    float x;    // between 0 and 1 (fraction of screen width)
    std::string id; // which enemy to spawn
};

#endif