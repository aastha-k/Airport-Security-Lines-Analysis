#include <iostream>
#include <queue>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

#define SIMULATION_TIME 100000.0

double arrival_rate = 3; // λ
double service_rate = 6; // μ
double global_time = 0;

class Passenger
{
public:
    int inter_arrival_time;
    int global_arrival_time;
    int processing_time;

    Passenger(double arrival_rate, double service_rate)
    {
        default_random_engine generator(random_device{}());
        exponential_distribution<double> arrival_distribution(arrival_rate);
        exponential_distribution<double> processing_distribution(service_rate);

        inter_arrival_time = static_cast<int>(arrival_distribution(generator) * 1000);
        processing_time = static_cast<int>(processing_distribution(generator) * 1000);

        global_time += inter_arrival_time;
        global_arrival_time = global_time;
        cout << "Created passenger has values : " << inter_arrival_time << " " << processing_time << "\n";
    }
};

double total_waiting_time = 0.0;
double start_proc_time = 0.0; // start processing time for a particular passenger in the service center
int passenger_cnt = 0;
int queue_length = 0; // number of items in queue
double total_service = 0;
int single_buffer_size = 5;
int num_servers = 5;
int total_passengers = 0;
queue<Passenger> security_line;
bool flag_stop_now = false;
mutex mtx;

// for multiple servers
vector<queue<Passenger>> security_lines(num_servers);
vector<thread> processing_threads;
vector<mutex> mtxs(num_servers);
vector<double> start_proc_times(num_servers, 0.0);

void simulateArrivals_singleServer_infiniteBuffer()
{
    while (true)
    {
        Passenger passenger(arrival_rate, service_rate);
        if (global_time > SIMULATION_TIME)
        {
            flag_stop_now = true;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(passenger.inter_arrival_time));

        mtx.lock();
        security_line.push(passenger);
        queue_length++;
        start_proc_time = max(start_proc_time, global_time);
        cout << "arrival_time " << global_time << "\n";
        mtx.unlock();
    }
}

void simulateProcessing_singleServer_infiniteBuffer()
{
    while (true)
    {
        if (flag_stop_now)
            break;

        mtx.lock();
        int proc_time = 0; // processing time of leaving passenger

        if (!security_line.empty())
        {
            Passenger passenger = security_line.front();
            proc_time = passenger.processing_time;
            cout << "start_proc_time : " << start_proc_time << "\n";
            security_line.pop();
            queue_length--;
            total_waiting_time += start_proc_time - passenger.global_arrival_time;
            cout << "wait: " << total_waiting_time << "\n";
            start_proc_time += proc_time;
            passenger_cnt++;
            total_service += proc_time;
        }

        mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(proc_time));
    }
}

void simulate_singleServer_infiniteBuffer()
{
    global_time = 0;
    passenger_cnt = 0;
    queue_length = 0;
    single_buffer_size = 5;
    total_waiting_time = 0;
    total_service = 0;
    passenger_cnt = 0;

    thread arrivals(simulateArrivals_singleServer_infiniteBuffer);
    thread processing_thread(simulateProcessing_singleServer_infiniteBuffer);
    arrivals.join();
    processing_thread.join();

    cout << "\n\n Theoretical average waiting time (seconds): " << arrival_rate / (service_rate * (service_rate - arrival_rate));
    cout << "\n Empirical average waiting time (seconds): " << total_waiting_time / (1000 * passenger_cnt) << "\n";
    cout << "\n Theoretical average queue length: " << (arrival_rate * arrival_rate) / (service_rate * (service_rate - arrival_rate));
    cout << "\n Empirical average queue length: " << total_waiting_time / SIMULATION_TIME << "\n";
    cout << "\n Theoretical Service Utilization: " << arrival_rate / service_rate;
    cout << "\n Empirical Service Utilization: " << total_service / SIMULATION_TIME << "\n";
}

void simulateArrivals_singleServer_finiteBuffer()
{
    while (true)
    {
        Passenger passenger(arrival_rate, service_rate);
        if (global_time > SIMULATION_TIME)
        {
            flag_stop_now = true;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(passenger.inter_arrival_time));

        if (queue_length < single_buffer_size)
        {
            mtx.lock();
            security_line.push(passenger);
            queue_length++;
            start_proc_time = max(start_proc_time, global_time);

            cout << "arrival_time " << global_time << "\n";
            mtx.unlock();
        }
    }
}

