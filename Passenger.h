// Passenger.h
#ifndef PASSENGER_H
#define PASSENGER_H

class Passenger
{
public:
    int _id;
    double _interArrivalTime;
    double _globalArrivalTime; // Time when the passenger arrives at the checkpoint.
    double _processing_time;   // Time it takes to process the passenger.

    Passenger(double arrival_rate, double service_rate);
};

#endif // PASSENGER_H
