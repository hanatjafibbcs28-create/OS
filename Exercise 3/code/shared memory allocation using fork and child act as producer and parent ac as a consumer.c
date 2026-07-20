#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define SIZE 10
struct shared {
    int buffer[SIZE];
    int in;
    int out;
    int count;
};
int main() {
    int shmid;
    struct shared *shm;
    pid_t pid;
    shmid = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Shared memory creation failed");
        exit(1);
    }
    shm = (struct shared *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }
    shm->in = 0;
    shm->out = 0;
    shm->count = 0;
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid == 0) {
        int input[SIZE] = {10, 20, 30, 40, 50, 60, 65, 70, 80, 85};
        printf("Producer (Child) Started:\n");
        for (int i = 0; i < SIZE; i++) {
            if (shm->count == SIZE) {
                printf("Buffer Full\n");
                break;
           }
            shm->buffer[shm->in] = input[i];
            printf("Produced %d at index %d\n", input[i], shm->in);
            shm->in = (shm->in + 1) % SIZE;
            shm->count++;
        }
        shmdt(shm);
        exit(0);

    } else {
        wait(NULL);
        printf("\nConsumer (Parent) Started:\n");
        while (shm->count > 0) {
            printf("Consumed %d from index %d\n", shm->buffer[shm->out], shm->out);
            shm->out = (shm->out + 1) % SIZE; 
            shm->count--;
        }
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}