void simulateProcessing_singleServer_finiteBuffer()
{
    while (true)
    {
        if (flag_stop_now)
            break;
        mtx.lock();
        int proc_time = 0;
        if (!security_line.empty())
        {
            Passenger passenger = security_line.front();
            proc_time = passenger.processing_time;
            cout << "start_proc_time : " << start_proc_time << "\n";
            security_line.pop();
            queue_length--;
            total_waiting_time += start_proc_time - passenger.global_arrival_time;
            cout << "wait: " << total_waiting_time << "\n";
            start_proc_time += proc_time;
            passenger_cnt++;
            total_service += proc_time;
        }

        mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(proc_time));
    }
}

void simulate_singleServer_finiteBuffer()
{
    global_time = 0;
    passenger_cnt = 0;
    queue_length = 0;
    single_buffer_size = 5;
    total_waiting_time = 0;
    total_service = 0;
    passenger_cnt = 0;

    thread arrivals(simulateArrivals_singleServer_finiteBuffer);
    thread processing_thread(simulateProcessing_singleServer_finiteBuffer);

    arrivals.join();
    processing_thread.join();

    cout << "\n\n Theoretical average waiting time (seconds): " << arrival_rate / (service_rate * (service_rate - arrival_rate));
    cout << "\n Empirical average waiting time (seconds): " << total_waiting_time / (1000 * passenger_cnt) << "\n";
    cout << "\n Theoretical average queue length: " << (arrival_rate * arrival_rate) / (service_rate * (service_rate - arrival_rate));
    cout << "\n Empirical average queue length: " << total_waiting_time / SIMULATION_TIME << "\n";
    cout << "\n Theoretical Service Utilization: " << arrival_rate / service_rate;
    cout << "\n Empirical Service Utilization: " << total_service / SIMULATION_TIME << "\n";
}

void simulateArrivals_multipleServer_infiniteBuffer()
{
    while (true)
    {
        Passenger passenger(arrival_rate, service_rate);
        if (global_time > SIMULATION_TIME)
        {
            flag_stop_now = true;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(passenger.inter_arrival_time));
        for (int i = 0; i < num_servers; i++)
        {
            mtxs[i].lock();
        }

        int ind = 0;
        int min_length = security_lines[0].size();
        for (int i = 0; i < num_servers; i++)
        {
            if (security_lines[i].size() < min_length)
            {
                ind = i;
                min_length = security_lines[i].size();
            }
        }

        for (int i = 0; i < num_servers; i++)
        {
            if (i == ind)
                continue;
            mtxs[i].unlock();
        }

        security_lines[ind].push(passenger);
        queue_length++;
        start_proc_times[ind] = max(start_proc_times[ind], global_time);
        cout << "arrival_time " << global_time << "\n";
        mtxs[ind].unlock();
    }
}

void simulateProcessing_multipleServer_infiniteBuffer(int ind)
{
    while (true)
    {
        if (flag_stop_now)
            break;

        mtxs[ind].lock();
        int proc_time = 0; // processing time of leaving passenger

        if (!security_lines[ind].empty())
        {
            Passenger passenger = security_lines[ind].front();
            proc_time = passenger.processing_time;
            cout << "start_proc_time : " << start_proc_time << "\n";
            security_lines[ind].pop();
            queue_length--;
            total_waiting_time += start_proc_times[ind] - passenger.global_arrival_time;
            cout << "wait: " << total_waiting_time << "\n";
            start_proc_times[ind] += proc_time;
            passenger_cnt++;
            total_service += proc_time;
        }

        mtxs[ind].unlock();
        this_thread::sleep_for(chrono::milliseconds(proc_time));
    }
}

