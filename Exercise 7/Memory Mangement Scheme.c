#include <stdio.h>
#include <stdbool.h>
#define MAX_BLOCKS 20
#define MAX_PROCESSES 20
// Structure to represent a physical Memory Block (Hole)
typedef struct {
    int block_id;
    int total_size;
    int allocated_process_id; // -1 if the block is free
    int fragment_size;        // Internal fragmentation within this block
    bool is_allocated;
} Block;
// Structure to represent a Process requesting memory
typedef struct {
    int process_id;
    int required_size;
    bool is_allocated;        // Tracks if the process successfully got memory
} Process;
// Global Arrays & Variables
Block blocks[MAX_BLOCKS];
Block backup_blocks[MAX_BLOCKS]; // To reset memory pool between runs
Process processes[MAX_PROCESSES];
int num_blocks = 0;
int num_processes = 0;
// Initialize blocks and processes via user input
void initialize_system() {
    int i,j;
    printf("Enter the number of memory blocks (Holes): ");
    scanf("%d", &num_blocks);
    for (i = 0; i < num_blocks; i++) {
        blocks[i].block_id = i + 1;
        printf("  Enter size for Hole H%d (in KB): ", i + 1);
        scanf("%d", &blocks[i].total_size);
        blocks[i].allocated_process_id = -1;
        blocks[i].fragment_size = 0;
        blocks[i].is_allocated = false;
        // Backup for reset
        backup_blocks[i] = blocks[i];
    }
    printf("\nEnter the number of processes: ");
    scanf("%d", &num_processes);
    for (i = 0; i < num_processes; i++) {
        processes[i].process_id = i + 1;
        printf("  Enter memory required for Process P%d (in KB): ", i + 1);
        scanf("%d", &processes[i].required_size);
        processes[i].is_allocated = false;
    }
}
// Reset block allocation statuses for testing multiple strategies
void reset_blocks() {
    int i;
    for (i = 0; i < num_blocks; i++) {
        blocks[i] = backup_blocks[i];
    }
    for (i = 0; i < num_processes; i++) {
        processes[i].is_allocated = false;
    }
}
// 1. First-Fit Strategy
void first_fit() {
    int i,j;
    printf("\n--- Allocation Logs (First-Fit) ---\n");
    for (i = 0; i < num_processes; i++) {
        for (j = 0; j < num_blocks; j++) {
            if (!blocks[j].is_allocated && blocks[j].total_size >= processes[i].required_size) {
                blocks[j].is_allocated = true;
                blocks[j].allocated_process_id = processes[i].process_id;
                blocks[j].fragment_size = blocks[j].total_size - processes[i].required_size;
                processes[i].is_allocated = true;
                printf("Process P%d is allocated to Hole H%d\n", processes[i].process_id, blocks[j].block_id);
                break;
            }
        }
    }
}
// 2. Best-Fit Strategy
void best_fit() {
    int i,j;
    printf("\n--- Allocation Logs (Best-Fit) ---\n");
    for (i = 0; i < num_processes; i++) {
        int best_idx = -1;
        for (j = 0; j < num_blocks; j++) {
            if (!blocks[j].is_allocated && blocks[j].total_size >= processes[i].required_size) {
                if (best_idx == -1 || blocks[j].total_size < blocks[best_idx].total_size) {
                    best_idx = j;
                }
            }
        }
        if (best_idx != -1) {
            blocks[best_idx].is_allocated = true;
            blocks[best_idx].allocated_process_id = processes[i].process_id;
            blocks[best_idx].fragment_size = blocks[best_idx].total_size - processes[i].required_size;
            processes[i].is_allocated = true;
            printf("Process P%d is allocated to Hole H%d\n", processes[i].process_id, blocks[best_idx].block_id);
        }
    }
}
// 3. Worst-Fit Strategy
void worst_fit() {
    int i,j;
    printf("\n--- Allocation Logs (Worst-Fit) ---\n");
    for (i = 0; i < num_processes; i++) {
        int worst_idx = -1;
        for (j = 0; j < num_blocks; j++) {
            if (!blocks[j].is_allocated && blocks[j].total_size >= processes[i].required_size) {
                if (worst_idx == -1 || blocks[j].total_size > blocks[worst_idx].total_size) {
                    worst_idx = j;
                }
            }
        }
        if (worst_idx != -1) {
            blocks[worst_idx].is_allocated = true;
            blocks[worst_idx].allocated_process_id = processes[i].process_id;
            blocks[worst_idx].fragment_size = blocks[worst_idx].total_size - processes[i].required_size;
            processes[i].is_allocated = true;
            printf("Process P%d is allocated to Hole H%d\n", processes[i].process_id, blocks[worst_idx].block_id);
        }
    }
}
// Display function to show the resulting allocation state and track fragmentation
void display_system_status() {
    int i,j;
    int total_internal_fragmentation = 0;
    int total_external_fragmentation = 0;
    bool any_unallocated = false;
    printf("\n==================== MEMORY ALLOCATION TABLE ====================\n");
    printf("%-10s %-12s %-12s %-15s %-10s\n", "Hole ID", "Hole Size", "Process ID", "Process Size", "Internal Frag");
    printf("------------------------------------------------------------------\n");
    for (j = 0; j < num_blocks; j++) {
        if (blocks[j].is_allocated) {
            int proc_size = 0;
            for (i = 0; i < num_processes; i++) {
                if (processes[i].process_id == blocks[j].allocated_process_id) {
                    proc_size = processes[i].required_size;
                    break;
                }
            }
            printf("H%-9d %-12d P%-11d %-15d %-10d\n",
                   blocks[j].block_id, blocks[j].total_size,
                   blocks[j].allocated_process_id, proc_size,
                   blocks[j].fragment_size);
           
            total_internal_fragmentation += blocks[j].fragment_size;
        } else {
            printf("H%-9d %-12d %-12s %-15s %-10d\n",
                   blocks[j].block_id, blocks[j].total_size, "Free", "-", 0);
           
            total_external_fragmentation += blocks[j].total_size;
        }
    }
    printf("==================================================================\n");
    printf("\n--- Unallocated Processes ---\n");
    for (i = 0; i < num_processes; i++) {
        if (!processes[i].is_allocated) {
            printf("Process P%d (Requires %d KB) -> Waiting\n", processes[i].process_id, processes[i].required_size);
            any_unallocated = true;
        }
    }
    if (!any_unallocated) {
        printf("None. All processes successfully allocated.\n");
        total_external_fragmentation = 0; // No process left stranded, external fragmentation is zero
    }
    printf("\n--- Fragmentation Summary ---\n");
    printf("Total Internal Fragmentation: %d KB\n", total_internal_fragmentation);
    printf("Total External Fragmentation: %d KB\n", total_external_fragmentation);
    printf("---------------------------------------\n");
}
int main() {
    int choice;
    printf("--- Contiguous Memory Management System Simulator ---\n\n");
    initialize_system();
    while (1) {
        printf("\nSelect Allocation Strategy:\n");
        printf("1. First-Fit\n");
        printf("2. Best-Fit\n");
        printf("3. Worst-Fit\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 4) {
            printf("Exiting simulator.\n");
            break;
        }
        reset_blocks(); // Clear previous allocations before testing a new strategy

        switch (choice) {
            case 1:
                first_fit();
                display_system_status();
                break;
            case 2:
                best_fit();
                display_system_status();
                break;
            case 3:
                worst_fit();
                display_system_status();
                break;
            default:
                printf("Invalid choice! Please select an option between 1 and 4.\n");
        }
    }

    return 0;
}
