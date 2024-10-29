#include <stdio.h>
#include <stdlib.h>

// Structure to store process information
struct Process {
    int pid;            // Process ID
    int arrival_time;   // Arrival time
    int burst_time;     // Burst time
    int finish_time;    // Finish time
    int turnaround_time; // Turnaround time
    int waiting_time;    // Waiting time
};

// Function to swap processes (used for sorting)
// void swap(struct Process *xp, struct Process *yp) {
//     struct Process temp = *xp;
//     *xp = *yp;
//     *yp = temp;
// }

// Function to sort processes by arrival time
void sortByArrivalTime(struct Process proc[], int n) {
    int i, j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (proc[j].arrival_time > proc[j+1].arrival_time) {
               struct Process temp = proc[j];
                proc[j] = proc[j+1];
                proc[j+1] = temp;
            }
        }
    }
}

// Function to calculate times for FCFS
void calculateTimes(struct Process proc[], int n) {
    int current_time = 0;
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    
    // Calculate for each process
    for(int i = 0; i < n; i++) {
        // If there's a gap between processes
        if(current_time < proc[i].arrival_time) {
            current_time = proc[i].arrival_time;
        }
        
        // Calculate finish time
        proc[i].finish_time = current_time + proc[i].burst_time;
        
        // Calculate turnaround time
        proc[i].turnaround_time = proc[i].finish_time - proc[i].arrival_time;
        
        // Calculate waiting time
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
        
        // Update current time
        current_time = proc[i].finish_time;
        
        // Add to totals
        total_turnaround_time += proc[i].turnaround_time;
        total_waiting_time += proc[i].waiting_time;
    }
    
    // Print process details
    printf("\nProcess Execution Details:\n");
    printf("-------------------------------------------------------\n");
    printf("PID\tArrival\tBurst\tFinish\tTurnaround\tWaiting\n");
    printf("-------------------------------------------------------\n");
    
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", 
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].finish_time,
               proc[i].turnaround_time,
               proc[i].waiting_time);
    }
    printf("-------------------------------------------------------\n");
    
    // Print averages
    printf("\nAverage Turnaround Time: %.2f", total_turnaround_time/n);
    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time/n);
}

// Function to display Gantt chart
void displayGanttChart(struct Process proc[], int n) {
    printf("\nGantt Chart:\n");
    printf("------------------------------------------\n");
    
    // Print process IDs
    printf("|");
    for(int i = 0; i < n; i++) {
        printf(" P%d |", proc[i].pid);
    }
    printf("\n");
    
    // Print timeline
    printf("0");
    for(int i = 0; i < n; i++) {
        printf("    %d", proc[i].finish_time);
    }
    printf("\n------------------------------------------\n");
}

int main() {
    int n, i;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process *proc = (struct Process*)malloc(n * sizeof(struct Process));
    
    // Input process details
    for(i = 0; i < n; i++) {
        printf("\nFor Process %d:\n", i+1);
        proc[i].pid = i+1;
        
        printf("Enter Arrival Time: ");
        scanf("%d", &proc[i].arrival_time);
        
        printf("Enter Burst Time: ");
        scanf("%d", &proc[i].burst_time);
    }
    
    // Sort processes by arrival time
    sortByArrivalTime(proc, n);
    
    // Calculate and display times
    calculateTimes(proc, n);
    
    // Display Gantt chart
    displayGanttChart(proc, n);
    
    free(proc);
    return 0;
}