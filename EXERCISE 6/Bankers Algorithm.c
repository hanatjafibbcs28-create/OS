#include <stdio.h>

#define MAX_P 20
#define MAX_R 20

int processes, resources;
int allocation[MAX_P][MAX_R];
int maximum[MAX_P][MAX_R];
int need[MAX_P][MAX_R];
int total[MAX_R];
int available[MAX_R];

void calculateNeed()
{
    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

void calculateAvailable()
{
    for (int j = 0; j < resources; j++)
    {
        int allocated = 0;

        for (int i = 0; i < processes; i++)
        {
            allocated += allocation[i][j];
        }

        available[j] = total[j] - allocated;
    }
}

void displayMatrix(char name[], int matrix[MAX_P][MAX_R])
{
    printf("\n%s\t", name);

    for (int j = 0; j < resources; j++)
    {
        printf("R%d\t", j);
    }

    printf("\n");

    for (int i = 0; i < processes; i++)
    {
        printf("P%d\t", i);

        for (int j = 0; j < resources; j++)
        {
            printf("%d\t", matrix[i][j]);
        }

        printf("\n");
    }
}

int isSafe()
{
    int work[MAX_R];
    int finish[MAX_P] = {0};
    int safeSequence[MAX_P];
    int count = 0;

    for (int j = 0; j < resources; j++)
    {
        work[j] = available[j];
    }

    printf("\nInitial Work: [ ");

    for (int j = 0; j < resources; j++)
    {
        printf("%d ", work[j]);
    }

    printf("]\n");

    while (count < processes)
    {
        int found = 0;

        for (int i = 0; i < processes; i++)
        {
            if (finish[i] == 0)
            {
                int possible = 1;

                for (int j = 0; j < resources; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        possible = 0;
                        break;
                    }
                }

                if (possible)
                {
                    for (int j = 0; j < resources; j++)
                    {
                        work[j] += allocation[i][j];
                    }

                    safeSequence[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found)
        {
            printf("\n>> UNSAFE STATE (Risk of Deadlock) <<\n");
            return 0;
        }
    }

    printf("\n>> SAFE STATE <<\n");
    printf("Safe Sequence: ");

    for (int i = 0; i < processes; i++)
    {
        printf("P%d", safeSequence[i]);

        if (i != processes - 1)
        {
            printf(" -> ");
        }
    }

    printf("\n");

    return 1;
}

void requestResources()
{
    int processNumber;
    int request[MAX_R];

    printf("\nEnter process number: ");
    scanf("%d", &processNumber);

    if (processNumber < 0 || processNumber >= processes)
    {
        printf("Invalid process number.\n");
        return;
    }

    printf("Enter resource request vector:\n");

    for (int j = 0; j < resources; j++)
    {
        scanf("%d", &request[j]);
    }

    for (int j = 0; j < resources; j++)
    {
        if (request[j] > need[processNumber][j])
        {
            printf("Error: Request exceeds the process's maximum need.\n");
            return;
        }

        if (request[j] > available[j])
        {
            printf("Resources are not currently available.\n");
            return;
        }
    }

    /*
       Temporarily allocate the requested resources
    */
    for (int j = 0; j < resources; j++)
    {
        available[j] -= request[j];
        allocation[processNumber][j] += request[j];
        need[processNumber][j] -= request[j];
    }

    if (isSafe())
    {
        printf("Request can be granted safely.\n");
    }
    else
    {
        /*
           Roll back if the state becomes unsafe
        */
        for (int j = 0; j < resources; j++)
        {
            available[j] += request[j];
            allocation[processNumber][j] -= request[j];
            need[processNumber][j] += request[j];
        }

        printf("Request denied because it makes the system unsafe.\n");
    }
}

void displayMatrices()
{
    printf("\nAvailable: ");

    for (int j = 0; j < resources; j++)
    {
        printf("%d ", available[j]);
    }

    printf("\n");

    displayMatrix("Max", maximum);
    displayMatrix("Allocation", allocation);
    displayMatrix("Need", need);
}

int main()
{
    int choice;

    printf("Enter number of processes: ");
    scanf("%d", &processes);

    printf("Enter number of resources: ");
    scanf("%d", &resources);

    if (processes <= 0 || processes > MAX_P ||
        resources <= 0 || resources > MAX_R)
    {
        printf("Invalid number of processes or resources.\n");
        return 1;
    }

    printf("\nEnter Allocation Matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("\nEnter Max Matrix:\n");

    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            scanf("%d", &maximum[i][j]);
        }
    }

    printf("\nEnter Total Instances Vector:\n");

    for (int j = 0; j < resources; j++)
    {
        scanf("%d", &total[j]);
    }

    calculateNeed();
    calculateAvailable();

    do
    {
        printf("\n1. Check Safety\n");
        printf("2. Request Resources\n");
        printf("3. Display Matrices\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                isSafe();
                break;

            case 2:
                requestResources();
                break;

            case 3:
                displayMatrices();
                break;

            case 4:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);

    return 0;
}
