# Analyzing Airport Security Lines

This project aims to simulate and analyze the behavior of airport security lines using various queuing system models. By leveraging the principles of queuing theory and implementing these models in C++, we aim to understand the impact of different configurations on waiting times, queue lengths, and system utilization.

## Table of Contents
- [Introduction](#introduction)
- [Project Overview](#project-overview)
- [Key Components](#key-components)
- [Models Simulated](#models-simulated)
- [Simulation Results](#simulation-results)
- [Code Explanation](#code-explanation)
- [Observations and Analysis](#observations-and-analysis)
- [Conclusion](#conclusion)
- [How to Run](#how-to-run)
- [Contributors](#contributors)

## Introduction

Queuing systems are crucial in managing resources efficiently, especially in high-traffic areas like airports. This project explores various queuing models to simulate and optimize airport security lines. By understanding the dynamics of arrival rates, service rates, and buffer sizes, we aim to propose strategies for minimizing wait times and improving the overall efficiency of security screening processes.

## Project Overview

The project simulates different queuing system configurations, including single-server and multi-server setups with both infinite and finite buffers. The simulations are implemented in C++ and use threads to model the concurrent processing of passengers through security lines.

## Key Components
- **Passenger Class**: Represents passengers with attributes such as inter-arrival time and processing time, generated using exponential distributions.
- **Queues and Buffers**: The simulation uses queues to model the waiting lines, with multiple queues for multi-server setups.
- **Threads**: Simulate passenger arrivals and processing concurrently, with mutexes ensuring thread safety.
- **Simulation Functions**: Different functions represent various queuing configurations like single-server infinite buffer (M/M/1) and multi-server finite buffer (M/M/m/K).

## Models Simulated
1. **Single Server with Infinite Buffer (M/M/1)**
2. **Single Server with Finite Buffer (M/M/1/K)**
3. **Multi-Server with Infinite Buffer (M/M/m)**
4. **Multi-Server with Finite Buffer (M/M/m/K)**

## Simulation Results

### Single Server with Infinite Buffer (M/M/1)
- **Waiting Time**: Shows how increasing arrival rates impact system utilization and waiting times.
  
### Single Server with Finite Buffer (M/M/1/K)
- **Queue Length**: Demonstrates how finite buffers can lead to packet loss when arrival rates are high.

### Multi-Server with Infinite Buffer (M/M/m)
- **Utilization**: Highlights the benefits of having multiple servers, significantly reducing waiting times and queue lengths.

### Multi-Server with Finite Buffer (M/M/m/K)
- **System Capacity**: Shows how multi-server systems handle larger volumes of passengers, reducing congestion.

## Code Explanation

The provided C++ code simulates the above models using the following components:
- **Passenger Class**: Defines the passenger attributes and simulates their behavior in the queue.
- **Global Variables**: Manage the arrival and service rates, as well as track various metrics like waiting times and queue lengths.
- **Threaded Simulation**: Uses threads to concurrently simulate passenger arrivals and processing, ensuring efficiency and accuracy.
- **Menu-Driven Interface**: Allows users to select the desired simulation model and view results.

## Observations and Analysis

- **Single vs. Multi-Server**: Multi-server systems are more efficient for handling high arrival rates, reducing both waiting times and queue lengths.
- **Buffer Size Impact**: Finite buffers can lead to packet loss in high traffic, underscoring the importance of buffer management.
- **Resource Utilization**: Multi-server systems offer better resource utilization by assigning idle servers to incoming tasks.

## Conclusion

The simulation successfully models various queuing systems, providing insights into how different configurations affect airport security line efficiency. The use of threads and mutexes in C++ ensures a realistic and concurrent simulation, making this a valuable tool for studying and optimizing queuing systems.

## How to Run

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-repo/airport-security-lines.git
   cd airport-security-lines
