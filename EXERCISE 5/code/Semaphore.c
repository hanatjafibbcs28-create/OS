semaphore.h:

#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#define SIZE 5          // Buffer capacity
// --- Semaphores for Producer -> Consumer ---
#define PC_MUTEX 0
#define PC_FULL  1
#define PC_EMPTY 2
// --- Semaphores for Consumer -> Producer ---
#define CP_MUTEX 3
#define CP_FULL  4
#define CP_EMPTY 5
#define TOTAL_SEMS 6
#define SHM_KEY 6770    
#define SEM_KEY 5770    
// Shared structural layout mapped inside the shared memory segment
struct SharedBuffer {
    int pc_data[SIZE];  // Producer writes here, Consumer reads
    int cp_data[SIZE];  // Consumer writes here, Producer reads
};
union semaphore {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
void wait_sem(int sid, int sno);
void signal_sem(int sid, int sno);
#endif

semaphore.c:

#include "semaphore.h" 
void wait_sem(int sid, int sno) {
    struct sembuf s;
    s.sem_num = sno;
    s.sem_op = -1;
    s.sem_flg = SEM_UNDO;
    if (semop(sid, &s, 1) == -1)
 {
        perror("wait_sem failed");
        exit(1);
    }
}
void signal_sem(int sid, int sno) {
    struct sembuf s;
    s.sem_num = sno;
    s.sem_op = 1;
    s.sem_flg = SEM_UNDO;
    if (semop(sid, &s, 1) == -1) {
        perror("signal_sem failed");
        exit(1);
    }
}


producer.c:

#include "semaphore.h"
int main() {
    union semaphore snum;
    int sid, s, i;
    int pc_in = 0, cp_out = 0;
    int input_num, response_num;
    struct SharedBuffer *shared_mem;
    unsigned short a[TOTAL_SEMS];
    // Allocate the dual-channel shared memory structural frame
    s = shmget(SHM_KEY, sizeof(struct SharedBuffer), IPC_CREAT | 0666);
    if (s == -1) { perror("producer shmget"); exit(1); }
    shared_mem = (struct SharedBuffer *)shmat(s, NULL, 0);
    if (shared_mem == (void *)-1) { perror("producer shmat"); exit(1); }
    // Initialize 6 total semaphore elements
    sid = semget(SEM_KEY, TOTAL_SEMS, IPC_CREAT | 0666);
    if (sid == -1) { perror("producer semget"); exit(1); }
    // Setup Channel 1: Producer -> Consumer
    a[PC_MUTEX] = 1; a[PC_FULL] = 0; a[PC_EMPTY] = SIZE;
    // Setup Channel 2: Consumer -> Producer
    a[CP_MUTEX] = 1; a[CP_FULL] = 0; a[CP_EMPTY] = SIZE;
    snum.array = a;
    semctl(sid, 0, SETALL, snum);
    printf("=== TWO-WAY CHAT: PRODUCER MODE ===\n");
    for (i = 0; i < SIZE; i++) {
        // 1. Send data to Consumer
        printf("\n[P -> C] Enter number to send: ");
        if (scanf("%d", &input_num) != 1) break;
        wait_sem(sid, PC_EMPTY);
        wait_sem(sid, PC_MUTEX);
        shared_mem->pc_data[pc_in % SIZE] = input_num;
        pc_in++;
        signal_sem(sid, PC_MUTEX);
        signal_sem(sid, PC_FULL);
        // 2. Await and Read data back from Consumer
        printf("[Producer] Waiting for Consumer response...\n");
        wait_sem(sid, CP_FULL);
        wait_sem(sid, CP_MUTEX);
        response_num = shared_mem->cp_data[cp_out % SIZE];
        printf("[C -> P] Received message from Consumer: %d\n", response_num);
        cp_out++;
        signal_sem(sid, CP_MUTEX);
        signal_sem(sid, CP_EMPTY);
    }
    shmdt(shared_mem);
    return 0;
}


Consumer.c:

#include "semaphore.h"
int main() {
    int sid, s, i;
    int pc_out = 0, cp_in = 0;
    int received_num, reply_num;
    struct SharedBuffer *shared_mem;
    s = shmget(SHM_KEY, sizeof(struct SharedBuffer), 0666);
    if (s == -1) { perror("consumer shmget"); exit(1); }
    shared_mem = (struct SharedBuffer *)shmat(s, NULL, 0);
    if (shared_mem == (void *)-1) { perror("consumer shmat"); exit(1); }
    sid = semget(SEM_KEY, TOTAL_SEMS, 0666);
    if (sid == -1) { perror("consumer semget"); exit(1); }
    printf("=== TWO-WAY CHAT: CONSUMER MODE ===\n");
    for (i = 0; i < SIZE; i++) {
        // 1. Wait and Read from Producer
        printf("\n[Consumer] Awaiting Producer data...\n");
        wait_sem(sid, PC_FULL);
        wait_sem(sid, PC_MUTEX);
        received_num = shared_mem->pc_data[pc_out % SIZE];
        printf("[P -> C] Received from Producer: %d\n", received_num);
        pc_out++;
        signal_sem(sid, PC_MUTEX);
        signal_sem(sid, PC_EMPTY);
        // 2. Generate a reply message and send to Producer
        reply_num = received_num * 2; // Auto-reply with double value
        printf("[C -> P] Replying back with calculated value: %d\n", reply_num);
        wait_sem(sid, CP_EMPTY);
        wait_sem(sid, CP_MUTEX);
        shared_mem->cp_data[cp_in % SIZE] = reply_num;
        cp_in++;
        signal_sem(sid, CP_MUTEX);
        signal_sem(sid, CP_FULL);   
        sleep(1); 
    }
    // Execution wrapped up: Purge System Resources from Linux Engine
    shmdt(shared_mem);
    shmctl(s, IPC_RMID, NULL);
    semctl(sid, 0, IPC_RMID);
    printf("\nTwo-Way communication ended. Shared structures cleared.\n");
    return 0;
}
