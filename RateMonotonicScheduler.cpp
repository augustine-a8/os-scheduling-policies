#include "RateMonotonicScheduler.h"

void RateMonotonicScheduler::add_task(double id, double period, double execution_time)
{
    Task new_task{id, period, execution_time, 0, 0, period, period,
                  true, false, 0, 0, 0, 0, 0};

    tasks.push_back(new_task);

    std::sort(tasks.begin(), tasks.end(),
              [](const Task &a, const Task &b)
              { return a.period < b.period; });
}

void RateMonotonicScheduler::analyze_response_times()
{
    std::cout << "\n===== Response Time Analysis (RTA) =====\n";

    for (size_t i = 0; i < tasks.size(); i++)
    {
        int R_prev = 0;
        int R = tasks[i].execution_time;

        while (R != R_prev)
        {
            R_prev = R;
            int interference = 0;

            for (size_t j = 0; j < i; j++)
            {
                interference += std::ceil((double)R_prev / tasks[j].period) * tasks[j].execution_time;
            }

            R = tasks[i].execution_time + interference;

            if (R > tasks[i].deadline)
                break;
        }

        tasks[i].response_time = R;

        std::cout << "Task " << tasks[i].id
                  << " | C=" << tasks[i].execution_time
                  << " | T=" << tasks[i].period
                  << " | D=" << tasks[i].deadline
                  << " | R=" << tasks[i].response_time
                  << " => "
                  << ((tasks[i].response_time <= tasks[i].deadline) ? "Schedulable" : "Misses Deadline")
                  << std::endl;
    }
}

void RateMonotonicScheduler::analyze_utilization()
{
    size_t n = tasks.size();
    if (n == 0)
    {
        std::cout << "\n[Utilization Check] No tasks defined.\n";
        return;
    }

    double U = 0.0;
    for (const auto &t : tasks)
    {
        U += (double)t.execution_time / (double)t.period;
    }

    // Bound: n*(2^(1/n) - 1)
    double bound = n * (std::pow(2.0, 1.0 / (double)n) - 1.0);

    std::cout << "\n===== Liu & Layland Utilization Test =====\n";
    std::cout << "Total utilization U = " << U << "\n";
    std::cout << "RMS theoretical bound = " << bound << "\n";

    if (U <= bound)
    {
        std::cout << "Result: U <= bound. Task set is GUARANTEED schedulable under RMS.\n";
    }
    else if (U <= 1.0)
    {
        std::cout << "Result: U > bound but U <= 1. Not guaranteed by Liu&Layland; may still be schedulable (need RTA or simulation).\n";
    }
    else
    {
        std::cout << "Result: U > 1. Impossible to schedule (total demand exceeds CPU capacity).\n";
    }
}

void RateMonotonicScheduler::run(double simulation_time)
{
    analyze_utilization();

    analyze_response_times();

    std::cout << "\n===== Starting RMS Simulation =====\n";

    while (current_time < simulation_time)
    {
        for (auto &t : tasks)
        {
            if (current_time == t.next_release)
            {
                t.remaining_time = t.execution_time;
                t.completed = false;
                t.missed_deadline = false;
                t.absolute_deadline = current_time + t.deadline;
                t.next_release += t.period;
                t.release_time = current_time;
            }
        }

        for (auto &t : tasks)
        {
            if (!t.completed && current_time > t.absolute_deadline)
            {
                t.missed_deadline = true;
                std::cout << "Task " << t.id
                          << " missed its deadline at time "
                          << current_time << " ms\n";
                t.completed = true;
                t.remaining_time = 0;
            }
        }

        Task *running_task = nullptr;
        for (auto &t : tasks)
        {
            if (t.remaining_time > 0)
            {
                running_task = &t;
                break;
            }
        }

        if (running_task)
        {
            running_task->remaining_time--;
            std::cout << "[" << std::setw(2) << current_time << " ms] Running Task "
                      << running_task->id << std::endl;

            if (running_task->remaining_time == 0)
            {
                running_task->completed = true;
                running_task->finish_time = current_time + 1;
                running_task->actual_response_time = running_task->finish_time - running_task->release_time;

                if (running_task->actual_response_time > running_task->max_observed_response_time)
                    running_task->max_observed_response_time = running_task->actual_response_time;
            }
        }
        else
        {
            std::cout << "[" << std::setw(2) << current_time << " ms] CPU Idle\n";
        }

        current_time++;
    }
}

void RateMonotonicScheduler::report()
{
    std::cout << "\n===== Simulation Complete =====\n";
    std::cout << std::setw(6) << "Task"
              << std::setw(10) << "RTA(R)"
              << std::setw(12) << "Observed"
              << std::setw(12) << "Deadline"
              << std::setw(14) << "Schedulable\n";
    std::cout << "----------------------------------------------------------\n";

    for (const auto &t : tasks)
    {
        bool schedulable = t.response_time <= t.deadline && !t.missed_deadline;

        std::cout << std::setw(4) << "T" << t.id
                  << std::setw(10) << t.response_time
                  << std::setw(12) << t.max_observed_response_time
                  << std::setw(12) << t.deadline
                  << std::setw(12) << (schedulable ? "Yes" : "No")
                  << std::endl;
    }
}
