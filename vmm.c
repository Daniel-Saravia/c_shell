#include "vmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Frame table to keep track of frame usage.
static FrameTable frame_table;

// Initializes the Virtual Memory Manager.
void initializeVMM() {
    // Calculate the number of frames based on physical memory size and page size.
    frame_table.num_frames = PHYS_MEM_SIZE / PAGE_SIZE;
    // Allocate and initialize frame usage count to zero for each frame.
    frame_table.frame_usage_count = (int *)calloc(frame_table.num_frames, sizeof(int));
    if (!frame_table.frame_usage_count) {
        // If allocation fails, display an error message and exit.
        perror("Failed to initialize frame usage count");
        exit(EXIT_FAILURE);
    }
}

// Creates a process with the specified PID and memory size.
void createProcess(PCB *pcb, unsigned int pid, size_t memory_size) {
    pcb->pid = pid;
    // Allocate memory for the process.
    allocateMemory(pcb, memory_size);
    // Print process creation information.
    printf("Process %u created with %zu bytes of memory, requiring %u pages.\n", pcb->pid, memory_size, pcb->page_table.num_pages);
}

// Allocates additional memory to a process.
void allocateMemory(PCB *pcb, size_t additional_memory_size) {
    // Calculate new memory requirement.
    size_t new_memory_requirement = pcb->memory_requirement + additional_memory_size;
    // Calculate the number of pages needed.
    unsigned int new_num_pages = (new_memory_requirement + PAGE_SIZE - 1) / PAGE_SIZE;

    // Check if additional pages are needed.
    if(new_num_pages > pcb->page_table.num_pages) {
        // Reallocate the page table to accommodate the new pages.
        PageTableEntry *new_entries = (PageTableEntry *)realloc(pcb->page_table.entries, new_num_pages * sizeof(PageTableEntry));
        if (!new_entries) {
            perror("Failed to allocate additional page table entries");
            exit(EXIT_FAILURE);
        }

        // Initialize the new page table entries.
        for(unsigned int i = pcb->page_table.num_pages; i < new_num_pages; ++i) {
            new_entries[i].valid = 0;
            new_entries[i].dirty = 0;
            new_entries[i].accessed = 0;
        }

        pcb->page_table.entries = new_entries;
        pcb->page_table.num_pages = new_num_pages;
    }

    pcb->memory_requirement = new_memory_requirement;
    printf("Process %u memory increased by %zu bytes, now requiring a total of %u pages.\n", pcb->pid, additional_memory_size, pcb->page_table.num_pages);
}

// Accesses a virtual address within a process's memory.
void accessMemory(PCB *pcb, unsigned int virtual_address) {
    // Calculate the page number and offset from the virtual address.
    unsigned int page_number = virtual_address / PAGE_SIZE;
    unsigned int offset = virtual_address % PAGE_SIZE;

    // Check if the page number is valid.
    if (page_number >= pcb->page_table.num_pages) {
        printf("Error: The virtual address %u is out of bounds for process %u.\n", virtual_address, pcb->pid);
        return;
    }

    PageTableEntry *entry = &pcb->page_table.entries[page_number];
    // Handle page fault if the page is not valid.
    if (!entry->valid) {
        printf("Page fault for process %u at virtual address %u: Page %u not in physical memory.\n", pcb->pid, virtual_address, page_number);
        // Load the page into memory (simulated here).
        entry->valid = 1;
        entry->frame_number = page_number % frame_table.num_frames; // Simple mapping example.
        printf("Page %u loaded into frame %u for process %u.\n", page_number, entry->frame_number, pcb->pid);
    }

    entry->accessed = 1;
    // Calculate the physical address from the page number and offset.
    unsigned int physical_address = (entry->frame_number * PAGE_SIZE) + offset;
    printf("Virtual address %u translated to physical address %u for process %u.\n", virtual_address, physical_address, pcb->pid);
}

// Frees a specified amount of memory from a process.
void freeMemory(PCB *pcb, size_t memory_to_free) {
    // Ensure the requested amount to free does not exceed the allocated memory.
    if(memory_to_free > pcb->memory_requirement) {
        printf("Error: Cannot free %zu bytes from process %u. Only %zu bytes are currently allocated.\n", memory_to_free, pcb->pid, pcb->memory_requirement);
        return;
    }

    // Decrease the process's memory requirement.
    pcb->memory_requirement -= memory_to_free;
    // Calculate the new number of pages required.
    unsigned int new_num_pages = (pcb->memory_requirement + PAGE_SIZE - 1) / PAGE_SIZE;

    // Check if the number of pages can be reduced.
    if(new_num_pages < pcb->page_table.num_pages) {
        // Reduce the page table size.
        PageTableEntry *new_entries = (PageTableEntry *)realloc(pcb->page_table.entries, new_num_pages * sizeof(PageTableEntry));
        if (!new_entries && new_num_pages > 0) {
            perror("Failed to reallocate the page table entries");
            exit(EXIT_FAILURE);
        }

        pcb->page_table.entries = new_entries;
        pcb->page_table.num_pages = new_num_pages;
    }

    printf("Freed %zu bytes of memory from process %u. %u pages remaining.\n", memory_to_free, pcb->pid, pcb->page_table.num_pages);
}

// Cleans up the VMM by freeing the frame table.
void cleanupVMM() {
    free(frame_table.frame_usage_count);
}


