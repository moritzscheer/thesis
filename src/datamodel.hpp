
#pragma once

#include <cstdint>
#include <unordered_map>

#include "model.hpp"
#include "training.hpp"
#include "vehicle.hpp"

class DataModel
{
  private:
    Model *global;

    std::vector<RawData> measurements;

    size_t current_measurement;

    std::unordered_map<int64_t, Vehicle *> active;

  public:
    DataModel(int argc, char *argv[]);

    ~DataModel();

    Vehicle *wait_measurement();

    double get_next_prediction_time(int64_t uid);

    double get_next_prediction_soc(int64_t uid);

    Vehicle *get_vehicle(int64_t uid);

    void add_vehicle(Vehicle *vehicle);

    std::string timestemp();
};
