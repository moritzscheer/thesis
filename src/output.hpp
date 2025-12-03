
#pragma once

#include <cstdint>
#include <string>

#include "model.hpp"

static FILE *output_file = nullptr;

struct Evaluation
{
    double soc;
    double power;
    int64_t timestemp;

    double soc_prediction;
    double soc_error_rate;

    double time_prediction;
    double time_error_rate;
};

Model *import_model(std::string &path);

void print_model(Model *model);

void print_evaluation(Evaluation &eval);

static void print(std::ostringstream &out);
