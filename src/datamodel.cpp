
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "datamodel.hpp"
#include "utils.hpp"
#include "vehicle.hpp"

Datamodel::Datamodel(const char *model_path, const char *data_path)
{
    if (model_path)
    {
        global = Model::load(model_path);
    }

    if (!global)
    {
        global = new Model(4, 0.25, 64, 0.01);
    }

    if (data_path)
    {
        file = fopen(data_path, "r");

        if (!file)
        {
            fprintf(stdout, "Unable to open file: %s.\n", data_path);
            throw exception();
        }

        if (!csv_valid(file))
        {
            fprintf(stdout, "CSV file uses wrong schema. Use the converter tool to convert to correct format.\n");
            fprintf(stdout, "Correct format is: timestemp(int64_t),soc(double),.\n");
            throw exception();
        }
    }
}

int Datamodel::load(const char *path)
{
    file = fopen(path, "r");
    if (!file)
    {
        std::cout << "Unable to open File " << path << std::endl;
        return -errno;
    }

    int res = parse_csv_lines(file, data);
    if (res != 0)
    {
        return res;
    }

    return data;
}

Vehicle *Datamodel::get_random_event()
{
    uniform_int_distribution<int> event_dist(0, 5);
    int event = event_dist(rng);

    Vehicle *vehicle;

    if (event == 1)
    {
        Vehicle &vehicle = reserved.front();
        reserved.pop_front();
        active.push_front(vehicle);
    }
}

int Datamodel::write(const char *path, Measurement data)
{
    FILE *file = fopen(path, "a");
    if (!file)
    {
        std::cout << "Unable to open File " << path << std::endl;
        return -errno;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);

    if (size == 0)
    {
        fprintf(file, "time,soc\n");
    }
    else
    {
        fprintf(file, "%f,%ld\n", data.soc, data.timestemp);
    }

    fclose(file);
    return 0;
}

int64_t get_timestemp()
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
    {
        return -errno;
    }

    return ts.tv_sec;
}
