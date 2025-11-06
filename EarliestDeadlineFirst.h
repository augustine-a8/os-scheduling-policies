#ifndef EARLIESTDEADLINEFIRST_H
#define EARLIESTDEADLINEFIRST_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include "TCB.h"

class EarliestDeadlineFirst
{
private:
    std::vector<Task> tasks;
    double current_time;

public:
    EarliestDeadlineFirst() : current_time(0) {}

    void add_task(double id, double period, double execution_time);
    void analyze_response_times();
    void analyze_utilization();
    void run(double simulation_time);
    void report();
};

#endif
