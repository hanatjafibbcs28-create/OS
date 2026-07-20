#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) {
        wait(NULL);
        close(fd[1]); 
        int received_nums[2];
        read(fd[0], received_nums, sizeof(received_nums));
        close(fd[0]);
        int a = received_nums[0];
        int b = received_nums[1];
        printf("[P1] Received numbers from pipe: %d and %d\n", a, b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        printf("[P1] Calculated GCD: %d\n", a);
    } else {
        close(fd[0]);
        int user_input[2];
        printf("[P2] Enter 2 numbers: ");
        scanf("%d %d", &user_input[0], &user_input[1]);
        write(fd[1], user_input, sizeof(user_input));
        close(fd[1]);
        exit(0);
    }
    return 0;
}

