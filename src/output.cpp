
#include <sstream>
#include <string>
#include <vector>

#include "model.hpp"
#include "output.hpp"
#include "vehicle.hpp"

Model *import_model(std::string &path)
{
    FILE *file = fopen(path.data(), "r");

    if (!file)
    {
        printf("Could not open file");
        return nullptr;
    }

    char buffer[4096];

    if (!fgets(buffer, sizeof(buffer), file))
    {
        return nullptr;
    }

    std::istringstream ss(buffer);
    std::string token;

    if (!std::getline(ss, token, ';'))
    {
        return nullptr;
    }

    double features = std::stoi(token);

    if (!std::getline(ss, token, ';'))
    {
        return nullptr;
    }

    double frequency = std::stod(token);

    if (!std::getline(ss, token, ';'))
    {
        return nullptr;
    }

    double num_frequency = std::stoi(token);

    if (!std::getline(ss, token, ';'))
    {
        return nullptr;
    }

    double learning_rate = std::stod(token);

    if (!std::getline(ss, token, ';'))
    {
        return nullptr;
    }

    size_t num_weights = std::stod(token);

    if (!fgets(buffer, sizeof(buffer), file))
    {
        return nullptr;
    }

    if (!fgets(buffer, sizeof(buffer), file))
    {
        return nullptr;
    }

    std::vector<double> weights;

    while (std::getline(ss, token, ';'))
    {
        if (!token.empty())
        {
            weights.push_back(std::stod(token));
        }
    }

    if ((int)weights.size() != num_weights)
    {
        return nullptr;
    }

    return new Model(weights, features, frequency, num_frequency, learning_rate);
}

void print_model(Model *model)
{
    std::ostringstream output;

    output << "--------------------------------------------------------------\n";
    output << "features; frequency; num_frequency; learning_rate; num_weights\n";
    output << model->features << "; " << model->frequency << "; " << model->num_frequency << "; "
           << model->learning_rate << "; " << model->weights.size();

    for (double w : model->weights)
    {
        output << "; " << w;
    }

    output << "\n";

    print(output);
}

void write_evaluation(Evaluation &eval, Vehicle *vehicle)
{
    std::ostringstream output;

    if (vehicle->state == START)
    {
        std::string uid = to_string(vehicle->uid);

        output << "\n";
        output << "-----------------------------------------------------------------------------\n";
        output << "Vehicle: " << uid << "\n";
        output << "timestemp; SoC; Power; prediction SoC; Error SoC; prediction Time; Error Time\n";
        output << "-----------------------------------------------------------------------------\n";
    }

    output << eval.timestemp << "; " << eval.soc << "; " << eval.power << "; " << eval.soc_prediction << "; "
           << eval.soc_error_rate << "; " << eval.time_prediction << "; " << eval.time_error_rate;

    print(output);

    if (vehicle->state == END)
    {
        print_model(vehicle);
    }
}

static void print(std::ostringstream &oss)
{
    std::string message = oss.str();

    if (output_file)
    {
        fprintf(output_file, "%s", message.c_str());
    }
    else
    {
        printf("%s", message.c_str());
    }
}
