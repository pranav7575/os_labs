#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;            // Process ID
    int arrival_time;   // Arrival Time
    int burst_time;     // Burst Time
    int priority;       // Priority (lower number means higher priority)
    int remaining_time; // Remaining Burst Time
    int finish_time;    // Finish Time
    int turnaround_time; // Turnaround Time
    int waiting_time;    // Waiting Time
    int start_time;      // First time process starts executing
};

void displayResults(struct Process processes[], int n) {
    printf("\nProcess\tAT\tBT\tPriority\tFT\tTAT\tWT\n");
    float avg_turnaround = 0, avg_waiting = 0;
    
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", 
            processes[i].pid,
            processes[i].arrival_time,
            processes[i].burst_time,
            processes[i].priority,
            processes[i].finish_time,
            processes[i].turnaround_time,
            processes[i].waiting_time);
            
        avg_turnaround += processes[i].turnaround_time;
        avg_waiting += processes[i].waiting_time;
    }
    
    printf("\nAverage Turnaround Time: %.2f", avg_turnaround/n);
    printf("\nAverage Waiting Time: %.2f\n", avg_waiting/n);
}

void priorityNonPreemptive(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    
    // Initialize remaining time
    for(int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    while(completed != n) {
        int highest_priority = 999999;
        int selected_idx = -1;
        
        // Find process with highest priority among arrived processes
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && 
               processes[i].remaining_time > 0) {
                if(processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    selected_idx = i;
                }
            }
        }
        
        if(selected_idx == -1) {
            current_time++;
            continue;
        }
        
        // Execute process
        processes[selected_idx].finish_time = current_time + processes[selected_idx].burst_time;
        processes[selected_idx].remaining_time = 0;
        current_time = processes[selected_idx].finish_time;
        completed++;
        
        // Calculate times immediately after process completion
        processes[selected_idx].turnaround_time = processes[selected_idx].finish_time - 
                                                processes[selected_idx].arrival_time;
        processes[selected_idx].waiting_time = processes[selected_idx].turnaround_time - 
                                             processes[selected_idx].burst_time;
    }
}

void priorityPreemptive(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    
    // Initialize remaining time
    for(int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].finish_time = 0;
    }
    
    while(completed != n) {
        int highest_priority = 999999;
        int selected_idx = -1;
        
        // Find process with highest priority among arrived processes
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && 
               processes[i].remaining_time > 0) {
                if(processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    selected_idx = i;
                }
            }
        }
        
        if(selected_idx == -1) {
            current_time++;
            continue;
        }
        
        // Execute process for 1 time unit
        processes[selected_idx].remaining_time--;
        current_time++;
        
        // If process completes
        if(processes[selected_idx].remaining_time == 0) {
            completed++;
            processes[selected_idx].finish_time = current_time;
            // Calculate times immediately after process completion
            processes[selected_idx].turnaround_time = processes[selected_idx].finish_time - 
                                                    processes[selected_idx].arrival_time;
            processes[selected_idx].waiting_time = processes[selected_idx].turnaround_time - 
                                                 processes[selected_idx].burst_time;
        }
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process *processes = (struct Process*)malloc(n * sizeof(struct Process));
    
    // Input process details
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("\nFor Process %d:\n", i + 1);
        printf("Enter Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Enter Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        printf("Enter Priority (lower number means higher priority): ");
        scanf("%d", &processes[i].priority);
    }
    
    // Create copies for both scheduling types
    struct Process *processes_np = (struct Process*)malloc(n * sizeof(struct Process));
    struct Process *processes_p = (struct Process*)malloc(n * sizeof(struct Process));
    
    for(int i = 0; i < n; i++) {
        processes_np[i] = processes[i];
        processes_p[i] = processes[i];
    }
    
    printf("\n\nNon-Preemptive Priority Scheduling:");
    priorityNonPreemptive(processes_np, n);
    displayResults(processes_np, n);
    
    printf("\n\nPreemptive Priority Scheduling:");
    priorityPreemptive(processes_p, n);
    displayResults(processes_p, n);
    
    // Free allocated memory
    free(processes);
    free(processes_np);
    free(processes_p);
    
    return 0;
}