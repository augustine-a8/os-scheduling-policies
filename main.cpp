#include "RateMonotonicScheduler.h"
#include "EarliestDeadlineFirst.h"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./scheduler --mode <edf|rms>" << std::endl;
        return 1;
    }

    std::string option(argv[1]);
    std::string mode(argv[2]);

    if (option != "--mode")
    {
        std::cerr << "Invalid option: expected --mode" << std::endl;
        return 1;
    }

    if (mode != "edf" && mode != "rms")
    {
        std::cerr << "Invalid mode: must be 'edf' or 'rms'" << std::endl;
        return 1;
    }

    if (mode == "rms")
    {
        // run rate monotonic scheduling
        RateMonotonicScheduler rms1;

        rms1.add_task(1, 20, 3);
        rms1.add_task(2, 5, 2);
        rms1.add_task(3, 10, 2);

        rms1.run(50);

        rms1.report();

        // RateMonotonicScheduler rms2;

        // rms2.add_task(1, 5, 2);
        // rms2.add_task(2, 8, 3);
        // rms2.add_task(3, 12, 4);

        // rms2.run(50);

        // rms2.report();
    }
    else
    {
        // run earliest deadline first
        EarliestDeadlineFirst edf1;

        edf1.add_task(1, 4, 1);
        edf1.add_task(2, 5, 1.5);
        edf1.add_task(3, 10, 2);

        edf1.run(50);

        edf1.report();

        EarliestDeadlineFirst edf2;

        edf2.add_task(1, 5, 2);
        edf2.add_task(2, 8, 3);
        edf2.add_task(3, 12, 4);

        edf2.run(50);

        edf2.report();
    }

    return EXIT_SUCCESS;
}