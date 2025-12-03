
#pragma once

#include "model.hpp"

enum charging_state
{
    START,
    CHARGING,
    END,
};

struct Vehicle : Model
{
  private:
    FILE *file;

  public:
    int uid;

    double soc_current;

    double power_current;

    long timestemp;

    charging_state state;

    Vehicle(int64_t uid, Model *model);

    double predict_soc(double time_delta) const;

    double predict_time(double next_soc) const;

    double error_rate(double prediction);
};
