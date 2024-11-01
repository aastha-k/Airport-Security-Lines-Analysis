// SingleServerFiniteBuffer.cpp
#include "Simulations.h"
#include "Passenger.h"
#include "Globals.h"
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>

// Global variables specific to this simulation
extern int passenger_number;
extern double gt;
extern std::mutex log_mtx;

namespace SingleServerFiniteBuffer
{
    // Simulation variables
    bool flag = false;
    std::mutex mtx;

    int passenger_count = 0;
    int queue_length = 0;
    double previous_time = 0.0;
    double total_service = 0.0;
    double total_waiting_time = 0.0;
    double start_processing_time = 0.0;
    double total_queue_length = 0.0;

    std::queue<Passenger> securityLineForSingleServer;

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

            std::lock_guard<std::mutex> lock(mtx);

            if (queue_length < single_buffer_size)
            {
                securityLineForSingleServer.push(passenger);
                total_queue_length += (passenger._globalArrivalTime - previous_time) * queue_length;
                queue_length++;
                previous_time = passenger._globalArrivalTime;
                start_processing_time = std::max(start_processing_time, gt);
            }
        }
    }

    void simulateProcessing()
    {
        while (true)
        {
            if (flag)
                break;

            mtx.lock();

            double processing_time = 0;

            if (!securityLineForSingleServer.empty())
            {
                Passenger passenger = securityLineForSingleServer.front();
                processing_time = passenger._processing_time;
                securityLineForSingleServer.pop();
                total_queue_length += queue_length * (start_processing_time + passenger._processing_time - previous_time);
                queue_length--;
                previous_time = start_processing_time + passenger._processing_time;
                total_waiting_time += start_processing_time - passenger._globalArrivalTime;

                log_mtx.lock();
                std::cout << "\nStart Processing Time for passenger " << passenger._id << " : " << start_processing_time;
                log_mtx.unlock();

                start_processing_time += processing_time;
                passenger_count++;
                total_service += processing_time;
            }

            mtx.unlock();

            std::this_thread::sleep_for(std::chrono::duration<double>(processing_time));
        }
    }

    void initializeVariables()
    {
        passenger_number = 1;
        gt = 0.0;
        passenger_count = 0;
        queue_length = 0;
        previous_time = 0.0;
        total_service = 0.0;
        total_waiting_time = 0.0;
        start_processing_time = 0.0;
        total_queue_length = 0.0;
        flag = false;

        // Clear the queue
        while (!securityLineForSingleServer.empty())
            securityLineForSingleServer.pop();
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

        // Create arrival and processing threads.
        std::thread arrivals(simulateArrivals);
        std::thread processing_thread(simulateProcessing);

        // Wait for the threads to finish.
        arrivals.join();
        processing_thread.join();

        printSimulation();
    }
}

void simulate_singleServer_finiteBuffer()
{
    SingleServerFiniteBuffer::simulate();
}
