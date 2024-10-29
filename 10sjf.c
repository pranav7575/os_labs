#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

void sortByArrival(struct Process *proc, int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(proc[j].arrival_time > proc[j+1].arrival_time) {
                struct Process temp = proc[j];
                proc[j] = proc[j+1];
                proc[j+1] = temp;
            }
        }
    }
}

void calculateNonPreemptiveSJF(struct Process *proc, int n) {
    int current_time = 0;
    int completed = 0;
    int *is_completed = (int*)calloc(n, sizeof(int));

    printf("\nNon-Preemptive SJF Scheduling:\n");

    while(completed != n) {
        int shortest_job = -1;
        int min_burst = __INT_MAX__;

        // Find process with minimum burst time among arrived processes
        for(int i = 0; i < n; i++) {
            if(proc[i].arrival_time <= current_time && !is_completed[i]) {
                if(proc[i].burst_time < min_burst) {
                    min_burst = proc[i].burst_time;
                    shortest_job = i;
                }
            }
        }

        if(shortest_job == -1) {
            current_time++;
            continue;
        }

        // Execute the process
        current_time += proc[shortest_job].burst_time;
        proc[shortest_job].completion_time = current_time;
        proc[shortest_job].turnaround_time = proc[shortest_job].completion_time - proc[shortest_job].arrival_time;
        proc[shortest_job].waiting_time = proc[shortest_job].turnaround_time - proc[shortest_job].burst_time;
        is_completed[shortest_job] = 1;
        completed++;

        printf("Process %d completed at time %d\n", proc[shortest_job].pid, current_time);
    }

    free(is_completed);
}

void calculatePreemptiveSJF(struct Process *proc, int n) {
    int current_time = 0;
    int completed = 0;
    int *is_completed = (int*)calloc(n, sizeof(int));
    
    // Initialize remaining time
    for(int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
    }

    printf("\nPreemptive SJF (SRTF) Scheduling:\n");

    while(completed != n) {
        int shortest_job = -1;
        int min_remaining = __INT_MAX__;

        // Find process with minimum remaining time among arrived processes
        for(int i = 0; i < n; i++) {
            if(proc[i].arrival_time <= current_time && !is_completed[i]) {
                if(proc[i].remaining_time < min_remaining) {
                    min_remaining = proc[i].remaining_time;
                    shortest_job = i;
                }
            }
        }

        if(shortest_job == -1) {
            current_time++;
            continue;
        }

        // Execute process for 1 time unit
        proc[shortest_job].remaining_time--;
        current_time++;

        if(proc[shortest_job].remaining_time == 0) {
            proc[shortest_job].completion_time = current_time;
            proc[shortest_job].turnaround_time = proc[shortest_job].completion_time - proc[shortest_job].arrival_time;
            proc[shortest_job].waiting_time = proc[shortest_job].turnaround_time - proc[shortest_job].burst_time;
            is_completed[shortest_job] = 1;
            completed++;

            printf("Process %d completed at time %d\n", proc[shortest_job].pid, current_time);
        }
    }

    free(is_completed);
}

void displayResults(struct Process *proc, int n, const char* type) {
    float avg_turnaround = 0, avg_waiting = 0;
    
    printf("\n%s Results:\n", type);
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               proc[i].pid,
               proc[i].arrival_time,
               proc[i].burst_time,
               proc[i].completion_time,
               proc[i].turnaround_time,
               proc[i].waiting_time);
               
        avg_turnaround += proc[i].turnaround_time;
        avg_waiting += proc[i].waiting_time;
    }
    
    avg_turnaround /= n;
    avg_waiting /= n;
    
    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround);
    printf("Average Waiting Time: %.2f\n", avg_waiting);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    struct Process *proc = (struct Process*)malloc(n * sizeof(struct Process));
    struct Process *proc_copy = (struct Process*)malloc(n * sizeof(struct Process));
    
    // Input process details
    for(int i = 0; i < n; i++) {
        printf("\nFor Process %d:\n", i+1);
        proc[i].pid = i+1;
        printf("Enter Arrival Time: ");
        scanf("%d", &proc[i].arrival_time);
        printf("Enter Burst Time: ");
        scanf("%d", &proc[i].burst_time);
    }
    
    // Make a copy for preemptive scheduling
    for(int i = 0; i < n; i++) {
        proc_copy[i] = proc[i];
    }
    
    // Sort processes by arrival time
    sortByArrival(proc, n);
    sortByArrival(proc_copy, n);
    
    // Calculate and display Non-Preemptive SJF
    calculateNonPreemptiveSJF(proc, n);
    displayResults(proc, n, "Non-Preemptive SJF");
    
    // Calculate and display Preemptive SJF (SRTF)
    calculatePreemptiveSJF(proc_copy, n);
    displayResults(proc_copy, n, "Preemptive SJF (SRTF)");
    
    free(proc);
    free(proc_copy);
    
    return 0;
}