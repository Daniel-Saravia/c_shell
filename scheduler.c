#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

// Process queue
static Process process_queue[MAX_PROCESSES];
static int queue_size = 0;
static int current_index = 0; // For Round-Robin scheduling

// Initialize the scheduler
void initialize_scheduler() {
    queue_size = 0;
    current_index = 0;
}

// Add a process to the scheduler
bool add_process(Process p) {
    if (queue_size >= MAX_PROCESSES) {
        return false;
    }
    process_queue[queue_size++] = p;
    return true;
}

// Execute the scheduler - simple Round-Robin for demonstration
void execute_scheduler() {
    if (queue_size == 0) {
        return;
    }

    // Select the next process
    Process *current_process = &process_queue[current_index];
    current_process->state = RUNNING;

    // Simulate process execution
    printf("Running process: %d\n", current_process->process_id);
    current_process->burst_time--; // Decrement burst time

    // Check if the process is completed
    if (current_process->burst_time <= 0) {
        current_process->state = TERMINATED;
        printf("Process %d terminated\n", current_process->process_id);
    } else {
        current_process->state = READY;
    }

    // Move to the next process
    current_index = (current_index + 1) % queue_size;
}

// Change the state of a process
void process_state_transition(int process_id, ProcessState new_state) {
    for (int i = 0; i < queue_size; i++) {
        if (process_queue[i].process_id == process_id) {
            process_queue[i].state = new_state;
            break;
        }
    }
}

// List all processes in the scheduler
void list_all_processes() {
    for (int i = 0; i < queue_size; i++) {
        printf("Process ID: %d, State: %d, Burst Time: %d\n", 
               process_queue[i].process_id, 
               process_queue[i].state, 
               process_queue[i].burst_time);
    }
}

// Show the current process
void show_current_process() {
    if (queue_size > 0) {
        Process current_process = process_queue[current_index];
        printf("Current Process ID: %d, State: %d, Burst Time: %d\n", 
               current_process.process_id, 
               current_process.state, 
               current_process.burst_time);
    } else {
        printf("No current process.\n");
    }
}

