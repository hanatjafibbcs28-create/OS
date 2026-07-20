Producer:
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    int shid, *buf, i, num;
    int size = 10;
    shid = shmget(4777, sizeof(int) * (size + 2), IPC_CREAT | 0666);
    if (shid == -1) {
        perror("shmget");
        return 1;
    }
    buf = (int *)shmat(shid, NULL, 0);
    if (buf == (void *)-1) {
        perror("shmat");
        return 1;
    }
    buf[10] = 0;
    buf[11] = 0; 
    for (i = 0; i < size; i++) {
        printf("Enter data: ");
        if (scanf("%d", &num) != 1) break;
        buf[buf[10]] = n
        buf[10] = (buf[10] + 1) % size;  
    }
    shmdt(buf);
    return 0;
}


Consumer:
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    int shmid, *buf, i, num;
    int size = 10;
    shmid = shmget(4777, sizeof(int) * (size + 2), 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }
    buf = (int *)shmat(shmid, NULL, 0);
    if (buf == (void *)-1) {
        perror("shmat");
        return 1;
    }
    printf("Data Read:\n");
    for (i = 0; i < size; i++) {
        num = buf[buf[11]];
        printf("%d\n", num);
        buf[11] = (buf[11] + 1) % size;
    }
    shmdt(buf);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

