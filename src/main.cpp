
#include "datamodel.hpp"
#include "output.hpp"

int main(int argc, char *argv[])
{
    try
    {

        DataModel DM = DataModel(argc, argv);

        //
        // Main loop: process new measurements as they arrive
        //
        while (Vehicle *vehicle = DM.wait_measurement())
        {

            Evaluation eval;

            //
            // 1. Capture the current measurement
            //

            eval.soc       = vehicle->soc_current;
            eval.timestemp = vehicle->timestemp;
            eval.power     = vehicle->power_current;

            //
            // 2. Predict SoC at the time of the next measurement
            //

            double next_time = DM.get_next_prediction_time(vehicle->uid);
            double duration  = vehicle->timestemp - next_time;

            eval.soc_prediction = vehicle->predict_soc(duration);
            eval.soc_error_rate = vehicle->error_rate(eval.soc_prediction);

            //
            // 3. Predict the time required to reach the next SoC milestone
            //

            double next_soc = DM.get_next_prediction_soc(vehicle->uid);

            eval.time_prediction = vehicle->predict_time(next_soc);
            eval.time_error_rate = vehicle->error_rate(eval.time_prediction);

            //
            // 4. Write Evaluation into Vehicle specific file
            //

            DM.write_evaluation(eval);
        }

        return EXIT_SUCCESS;
    }
    catch (exception &e)
    {
        return EXIT_FAILURE;
    }
}
