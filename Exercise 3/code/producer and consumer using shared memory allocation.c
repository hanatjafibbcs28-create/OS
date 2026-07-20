Producer:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe1[2];
    int pipe2[2],i;
    pipe(pipe1);
    pipe(pipe2);
    if (fork() > 0) {
        wait(NULL);
        int even_count;
        read(pipe2[0], &even_count, sizeof(int));
        if (even_count < 1) {
            printf("[P1] No even numbers received.\n");
            return 0;
        }
        int arr[even_count];
        read(pipe2[0], arr, sizeof(int) * even_count); 
        printf("[P1] Received Even Elements: ");
        for (i = 0; i < even_count; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        if (even_count < 2) {
            printf("[P1] Cannot find second minimum with only 1 even number.\n");
            return 0;
        }
        int first_min = arr[0];
        int second_min = arr[0];
        int found_distinct = 0;
        for ( i = 1; i < even_count; i++) {
            if (arr[i] < first_min) {
                second_min = first_min;
                first_min = arr[i];
                found_distinct = 1;
            } 
            else if ((arr[i] < second_min || first_min == second_min) && arr[i] != first_min) {
                second_min = arr[i];
                found_distinct = 1;
            }
        }
        if (found_distinct && first_min != second_min) {
            printf("[P1] Second minimum even number is: %d\n", second_min);
        } else {
            printf("[P1] No distinct second minimum element found (all values are identical).\n");
        }
    } 
    else {
        if (fork() > 0) {
            wait(NULL);
            int total_count;
            read(pipe1[0], &total_count, sizeof(int));
            int arr[total_count];
            read(pipe1[0], arr, sizeof(int) * total_count);
            int even_arr[total_count];
            int even_count = 0;
            for ( i = 0; i < total_count; i++) {
                if (arr[i] % 2 == 0) {
                    even_arr[even_count] = arr[i];
                    even_count++;
                }
            }
            write(pipe2[1], &even_count, sizeof(int));          
            write(pipe2[1], even_arr, sizeof(int) * even_count);
            return 0;
        } 
        else {
            int count;
            printf("[P3] Enter the no.of.elements:");
            scanf("%d", &count);
            int arr[count];
            printf("[P3] Enter %d numbers:\n", count);
            for ( i = 0; i < count; i++) {
                scanf("%d", &arr[i]);
            }
            write(pipe1[1], &count, sizeof(int));    
            write(pipe1[1], arr, sizeof(int) * count);   
            return 0;
        }
    }
    return 0;
}




Consumer:

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 10       
#define KEY 4777     

int main() {
    int s1, d;
    int *buf;
    int num;
    int out = 0;
    int i; // Declaring 'i' explicitly here

    // Locate the existing shared memory segment
    s1 = shmget(KEY, sizeof(int) * SIZE, 0600);
    if (s1 == -1) {
        perror("shmget failed (Is the producer running?)");
        exit(1);
    }

    // Attach the shared memory segment
    buf = (int *)shmat(s1, NULL, 0);
    if (buf == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("\nConsumer reading data from shared memory:\n");

    // Read data from the circular buffer
    for (i = 0; i < SIZE; i++) {
        num = buf[out % SIZE];  
        printf("Data at index [%d]: %d\n", out, num);
        out++;
    }

    // Detach from the segment
    shmdt(buf);

    // Delete and destroy the shared memory segment
    d = shmctl(s1, IPC_RMID, NULL);
    if (d == -1) {
        perror("shmctl RMID failed");
        exit(1);
    }

    printf("Shared memory cleaned up successfully.\n");
    return 0;
}
