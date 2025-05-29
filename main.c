// Include necessary standard libraries
#include <stdio.h>      // For input and output functions
#include <string.h>     // For string manipulation (e.g., strcpy)
#include <stdlib.h>     // For general utilities like memory management

// Define constants for limits
#define MAX_PROCESSES 100       // Maximum number of processes allowed
#define MAX_NAME_LEN 32         // Maximum length for process name
#define MEMORY_SIZE 100       // Total memory available (in MB)

// Define a structure to represent a process
typedef struct {
    int pid;                   // Process ID
    char name[MAX_NAME_LEN];   // Process name
    int burst_time;            // Total time the process needs on CPU
    int arrival_time;          // Time when the process arrives
    int priority;              // Process priority (lower = higher priority)
    int memory;                // Memory required by the process (in MB)
    int remaining_time;        // Time left for process to complete
    int start_time;            // Time process starts execution
    int completion_time;       // Time process completes execution
    int waiting_time;          // Time process waits in the queue
    int turnaround_time;       // Total time from arrival to completion
    int allocated;             // Flag to check if memory is allocated (1 or 0)
    char state[16];            // Current state of process (New, Running, Terminated)
} Process;

// Global variables to store processes and memory usage
Process processes[MAX_PROCESSES];   // Array to hold all processes
int process_count = 0;              // Total number of processes
int memory_used = 0;                // Track current memory usage

// Function to preload sample processes
void load_sample_processes() {
    // Assign values to processes
    processes[0] = (Process){1, "Word Processor", 10, 0, 3, 15, 10, -1, -1, -1, -1, 0, "New"};
    processes[1] = (Process){2, "Web Browser", 6, 3, 5, 30, 6, -1, -1, -1, -1, 0, "New"};
    processes[2] = (Process){3, "File Manager", 2, 5, 2, 5, 2, -1, -1, -1, -1, 0, "New"};
    processes[3] = (Process){4, "Compiler", 8, 6, 4, 20, 8, -1, -1, -1, -1, 0, "New"};
    processes[4] = (Process){5, "Media Player", 4, 10, 1, 25, 4, -1, -1, -1, -1, 0, "New"};
    process_count = 5; // Set the count
}

// Function to allocate memory to a process
int allocate_memory(Process *p) {
    // Check if enough memory is available
    if (memory_used + p->memory <= MEMORY_SIZE) {
        memory_used += p->memory;  // Increase used memory
        p->allocated = 1;          // Mark as allocated
        return 1;                  // Return success
    }
    return 0; // Not enough memory
}

// Function to free memory used by a process
void deallocate_memory(Process *p) {
    if (p->allocated) {            // If memory was allocated
        memory_used -= p->memory;  // Subtract memory used
        p->allocated = 0;          // Mark as deallocated
    }
}

// Function to print memory usage
void print_memory_status() {
    printf("Total Memory: %d MB, Used: %d MB, Available: %d MB\n",
        MEMORY_SIZE, memory_used, MEMORY_SIZE - memory_used);
}

// Function to display all processes
void print_processes() {
    printf("PID\tName\t\tBurst\tArrival\tPriority\tMemory\n");
    for (int i = 0; i < process_count; i++) {
        printf("%d\t%-15s%d\t%d\t%d\t\t%dMB\n",
               processes[i].pid, processes[i].name,
               processes[i].burst_time, processes[i].arrival_time,
               processes[i].priority, processes[i].memory);
    }
}

// Function to reset process states and variables before scheduling
void reset_processes() {
    for (int i = 0; i < process_count; i++) {
        processes[i].remaining_time = processes[i].burst_time; // Reset remaining time
        processes[i].start_time = -1;
        processes[i].completion_time = -1;
        processes[i].waiting_time = -1;
        processes[i].turnaround_time = -1;
        processes[i].allocated = 0;
        strcpy(processes[i].state, "New");  // Reset state to New
    }
    memory_used = 0;  // Reset memory usage
}

// Function to calculate and show average wait and turnaround times
void calculate_metrics() {
    float total_wait = 0, total_turnaround = 0;
    int completed = 0;
    for (int i = 0; i < process_count; i++) {
        if (processes[i].completion_time != -1) {
            total_wait += processes[i].waiting_time;
            total_turnaround += processes[i].turnaround_time;
            completed++;
        }
    }
    if (completed) {
        printf("Average Waiting Time: %.2f\n", total_wait / completed);
        printf("Average Turnaround Time: %.2f\n", total_turnaround / completed);
    }
}

