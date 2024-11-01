// MultiServerInfiniteBuffer.cpp
#include "Simulations.h"
#include "Passenger.h"
#include "Globals.h"
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <climits>

// Global variables specific to this simulation
extern int passenger_number;
extern double gt;
extern std::mutex log_mtx;

namespace MultiServerInfiniteBuffer
{
    // Simulation variables
    bool flag = false;
    std::vector<std::mutex> mtx_vec(1000);
    std::vector<double> startProcessingTimeForMultipleServers(1000, 0.0);

    int passenger_count = 0;
    double total_service = 0.0;
    double total_waiting_time = 0.0;

    std::vector<std::queue<Passenger>> securityLineForMultipleServers(1000);

    void simulateArrivals()
    {
        while (true)
        {
            Passenger passenger(arrival_rate, service_rate);

            if (gt > SIMULATION_TIME)
            {
                flag = true;
                break;
            }

            std::this_thread::sleep_for(std::chrono::duration<double>(passenger._interArrivalTime));

            for (int i = 0; i < m; i++)
                mtx_vec[i].lock();

            int minq = -1;
            int minlen = INT_MAX;

            for (int i = 0; i < m; i++)
            {
                if (securityLineForMultipleServers[i].size() < minlen)
                {
                    minlen = securityLineForMultipleServers[i].size();
                    minq = i;
                }
            }

            for (int i = 0; i < m; i++)
            {
                if (i == minq)
                    continue;
                mtx_vec[i].unlock();
            }

            securityLineForMultipleServers[minq].push(passenger);
            startProcessingTimeForMultipleServers[minq] = std::max(startProcessingTimeForMultipleServers[minq], gt);

            mtx_vec[minq].unlock();
        }
    }

    void simulateProcessing(int index)
    {
        while (true)
        {
            if (flag)
                break;

            mtx_vec[index].lock();

            double processing_time = 0;

            if (!securityLineForMultipleServers[index].empty())
            {
                Passenger passenger = securityLineForMultipleServers[index].front();
                processing_time = passenger._processing_time;
                log_mtx.lock();
                std::cout << "\nStart Processing Time for passenger " << passenger._id << " : " << startProcessingTimeForMultipleServers[index];
                std::cout << "\nWaiting time for passenger " << passenger._id << " : " << startProcessingTimeForMultipleServers[index] - passenger._globalArrivalTime;
                log_mtx.unlock();
                securityLineForMultipleServers[index].pop();
                total_waiting_time += startProcessingTimeForMultipleServers[index] - passenger._globalArrivalTime;
                startProcessingTimeForMultipleServers[index] += processing_time;
                passenger_count++;
                total_service += processing_time;
            }

            mtx_vec[index].unlock();

            std::this_thread::sleep_for(std::chrono::duration<double>(processing_time));
        }
    }

    void initializeVariables()
    {
        passenger_number = 1;
        gt = 0.0;
        passenger_count = 0;
        total_service = 0.0;
        total_waiting_time = 0.0;
        flag = false;

        startProcessingTimeForMultipleServers.resize(m);
        securityLineForMultipleServers.resize(m);

        for (int i = 0; i < m; i++)
        {
            while (!securityLineForMultipleServers[i].empty())
                securityLineForMultipleServers[i].pop();
        }
    }

    void printSimulation()
    {
        std::cout << "\n\nAverage waiting time (simulation): " << total_waiting_time / passenger_count << " seconds.";
        std::cout << "\nAverage queue length (simulation): " << total_waiting_time / SIMULATION_TIME;
        std::cout << "\nService Utilization (simulation): " << total_service / SIMULATION_TIME;
        std::cout << "\n\n";
    }

    void simulate()
    {
        initializeVariables();

        std::thread arrivals(simulateArrivals);
        std::vector<std::thread> scanners;
        for (int i = 0; i < m; i++)
        {
            scanners.emplace_back(simulateProcessing, i);
        }

        arrivals.join();
        for (int i = 0; i < m; i++)
            scanners[i].join();

        printSimulation();
    }
}

void simulate_multiServer_infiniteBuffer()
{
    MultiServerInfiniteBuffer::simulate();
}
