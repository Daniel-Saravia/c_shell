#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>

// Process states
typedef enum {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} ProcessState;

// Process structure
typedef struct {
    int process_id;
    ProcessState state;
    int burst_time; // For simplicity, assuming CPU burst time is known
} Process;

// Function prototypes
void initialize_scheduler();
bool add_process(Process p);
void execute_scheduler();
void process_state_transition(int process_id, ProcessState new_state);

#endif // SCHEDULER_H
