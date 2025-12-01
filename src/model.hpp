
#pragma once

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <cmath>

using namespace std;

struct Model
{
    /* Number of the input vector */

    int FEATURES;

    /* Base frequency for Fourier Features */

    double FREQUENCY;

    /* Number of Fourier Features per input */

    int NUM_FREQUENCY;

    /* Least mean square learning rate */

    double LEARNING_RATE;

    /* Weight vector */

    vector<double> WEIGHTS;

    Model(const int features = 64, const double frequency = 0.25, const int num_frequency = 8,
          const double learning_rate = 0.1);

    static Model *load(const char *path);

    static int safe(Model *model, const char *path);

    inline void train_iterations(Model *model, const int count);

    vector<double> compute_fourier_features(const vector<double> &inputs, double task_selector) const;

    double predict(const vector<double> &inputs, double task_selector) const;

    void update(const vector<double> &inputs, double task_selector, double target);
};
