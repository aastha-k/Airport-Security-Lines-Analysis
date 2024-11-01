// Passenger.cpp
#include "Passenger.h"
#include "Globals.h"
#include <random>
#include <chrono>
#include <iostream>
#include <mutex>

// External variables
extern int passenger_number;
extern double gt;
extern std::mutex log_mtx;

Passenger::Passenger(double arrival_rate, double service_rate)
{
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> arrival_distribution(arrival_rate);
    std::exponential_distribution<double> processing_distribution(service_rate);

    _id = passenger_number++;
    _interArrivalTime = arrival_distribution(generator);
    gt += _interArrivalTime;
    _globalArrivalTime = gt;
    _processing_time = processing_distribution(generator);

    log_mtx.lock();
    std::cout << "\n\nPassenger Number : " << _id << "\nArrival Time : " << _interArrivalTime << ", Service Time : " << _processing_time;
    log_mtx.unlock();
}
