#ifndef TCB_H
#define TCB_H

struct Task
{
    double id;
    double period;
    double execution_time;
    double remaining_time;
    double next_release;
    double deadline;
    double absolute_deadline;
    bool completed;
    bool missed_deadline;

    // RTA & timing analysis
    double response_time;
    double release_time;
    double finish_time;
    double actual_response_time;
    double max_observed_response_time;
};

#endif