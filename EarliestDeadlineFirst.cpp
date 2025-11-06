#include "EarliestDeadlineFirst.h"

void EarliestDeadlineFirst::add_task(double id, double period, double execution_time)
{
    Task new_task{id, period, execution_time, 0, 0, period, period,
                  true, false, 0, 0, 0, 0, 0};

    tasks.push_back(new_task);
}

void EarliestDeadlineFirst::analyze_utilization()
{
    size_t n = tasks.size();
    if (n == 0)
    {
        std::cout << "\n[Utilization Check] No tasks defined.\n";
        return;
    }

    double U = 0.0;
    for (const auto &t : tasks)
        U += (double)t.execution_time / (double)t.period;

    std::cout << "\n===== EDF Utilization Test =====\n";
    std::cout << "Total utilization U = " << U << "\n";
    std::cout << "Feasibility criterion: U ≤ 1.0\n";

    if (U <= 1.0)
        std::cout << "Feasible under EDF scheduling.\n";
    else
        std::cout << "Not feasible under EDF (CPU over-utilized).\n";
}

void EarliestDeadlineFirst::run(double simulation_time)
{
    analyze_utilization();

    std::cout << "\n===== Starting EDF Simulation =====\n";

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
                std::cout << "⚠️  Task " << t.id
                          << " missed its deadline at time "
                          << current_time << " ms\n";
                t.completed = true;
                t.remaining_time = 0;
            }
        }

        std::vector<Task *> ready_tasks;
        for (auto &t : tasks)
        {
            if (t.remaining_time > 0)
                ready_tasks.push_back(&t);
        }

        std::sort(ready_tasks.begin(), ready_tasks.end(),
                  [](Task *a, Task *b)
                  { return a->absolute_deadline < b->absolute_deadline; });

        Task *running_task = ready_tasks.empty() ? nullptr : ready_tasks.front();

        if (running_task)
        {
            running_task->remaining_time--;
            std::cout << "[" << std::setw(2) << current_time << " ms] Running Task "
                      << running_task->id << " (D="
                      << running_task->absolute_deadline << ")\n";

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

void EarliestDeadlineFirst::report()
{
    std::cout << "\n===== Simulation Complete (EDF) =====\n";
    std::cout << std::setw(6) << "Task"
              << std::setw(10) << "Period"
              << std::setw(12) << "Deadline"
              << std::setw(12) << "Missed?"
              << std::setw(14) << "Max Resp Time\n";
    std::cout << "----------------------------------------------------------\n";

    for (const auto &t : tasks)
    {
        std::cout << std::setw(4) << "T" << t.id
                  << std::setw(10) << t.period
                  << std::setw(12) << t.deadline
                  << std::setw(12) << (t.missed_deadline ? "Yes" : "No")
                  << std::setw(14) << t.max_observed_response_time
                  << std::endl;
    }
}