void simulate_multipleServer_infiniteBuffer()
{
    global_time = 0;
    passenger_cnt = 0;
    queue_length = 0;
    single_buffer_size = 5;
    total_waiting_time = 0;
    total_service = 0;
    passenger_cnt = 0;

    thread arrivals(simulateArrivals_multipleServer_infiniteBuffer);

    vector<thread> servers;
    for (int i = 0; i < num_servers; i++)
    {
        thread t(simulateProcessing_multipleServer_infiniteBuffer, i);
        servers.push_back(move(t));
    }

    arrivals.join();
    for (int i = 0; i < num_servers; i++)
    {
        servers[i].join();
    }

    cout << "\n Empirical average waiting time (seconds): " << total_waiting_time / (1000 * passenger_cnt) << "\n";
    cout << "\n Empirical average queue length (seconds): " << total_waiting_time / SIMULATION_TIME << "\n";
    cout << "\n Empirical Service Utilization: " << total_service / (num_servers * SIMULATION_TIME) << "\n";
}

void simulateArrivals_multipleServer_finiteBuffer()
{
    while (true)
    {
        Passenger passenger(arrival_rate, service_rate);
        if (global_time > SIMULATION_TIME)
        {
            flag_stop_now = true;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(passenger.inter_arrival_time));
        for (int i = 0; i < num_servers; i++)
        {
            mtxs[i].lock();
        }

        int ind = 0;
        int min_length = security_lines[0].size();
        for (int i = 0; i < num_servers; i++)
        {
            if (security_lines[i].size() < min_length)
            {
                ind = i;
                min_length = security_lines[i].size();
            }
        }
        if (min_length < single_buffer_size)
        {
            for (int i = 0; i < num_servers; i++)
            {
                if (i == ind)
                    continue;
                mtxs[i].unlock();
            }

            security_lines[ind].push(passenger);
            queue_length++;
            start_proc_times[ind] = max(start_proc_times[ind], global_time);
            cout << "arrival_time " << global_time << "\n";
            mtxs[ind].unlock();
        }
    }
}

void simulateProcessing_multipleServer_finiteBuffer(int ind)
{
    while (true)
    {
        if (flag_stop_now)
            break;

        mtxs[ind].lock();
        int proc_time = 0; // processing time of leaving passenger

        if (!security_lines[ind].empty())
        {
            Passenger passenger = security_lines[ind].front();
            proc_time = passenger.processing_time;
            cout << "start_proc_time : " << start_proc_time << "\n";
            security_lines[ind].pop();
            queue_length--;
            total_waiting_time += start_proc_times[ind] - passenger.global_arrival_time;
            cout << "wait: " << total_waiting_time << "\n";
            start_proc_times[ind] += proc_time;
            passenger_cnt++;
            total_service += proc_time;
        }

        mtxs[ind].unlock();
        this_thread::sleep_for(chrono::milliseconds(proc_time));
    }
}

void simulate_multipleServer_finiteBuffer()
{
    global_time = 0;
    passenger_cnt = 0;
    queue_length = 0;
    single_buffer_size = 5;
    total_waiting_time = 0;
    total_service = 0;
    passenger_cnt = 0;

    thread arrivals(simulateArrivals_multipleServer_finiteBuffer);

    vector<thread> servers;
    for (int i = 0; i < num_servers; i++)
    {
        thread t(simulateProcessing_multipleServer_finiteBuffer, i);
        servers.push_back(move(t));
    }

    arrivals.join();
    for (int i = 0; i < num_servers; i++)
    {
        servers[i].join();
    }

    cout << "\n Empirical average waiting time (seconds): " << total_waiting_time / (1000 * passenger_cnt) << "\n";
    cout << "\n Empirical average queue length (seconds): " << total_waiting_time / SIMULATION_TIME << "\n";
    cout << "\n Empirical Service Utilization: " << total_service / (num_servers * SIMULATION_TIME) << "\n";
}

int main()
{
    int choice;
    do
    {
        cout << "Menu:" << endl;
        cout << "1. Single Server - Infinite Buffer" << endl;
        cout << "2. Single Server - Finite Buffer" << endl;
        cout << "3. Multiple Servers - Infinite Buffer" << endl;
        cout << "4. Multiple Servers - Finite Buffer" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            simulate_singleServer_infiniteBuffer();
            return 0;
        case 2:
            simulate_singleServer_finiteBuffer();
            return 0;
        case 3:
            simulate_multipleServer_infiniteBuffer();
            return 0;
        case 4:
            simulate_multipleServer_finiteBuffer();
            return 0;
        case 5:
            cout << "Exiting the program." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
            return 0;
        }

    } while (choice != 5);

    return 0;
}