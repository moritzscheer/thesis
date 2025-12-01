
#include "datamodel.hpp"
#include "model.hpp"
#include "vehicle.hpp"
#include <cstdlib>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *model_path;
    const char *training_path;

    int opt;

    while ((opt = getopt(argc, argv, "f:n:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            printf("File: %s\n", optarg);
            model_path = optarg;
            break;
        case 'i':
            printf("Training data path: %s\n", optarg);
            training_path = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [-m Model] [-t Training]\n", argv[0]);
            return 1;
        }
    }

    Datamodel datamodel = Datamodel(model_path, training_path);

    while (dm)
    {
        Vehicle &vehicle = ;

        bool done = vehicle.update_measurements();

        if (!done)
        {
            vehicles.push_back(vehicle);
        }

        vehicles.pop_front();
    }

    delete dm;
    return errno;
}
