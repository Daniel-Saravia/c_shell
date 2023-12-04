#ifndef VMM_H
#define VMM_H

#include <stddef.h>
#include <stdint.h>

// Define the size of a memory page.
#define PAGE_SIZE 4096  // Page size in bytes.

// Define the size of the physical memory.
#define PHYS_MEM_SIZE 16 * PAGE_SIZE  // Physical memory size in bytes.

// Define the size of the virtual memory.
#define VIRT_MEM_SIZE 64 * PAGE_SIZE  // Virtual memory size in bytes.

// Structure to represent a page table entry.
typedef struct {
    unsigned int frame_number: 10;  // Frame number within physical memory (limited to 10 bits for small physical memory size).
    unsigned int valid: 1;          // Flag to indicate if the page is valid (in memory).
    unsigned int dirty: 1;          // Flag to indicate if the page has been modified.
    unsigned int accessed: 1;       // Flag to indicate if the page has been accessed.
} PageTableEntry;

// Structure to represent a page table.
typedef struct {
    PageTableEntry *entries;        // Pointer to an array of page table entries.
    unsigned int num_pages;         // Number of pages in the page table.
} PageTable;

// Structure to represent a process control block (PCB).
typedef struct {
    PageTable page_table;           // Page table for the process.
    unsigned int pid;               // Process ID.
    size_t memory_requirement;      // Total memory requirement of the process in bytes.
} PCB;

// Structure to represent the frame table.
typedef struct {
    int *frame_usage_count;         // Array to track the usage count of each frame.
    unsigned int num_frames;        // Total number of frames in physical memory.
} FrameTable;

// Function declarations for virtual memory management.

// Initializes the Virtual Memory Manager.
void initializeVMM();

// Creates a process with the given PID and memory size.
void createProcess(PCB *pcb, unsigned int pid, size_t memory_size);

// Allocates memory for a process.
void allocateMemory(PCB *pcb, size_t memory_size);

// Simulates memory access for a given virtual address in a process.
void accessMemory(PCB *pcb, unsigned int virtual_address);

// Frees a specified amount of memory from a process.
void freeMemory(PCB *pcb, size_t memory_to_free);

#endif // VMM_H

