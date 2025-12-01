
#pragma once

#include <cstdint>
#include <list>

#include "vehicle.hpp"

static mt19937 rng;

class Datamodel
{
  private:
    FILE *file;

    Model *global;

    list<Vehicle> active;

  public:
    Datamodel(const char *model, const char *training);

    int load(const char *path);

    int write(const char *path, Measurement data);

    Vehicle *get_random_event();

    inline int64_t get_timestemp();
};
