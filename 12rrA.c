#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to hold process information
struct Process {
    int id;
    int arrival_time;    // Added arrival time
    int burst_time;
    int remaining_time;
    int finish_time;
    int turnaround_time;
    int waiting_time;
    bool started;        // Flag to track if process has started execution
};

// Function to find the next process to execute
int findNextProcess(struct Process processes[], int n, int current_time) {
    int selected = -1;
    int earliest_arrival = __INT_MAX__;
    
    for(int i = 0; i < n; i++) {
        if(processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
            if(!processes[i].started) {
                if(processes[i].arrival_time < earliest_arrival) {
                    earliest_arrival = processes[i].arrival_time;
                    selected = i;
                }
            } else if(selected == -1) {
                selected = i;
            }
        }
    }
    return selected;
}

// Function to implement Round Robin scheduling
void roundRobinScheduling(struct Process processes[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;
    
    // Initialize remaining time and started flag for all processes
    for(int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].started = false;
    }
    
    // Continue until all processes are completed
    while(completed < n) {
        int current_process = findNextProcess(processes, n, current_time);
        
        if(current_process == -1) {
            // No process available, advance time to next arrival
            int next_arrival = __INT_MAX__;
            for(int i = 0; i < n; i++) {
                if(processes[i].remaining_time > 0 && processes[i].arrival_time < next_arrival) {
                    next_arrival = processes[i].arrival_time;
                }
            }
            if(next_arrival != __INT_MAX__) {
                current_time = next_arrival;
            }
            continue;
        }
        
        processes[current_process].started = true;
        
        if(processes[current_process].remaining_time > quantum) {
            // Process will use full quantum
            current_time += quantum;
            processes[current_process].remaining_time -= quantum;
        }
        else {
            // Process will complete
            current_time += processes[current_process].remaining_time;
            processes[current_process].finish_time = current_time;
            processes[current_process].remaining_time = 0;
            completed++;
        }
    }
    
    // Calculate turnaround time and waiting time
    for(int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }
}

// Function to print the scheduling results
void printSchedule(struct Process processes[], int n, int quantum) {
    float avg_turnaround = 0, avg_waiting = 0;
    
    printf("\nRound Robin CPU Scheduling (Time Quantum = %d)\n", quantum);
    printf("\nProcess\tArrival Time\tBurst Time\tFinish Time\tTurnaround Time\tWaiting Time\n");
    printf("--------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].finish_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);
               
        avg_turnaround += processes[i].turnaround_time;
        avg_waiting += processes[i].waiting_time;
    }
    
    avg_turnaround /= n;
    avg_waiting /= n;
    
    printf("\nAverage Turnaround Time: %.2f", avg_turnaround);
    printf("\nAverage Waiting Time: %.2f\n", avg_waiting);
}

int main() {
    int n, quantum;
    
    // Get number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    // Allocate memory for processes
    struct Process *processes = (struct Process*)malloc(n * sizeof(struct Process));
    
    // Get arrival time and burst time for each process
    for(int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Enter arrival time for Process P%d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter burst time for Process P%d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
    }
    
    // Get time quantum
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    
    // Perform Round Robin scheduling
    roundRobinScheduling(processes, n, quantum);
    
    // Print results
    printSchedule(processes, n, quantum);
    
    // Free allocated memory
    free(processes);
    
    return 0;
}