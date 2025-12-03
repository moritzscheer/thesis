
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>

#include "model.hpp"

using namespace std;

Model::Model(const int features, const double frequency, const int num_frequency, const double learning_rate)
    : features(features), frequency(frequency), num_frequency(num_frequency), learning_rate(learning_rate)
{
    const int total_features = features * 2 * num_frequency + 1;
    weights.resize(total_features, 0.0);
}

Model *Model::load(std::string &path)
{
    FILE *file = fopen(path.data(), "r+");

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

/*
 *
 */
vector<double> Model::compute_fourier_features(const vector<double> &inputs, double task_selector) const
{
    vector<double> features;
    features.reserve(features * 2 * num_frequency + 1);

    for (double input_value : inputs)
    {
        for (int i = 1; i <= num_frequency; i++)
        {
            double frequency = i * frequency;
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

    for (size_t i = 0; i < weights.size(); i++)
    {
        sum += weights.at(i) * features.at(i);
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
    for (size_t i = 0; i < weights.size(); i++)
    {
        prediction += weights[i] * features[i];
    }

    double error = target - prediction;

    for (size_t i = 0; i < weights.size(); i++)
    {
        weights[i] += learning_rate * error * features[i];
    }
}
