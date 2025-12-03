
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <exception>
#include <getopt.h>
#include <string>

#include "datamodel.hpp"
#include "output.hpp"
#include "training.hpp"

using namespace std;

DataModel::DataModel(int argc, char *argv[])
{
    string training_path = nullptr;
    string source_path   = nullptr;
    string dest_path     = nullptr;

    int opt;

    while ((opt = getopt(argc, argv, "f:n:")) != -1)
    {
        switch (opt)
        {
        case 's':
        {
            source_path = optarg;
            break;
        }
        case 'd':
        {
            dest_path = optarg;
            break;
        }
        case 't':
        {
            training_path = optarg;
            break;
        }
        }
    }

    //
    // 1. Open CSV file to retrieve measurement data.
    //

    if (!training_path.empty())
    {
        measurements = read_measurements(training_path);
    }

    if (measurements.empty())
    {
        measurements = generate_measurements();
    }

    //
    // 2. Open an existing model file to retrieve safed Model.
    //

    if (!source_path.empty())
    {
        global = import_model(source_path);
    }

    if (!global)
    {
        global = new Model(4, 0.25, 64, 0.01);
    }

    //
    // 3. Open model file to safe data into.
    //

    if (dest_path.empty())
    {
        dest_path = "./model_" + timestemp() + ".csv";
    }

    output_file = fopen(dest_path.c_str(), "w");

    if (!output_file)
    {
        printf("Unable to open file \"%s\" for output: %s\n", dest_path.c_str(), strerror(errno));
        throw exception();
    }

    current_measurement = 0;
}

DataModel::~DataModel()
{
    if (output_file)
    {
        fclose(output_file);
        printf("Unable to open file \"%s\" for output: %s\n", dest_path.c_str(), strerror(errno));
    }
}

Vehicle *DataModel::wait_measurement()
{
    size_t &index = current_measurement;

    if (index >= measurements.size())
    {
        return nullptr;
    }

    index++;

    RawData *measure = &measurements[index];

    Vehicle *vehicle = get_vehicle(measure->uid);

    if (!vehicle)
    {
        auto &uid    = measure->uid;
        Model *local = global;

        vehicle = new Vehicle(uid, local);

        add_vehicle(vehicle);
    }

    vehicle->timestemp     = measure->timestemp;
    vehicle->soc_current   = measure->soc;
    vehicle->power_current = measure->power;

    return vehicle;
}

double DataModel::get_next_prediction_time(int64_t uid)
{
    size_t index = current_measurement + 1;

    if (index >= measurements.size())
    {
        return 0;
    }

    for (size_t i = index; i < measurements.size(); ++i)
    {
        if (measurements[i].uid == uid)
        {
            return (double)measurements[index].timestemp;
        }
    }

    return 0;
}

double DataModel::get_next_prediction_soc(int64_t uid)
{
    size_t index = current_measurement + 1;

    if (index >= measurements.size())
    {
        return 0;
    }

    for (size_t i = index; i < measurements.size(); ++i)
    {
        if (measurements[i].uid == uid)
        {
            return (double)measurements[index].soc;
        }
    }

    return 0;
}

Vehicle *DataModel::get_vehicle(int64_t uid)
{
    Vehicle *vehicle = nullptr;

    auto it = active.find(uid);
    if (it != active.end())
    {
        vehicle = it->second;
    }

    return vehicle;
}

void DataModel::add_vehicle(Vehicle *vehicle)
{
    active.emplace(vehicle->uid, vehicle);
}

Model *DataModel::import_model(std::string path)
{
}

void DataModel::write_model(Model *model)
{
}

void DataModel::write_evaluation(Evaluation &eval)
{
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
