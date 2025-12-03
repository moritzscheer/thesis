
#include "vehicle.hpp"

Vehicle::Vehicle(int64_t uid, Model *model) : uid()
{
}

double Vehicle::predict_soc(double time_delta) const
{
    std::vector<double> data;
    data.reserve(4);

    data.emplace_back(soc_current);
    data.emplace_back(timestemp);
    data.emplace_back(power_current);
    data.emplace_back(time_delta);

    return predict(data, 1);
}

double Vehicle::predict_time(double next_soc) const
{
    std::vector<double> data;
    data.reserve(4);

    data.emplace_back(soc_current);
    data.emplace_back(timestemp);
    data.emplace_back(power_current);
    data.emplace_back(next_soc);

    return predict(data, 0);
}

int Vehicle::write(Evaluation result)
{
    return 0;
}
