
#pragma once

#include "model.cpp"
#include <queue>

struct Measurement
{
    double soc;
    double power;
    int64_t timestemp;

    double soc_prediction;
    int64_t time_prediction;
};

class Vehicle : Model
{
  private:
    FILE *file;

    int vehicle_number;

    double soc_current;

    double power_current;

    long timestemp;

    std::queue<Measurement> next;

  public:
    Vehicle(vector<double> weights);

    bool update_measurements();

    double simulate_soc();

    double simulate_power();
};
