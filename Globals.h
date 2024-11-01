// Globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <mutex>
#include <vector>
#include <queue>

extern double arrival_rate;          // λ
extern double service_rate;          // μ
extern int m;                        // Number of servers
extern int single_buffer_size;       // Buffer size for finite buffer simulations
extern int K;                        // Buffer size for multi-server finite buffer
extern const double SIMULATION_TIME; // Simulation time in seconds

#endif // GLOBALS_H
