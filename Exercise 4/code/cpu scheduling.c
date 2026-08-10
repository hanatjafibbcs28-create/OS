#include <stdio.h>
#include <stdlib.h>
struct Process {
    int id;
    int at;
    int bt;
    int rt;
    int pr;
    int ct;
    int tat;
    int wt;
};
void show(struct Process p[], int n) {
    float total_tat = 0, total_wt = 0;
    int i;
    printf("\nID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].pr, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("Average TAT = %.2f\n", total_tat / n);
    printf("Average WT = %.2f\n", total_wt / n);
}
void fcfs(struct Process p[], int n) {
    int i, j;
    int time = 0;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j+1].at) {
                struct Process temp = p[j]; p[j] = p[j+1]; p[j+1] = temp;
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        p[i].ct = time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        time = p[i].ct;
    }
    printf("\n--- FCFS ---");
    show(p, n);
}
void sjf(struct Process p[], int n) {
    int done = 0, time = 0;
    int vis[50] = {0}; 
    int i;
    for (i = 0; i < n; i++) { p[i].ct = 0; p[i].tat = 0; p[i].wt = 0; }
    while (done != n) {
        int idx = -1, min_bt = 10000;
        for (i = 0; i < n; i++) {
            if (p[i].at <= time && !vis[i]) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            p[idx].ct = time + p[idx].bt;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            vis[idx] = 1;
            done++;
            time = p[idx].ct;
        } else {
            time++;
        }
    }
    printf("\n--- SJF ---");
    show(p, n);
}
void srtf(struct Process p[], int n) {
    int done = 0, time = 0;
    int i;
    for (i = 0; i < n; i++) { p[i].rt = p[i].bt; p[i].ct = 0; p[i].tat = 0; p[i].wt = 0; }
    while (done != n) {
        int idx = -1, min_rt = 10000;
        for (i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].rt < min_rt) {
                    min_rt = p[i].rt;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            p[idx].rt--;
            if (p[idx].rt == 0) {
                p[idx].ct = time + 1;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                done++;
            }
            time++;
        } else {
            time++;
        }
    }
    printf("\n--- SRTF ---");
    show(p, n);
}
void priority_np(struct Process p[], int n) {
    int done = 0, time = 0;
    int vis[50] = {0};
    int i;
    for (i = 0; i < n; i++) { p[i].ct = 0; p[i].tat = 0; p[i].wt = 0; }
    while (done != n) {
        int idx = -1, min_pr = 10000; 
        for (i = 0; i < n; i++) {
            if (p[i].at <= time && !vis[i]) {
                if (p[i].pr < min_pr) {
                    min_pr = p[i].pr;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            p[idx].ct = time + p[idx].bt;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            vis[idx] = 1;
            done++;
            time = p[idx].ct;
        } else {
            time++;
        }
    }
    printf("\n--- Priority (Non-Preemptive) ---");
    show(p, n);
}
void priority_p(struct Process p[], int n) {
    int done = 0, time = 0;
    int i;
    for (i = 0; i < n; i++) { p[i].rt = p[i].bt; p[i].ct = 0; p[i].tat = 0; p[i].wt = 0; }
    while (done != n) {
        int idx = -1, min_pr = 10000;
        for (i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].pr < min_pr) {
                    min_pr = p[i].pr;
                    idx = i;
                }
            }
        }
        if (idx != -1) {
            p[idx].rt--;
            if (p[idx].rt == 0) {
                p[idx].ct = time + 1;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                done++;
            }
            time++;
        } else {
            time++;
        }
    }
    printf("\n--- Priority (Preemptive) ---");
    show(p, n);
}
void rr(struct Process p[], int n, int tq) {
    int time = 0, done = 0;
    int q[500], f = 0, r = 0; 
    int vis[50] = {0};
    int i, j;
    for (i = 0; i < n; i++) { p[i].rt = p[i].bt; p[i].ct = 0; p[i].tat = 0; p[i].wt = 0; }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j+1].at) {
                struct Process temp = p[j]; p[j] = p[j+1]; p[j+1] = temp;
            }
        }
    }
    time = p[0].at;
    q[r++] = 0;
    vis[0] = 1;
    while (f < r) {
        int idx = q[f++];
        int run = (p[idx].rt > tq) ? tq : p[idx].rt;
        p[idx].rt -= run;
        time += run;
        for (i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && !vis[i]) {
                q[r++] = i;
                vis[i] = 1;
            }
        }
        if (p[idx].rt > 0) {
            q[r++] = idx;
        } else {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            done++;
        }
        if (f == r && done < n) {
            for (i = 0; i < n; i++) {
                if (p[i].rt > 0) {
                    time = p[i].at;
                    q[r++] = i;
                    vis[i] = 1;
                    break;
                }
            }
        }
    }
    printf("\n--- Round Robin ---");
    show(p, n);
}
int main() {
    int n, tq, choice;
    int i;
    struct Process p[50];
        printf("\n===== CPU SCHEDULING MENU =====");
        printf("\n1. First-Come, First-Served (FCFS)");
        printf("\n2. Shortest Job First (SJF)");
        printf("\n3. Shortest Remaining Time First (SRTF)");
        printf("\n4. Priority (Non-Preemptive)");
        printf("\n5. Priority (Preemptive)");
        printf("\n6. Round Robin (RR)");
        printf("\n7. Exit");
    while(1){
        printf("\nEnter your choice (1-7): ");
        scanf("%d", &choice);
        if (choice == 7) {
            printf("Exiting program...\n");
            exit(0);
        }
        if (choice >= 1 && choice <= 6) {
            printf("Enter the total number of processes: ");
            scanf("%d", &n);
            if (choice == 6) {
                printf("Enter Time Quantum for Round Robin: ");
                scanf("%d", &tq);
            }
            printf("\nEnter Process Details (Lower Priority Number = Higher Priority):\n");
            for (i = 0; i < n; i++) {
                p[i].id = i + 1;
                printf("\nProcess %d:\n", p[i].id);
                printf("  Arrival Time: ");
                scanf("%d", &p[i].at);
                printf("  Burst Time: ");
                scanf("%d", &p[i].bt);
                printf("  Priority: ");
                scanf("%d", &p[i].pr);
                while (getchar() != '\n');
            }
        }
        switch (choice) {
            case 1:
                fcfs(p, n);
                break;
            case 2:
                sjf(p, n);
                break;
            case 3:
                srtf(p, n);
                break;
            case 4:
                priority_np(p, n);
                break;
            case 5:
                priority_p(p, n);
                break;
            case 6:
                rr(p, n, tq);
                break;
            default:
                printf("Invalid choice! Please choose between 1 and 7.\n");
        }
    }
    return 0;
}
