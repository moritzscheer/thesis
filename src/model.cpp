
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>

#include "model.hpp"

using namespace std;

Model::Model(const int features, const double frequency, const int num_frequency, const double learning_rate)
    : FEATURES(features), FREQUENCY(frequency), NUM_FREQUENCY(num_frequency), LEARNING_RATE(learning_rate)
{
    const int total_features = features * 2 * num_frequency + 1;
    WEIGHTS.resize(total_features, 0.0);
}

Model *Model::load(const char *path)
{
    FILE *file = fopen(path, "r+");

    if (!file)
    {
        printf("Could not open file");
        return nullptr;
    }

    Model *model;

    size_t nread = fread(model, sizeof(Model), 1, file);
    if (nread < 0)
    {
        printf("Could not read file");
        errno = nread;
    }

    int res = fclose(file);
    if (res != 0)
    {
        printf("Could not close file");
        errno = res;
    }

    return 0;
}

int Model::safe(Model *model, const char *path)
{
    FILE *file = fopen(path, "w+");

    if (!file)
    {
        printf("Could not open file");
        return -errno;
    }

    size_t nwrite = fwrite(model, sizeof(Model), 1, file);
    if (nwrite != 1)
    {
        printf("Could not write to file");
        return (int)nwrite;
    }

    int res = fclose(file);
    if (res != 0)
    {
        printf("Could not close file");
        return res;
    }

    return 0;
}

inline void Model::train_iterations(Model *model, const int count)
{
    mt19937 rng(42);
    uniform_real_distribution<double> soc_dist(0.1, 0.9);
    uniform_real_distribution<double> time_dist(0.0, 1.0);
    uniform_real_distribution<double> power_dist(5.0, 50.0);
    uniform_real_distribution<double> temp_dist(10.0, 40.0);

    int rest = count;

    for (int i = 0; i < count; i++)
    {
        double soc         = soc_dist(rng);
        double time        = time_dist(rng);
        double power       = power_dist(rng);
        double temperature = temp_dist(rng);

        const double target_time_to_soc = (0.8 - soc) * 3.0 - power * 0.01;
        const double target_future_soc  = soc + time * power * 0.0005;

        model->update({soc, time, power, temperature}, 0.0,
                      target_time_to_soc);                                      // Task 0: predict time to target SoC
        model->update({soc, time, power, temperature}, 1.0, target_future_soc); // Task 1: predict future SoC

        printf("%d\n", rest--);
    }
}

/*
 *
 */
vector<double> Model::compute_fourier_features(const vector<double> &inputs, double task_selector) const
{
    vector<double> features;
    features.reserve(FEATURES * 2 * NUM_FREQUENCY + 1);

    for (double input_value : inputs)
    {
        for (int i = 1; i <= NUM_FREQUENCY; i++)
        {
            double frequency = i * FREQUENCY;
            features.push_back(cos(2.0 * M_PI * frequency * input_value));
            features.push_back(sin(2.0 * M_PI * frequency * input_value));
        }
    }

    features.push_back(task_selector);
    return features;
}

/*
 *
 */
double Model::predict(const vector<double> &inputs, double task_selector) const
{
    auto features = compute_fourier_features(inputs, task_selector);

    double sum = 0.0;

    for (size_t i = 0; i < WEIGHTS.size(); i++)
    {
        sum += WEIGHTS.at(i) * features.at(i);
    }

    return sum;
}

/*
 *
 */
void Model::update(const vector<double> &inputs, double task_selector, double target)
{
    const auto features = compute_fourier_features(inputs, task_selector);
    double prediction   = 0.0;
    for (size_t i = 0; i < WEIGHTS.size(); i++)
    {
        prediction += WEIGHTS[i] * features[i];
    }

    double error = target - prediction;

    for (size_t i = 0; i < WEIGHTS.size(); i++)
    {
        WEIGHTS[i] += LEARNING_RATE * error * features[i];
    }
}