// First-Come-First-Served Scheduling algorithm
void fcfs() {
    reset_processes();  // Prepare processes
    printf("\n--- FCFS Scheduling ---\n");

    int time = 0;
    for (int i = 0; i < process_count; i++) {
        Process *p = &processes[i];
        if (!allocate_memory(p)) {
            printf("Process %d skipped (Insufficient memory)\n", p->pid);
            continue;
        }

        strcpy(p->state, "Running");
        // Set start time to current time or arrival time (whichever is later)
        p->start_time = (time > p->arrival_time) ? time : p->arrival_time;
        p->completion_time = p->start_time + p->burst_time;
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->start_time - p->arrival_time;
        time = p->completion_time;
        strcpy(p->state, "Terminated");

        printf("| %s [%d - %d] ", p->name, p->start_time, p->completion_time);
        deallocate_memory(p);  // Free memory
    }
    printf("|\n");
    calculate_metrics();  // Show average times
}

// Shortest Job First Scheduling algorithm
void sjf() {
    reset_processes();
    printf("\n--- SJF Scheduling ---\n");

    int completed = 0, time = 0;

    while (completed < process_count) {
        int shortest = -1;
        int min_burst = 9999;

        // Find process with shortest burst time
        for (int i = 0; i < process_count; i++) {
            Process *p = &processes[i];
            if (p->completion_time == -1 && p->arrival_time <= time && p->burst_time < min_burst) {
                min_burst = p->burst_time;
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++; // No process ready, move time forward
            continue;
        }

        Process *p = &processes[shortest];
        if (!allocate_memory(p)) {
            printf("Process %d skipped (Insufficient memory)\n", p->pid);
            p->completion_time = 0;
            completed++;
            continue;
        }

        strcpy(p->state, "Running");
        p->start_time = time;
        p->completion_time = time + p->burst_time;
        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->start_time - p->arrival_time;
        time = p->completion_time;
        strcpy(p->state, "Terminated");
        printf("| %s [%d - %d] ", p->name, p->start_time, p->completion_time);
        deallocate_memory(p);
        completed++;
    }

    printf("|\n");
    calculate_metrics();
}

// Round Robin Scheduling algorithm
void round_robin(int quantum) {
    reset_processes();
    printf("\n--- Round Robin Scheduling (Q = %d) ---\n", quantum);

    int time = 0, completed = 0;
    int queue[MAX_PROCESSES]; // Circular queue for process index
    int front = 0, rear = 0;
    int added[MAX_PROCESSES] = {0}; // Track which processes are added

    while (completed < process_count) {
        // Add new arrivals to the queue
        for (int i = 0; i < process_count; i++) {
            if (!added[i] && processes[i].arrival_time <= time) {
                if (allocate_memory(&processes[i])) {
                    queue[rear++] = i;
                    added[i] = 1;
                } else {
                    printf("Process %d skipped (Insufficient memory)\n", processes[i].pid);
                    added[i] = 1;
                }
            }
        }

        if (front == rear) {
            time++; // CPU idle
            continue;
        }

        int idx = queue[front++];
        Process *p = &processes[idx];
        strcpy(p->state, "Running");
        if (p->start_time == -1)
            p->start_time = time;

        // Execute for quantum or less
        int slice = (p->remaining_time < quantum) ? p->remaining_time : quantum;
        printf("| %s [%d - %d] ", p->name, time, time + slice);
        time += slice;
        p->remaining_time -= slice;

        // Add new arrivals again after time increment
        for (int i = 0; i < process_count; i++) {
            if (!added[i] && processes[i].arrival_time <= time) {
                if (allocate_memory(&processes[i])) {
                    queue[rear++] = i;
                    added[i] = 1;
                } else {
                    printf("Process %d skipped (Insufficient memory)\n", processes[i].pid);
                    added[i] = 1;
                }
            }
        }

        // If done, mark completed
        if (p->remaining_time == 0) {
            p->completion_time = time;
            p->turnaround_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            strcpy(p->state, "Terminated");
            deallocate_memory(p);
            completed++;
        } else {
            queue[rear++] = idx; // Add back to queue
        }
    }

    printf("|\n");
    calculate_metrics();
}

// Priority Scheduling algorithm (Non-preemptive)
void priority_scheduling() {
    printf("|\n");
}

// Main function with menu
int main() {
    int choice, quantum;
    load_sample_processes();  // Preload 5 sample processes

    //Loop forever until explicitly broken.
    while (1) {
        // Display menu
        printf("\n--- Process Scheduler ---\n");
        printf("1. View Processes\n");
        printf("2. First Come First Serve(FCFS)\n");
        printf("3. Shortest Job First(SJF)\n");
        printf("4. Round Robin\n");
        printf("5. Priority Scheduling\n");
        printf("6. Memory Status\n");
        printf("7. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);  // Get user input

      switch (choice) {
    case 1:
        print_processes();
        break;
    case 2:
        fcfs();
        break;
    case 3:
        sjf();
        break;
    case 4:
        printf("Enter quantum: ");
        scanf("%d", &quantum);
        round_robin(quantum);
        break;
    case 5:
        priority_scheduling();
        break;
    case 6:
        print_memory_status();
        break;
    case 7:
        return 0;
    default:
        printf("Invalid option!\n");
}

    }
}
