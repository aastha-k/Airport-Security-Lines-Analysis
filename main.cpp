// main.cpp
#include "Simulations.h"
#include "Globals.h"
#include <iostream>

double arrival_rate = 5.0;           // λ
double service_rate = 6.0;           // μ
int m = 1;                           // Number of servers
int single_buffer_size = 5;          // Buffer size for single server finite buffer
int K = 100;                         // Buffer size for multi-server finite buffer
const double SIMULATION_TIME = 10.0; // Simulation time in seconds

int main()
{
    std::cout << "Enter to simulate \n1 - single server with infinite buffers.\n2 - single server with finite buffers.\n";
    std::cout << "3 - multiple servers with infinite buffers.\n4 - multiple servers with finite buffers.";
    std::cout << "\n-1 to exit.\n";
    int x;
    std::cin >> x;

    if (x == 1)
    {
        std::cout << "Enter arrival rate: ";
        std::cin >> arrival_rate;
        std::cout << "Enter service rate: ";
        std::cin >> service_rate;
        m = 1;
        simulate_singleServer_infiniteBuffers();
    }
    else if (x == 2)
    {
        std::cout << "Enter arrival rate: ";
        std::cin >> arrival_rate;
        std::cout << "Enter service rate: ";
        std::cin >> service_rate;
        std::cout << "Enter buffer size: ";
        std::cin >> single_buffer_size;
        m = 1;
        simulate_singleServer_finiteBuffer();
    }
    else if (x == 3)
    {
        std::cout << "Enter arrival rate: ";
        std::cin >> arrival_rate;
        std::cout << "Enter service rate: ";
        std::cin >> service_rate;
        std::cout << "Enter number of servers: ";
        std::cin >> m;
        simulate_multiServer_infiniteBuffer();
    }
    else if (x == 4)
    {
        std::cout << "Enter arrival rate: ";
        std::cin >> arrival_rate;
        std::cout << "Enter service rate: ";
        std::cin >> service_rate;
        std::cout << "Enter number of servers: ";
        std::cin >> m;
        std::cout << "Enter buffer size: ";
        std::cin >> K;
        simulate_multipleServer_finiteBuffer();
    }
    else if (x == -1)
    {
        std::cout << "Exiting the program.\n";
    }
    else
    {
        std::cout << "Invalid choice.\n";
    }

    return 0;
}
