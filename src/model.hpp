
#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <cmath>

using namespace std;

struct Model
{
    /* Number of the input vector */

    int features;

    /* Base frequency for Fourier Features */

    double frequency;

    /* Number of Fourier Features per input */

    int num_frequency;

    /* Least mean square learning rate */

    double learning_rate;

    /* Weight vector */

    vector<double> weights;

    Model(const int features = 64, const double frequency = 0.25, const int num_frequency = 8,
          const double learning_rate = 0.1);

    Model(std::vector<double> weights, const int features = 64, const double frequency = 0.25,
          const int num_frequency = 8, const double learning_rate = 0.1);

    Model *operator=(Model *base);

    static Model *load(std::string &path);

    static int safe(Model *model, const char *path);

    vector<double> compute_fourier_features(const vector<double> &inputs, double task_selector) const;

    double predict(const vector<double> &inputs, double task_selector) const;

    void update(const vector<double> &inputs, double task_selector, double target);
};
