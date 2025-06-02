📘 Process Scheduling and Memory Management in C

This project is a simple simulation of process scheduling algorithms and memory management using the C programming language. It demonstrates how operating systems manage process execution with different scheduling strategies, including First-Come-First-Served (FCFS), Shortest Job First (SJF), Round Robin, and Priority Scheduling. It also includes memory allocation/deallocation logic for each process.

🧠 Features
Process Structure: Each process has properties like PID, name, burst time, arrival time, memory needs, and scheduling state.

Memory Management:

Allocates memory before scheduling a process.

Skips processes if memory isn't available.

Deallocates memory after execution.

Scheduling Algorithms:

FCFS (First-Come-First-Served)

SJF (Shortest Job First)

Round Robin (with configurable time quantum)

Priority Scheduling (non-preemptive)

Metrics Reporting:

Average Waiting Time

Average Turnaround Time

Process Viewer: Display list of all processes with their configurations.

🛠️ How It Works
Process Structure
Each process is represented using a struct containing:

Process ID (PID)

Process Name

Arrival Time

Burst Time

Priority

Memory Requirement

Remaining Time

Start Time

Completion Time

Waiting Time

Turnaround Time

State (New, Running, Terminated)

Memory Allocation Flag

Memory Model
Total available memory: 100 MB

Each process requests a portion of memory before execution.

If memory is not sufficient, the process is skipped.

📋 Sample Data
15 predefined sample processes are loaded with different:

Arrival times

Burst times

Memory requirements

Priorities

Examples
1  Word Processor     10  0   3   15MB
2  Web Browser        6   3   5   30MB
3  File Manager       2   5   2   5MB
...


🚀 Compilation & Usage
Step 1: Compile

gcc -o scheduler main.c

Step 2: Run
./scheduler

This will invoke the main() function (you can customize it to call fcfs(), sjf(), etc.).

🔁 Available Functions
| Function                  | Description                                             |
| ------------------------- | ------------------------------------------------------- |
| `load_sample_processes()` | Loads predefined processes into memory                  |
| `allocate_memory(p)`      | Tries to allocate memory to process `p`                 |
| `deallocate_memory(p)`    | Frees memory used by process `p`                        |
| `print_memory_status()`   | Displays memory usage                                   |
| `print_processes()`       | Shows list of all processes                             |
| `reset_processes()`       | Resets all process states for rescheduling              |
| `calculate_metrics()`     | Prints average wait and turnaround times                |
| `fcfs()`                  | Runs FCFS scheduling                                    |
| `sjf()`                   | Runs Shortest Job First scheduling                      |
| `round_robin(quantum)`    | Runs Round Robin scheduling with specified time quantum |
| `priority_scheduling()`   | Runs Priority Scheduling (non-preemptive)               |

📊 Sample Output

--- FCFS Scheduling ---
| Word Processor [0 - 10] | Web Browser [10 - 16] | File Manager [16 - 18] ...
Average Waiting Time: 5.93
Average Turnaround Time: 12.47

🧪 Customization
You can modify the main() function to test each scheduler individually:

int main() {
    load_sample_processes();
    print_processes();
    print_memory_status();

    // Uncomment the scheduler you want to test
    fcfs();
    // sjf();
    // round_robin(4);  // Use quantum = 4
    // priority_scheduling();

    return 0;
}

📎 Notes
This program assumes non-preemptive scheduling for SJF and Priority.

Round Robin is preemptive and time-sliced.

Memory is dynamically managed per process to simulate real-world scenarios.

🧑‍💻 Author
Developed as part of an Operating Systems simulation assignment.

Contributions and enhancements welcome!
