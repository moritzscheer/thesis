
#include "vehicle.hpp"
#include <algorithm>
#include <random>

Vehicle::Vehicle(vector<double> weights)
{
    WEIGHTS = weights;

    vehicle_number = vehicle_number_dist(rng);
    soc_current    = soc_dist(rng);
    power_current  = 0;

    int64_t tmp = get_timestemp();
    if (tmp < 0)
    {
        return;
    }

    timestemp = tmp;
}

bool Vehicle::update_measurements()
{
    timestemp     = get_timestemp();
    soc_current   = simulate_soc();
    power_current = simulate_power();

    return false;
}

double Vehicle::simulate_soc()
{
    double rate = 0.02 * (1.0 - soc_current);
    std::normal_distribution<double> noise(0.0, 0.001);

    double soc_next = soc_current + rate + noise(rng);
    return std::clamp(soc_next, 0.0, 1.0);
}

double Vehicle::simulate_power()
{
    const double P_max = 120.0;

    double power = P_max * (1.0 - soc_current) * (1.0 - 0.3 * soc_current);

    std::normal_distribution<double> noise(0.0, 2.0); // ±2 kW jitter
    power += noise(rng);

    return std::max(power, 0.0);
}
