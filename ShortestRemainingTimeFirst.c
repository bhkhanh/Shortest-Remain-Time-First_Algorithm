/*#######################################################
# University of Information Technology                  #
# Operating System                                      #
# File: ShortestRemainingTimeFirst.c                    #
# Shortest Remaining Time First algorithm [Preemptive]  #
#######################################################*/
//Done (missing Gantt Chart)

#include <stdio.h>
#include <stdbool.h>

struct ProcessControlBlock
{
    int processName, arrivalTime, burstTime;
    int waitingTime, turnaroundTime, responseTime;
    int startTime;
};

//for users to input information about PCB (process order, arrival time & burst time)
void Input_PCB(int numberProcess, struct ProcessControlBlock pcb[])
{
    for (int i = 0; i < numberProcess; i++)
    {
        printf("Enter the Process Name: ");
        scanf("%d", &pcb[i].processName);

        printf("Enter the Arrival Time: ");
        scanf("%d", &pcb[i].arrivalTime);

        printf("Enter the Burst Time: ");
        scanf("%d", &pcb[i].burstTime);

        printf("\n");
    }
}

//sort all processes according to arrival time (using selection sort algorithm)
void Sorting_ArrivalTime(int numberProcess, struct ProcessControlBlock pcb[])
{
    struct ProcessControlBlock temporary;
    int minimum;
    for (int i = 0; i < numberProcess - 1; i++)
    {
        minimum = i;
        for (int j = i + 1; j < numberProcess; j++)
        {
            if (pcb[j].arrivalTime < pcb[minimum].arrivalTime)
                minimum = j;
        }
        
        //swapping
        temporary = pcb[minimum];
        pcb[minimum] = pcb[i];
        pcb[i] = temporary;
    }
}

//this function will get the total of all burst time of all processes
int totalBurstTime(int numberProcess, struct ProcessControlBlock pcb[])
{
    int sum = 0;
    for (int i = 0; i < numberProcess; i++)
    {
        sum += pcb[i].burstTime;
    }
    return sum;
}

//calculate waiting time and response time of each process
void Calculating_WaitingTime(int numberProcess, struct ProcessControlBlock pcb[])
{
    int remainingTime[numberProcess];
    for (int i = 0; i < numberProcess; i++)
    {
        remainingTime[i] = pcb[i].burstTime;
    }
    
    int complete = 0; 
    int currentTime = 0;
    int currentBurst = totalBurstTime(numberProcess, pcb);
    int currentProcess = 0;
    int finishTime;
    bool check = false;

    while (complete != numberProcess)
    {
        for (int j = 0; j < numberProcess; j++)
        {
            if (pcb[j].arrivalTime <= currentTime && remainingTime[j] < currentBurst && remainingTime[j] > 0)
            {
                currentBurst = remainingTime[j];
                currentProcess = j;
                check = true;
            }
        }
        
        if (check == false)
        {
            currentTime++;
            continue;
        }

        if (remainingTime[currentProcess] == pcb[currentProcess].burstTime)
            pcb[currentProcess].startTime = currentTime;

        remainingTime[currentProcess]--;
        currentBurst = remainingTime[currentProcess];
        if (currentBurst == 0)
            currentBurst = totalBurstTime(numberProcess, pcb);

        if (remainingTime[currentProcess] == 0)
        {
            complete++;
            check = false;
            finishTime = currentTime + 1;
            pcb[currentProcess].waitingTime = finishTime - pcb[currentProcess].burstTime - pcb[currentProcess].arrivalTime;
            pcb[currentProcess].responseTime = pcb[currentProcess].startTime - pcb[currentProcess].arrivalTime;
            if (pcb[currentProcess].waitingTime < 0)
                pcb[currentProcess].waitingTime = 0;
        }
        currentTime++;
    }
}

//calculate turnaround time of each process
void Calculating_TurnaroundTime(int numberProcess, struct ProcessControlBlock pcb[])
{
    for (int i = 0; i < numberProcess; i++)
    {
        pcb[i].turnaroundTime = pcb[i].burstTime + pcb[i].waitingTime;
    }
}

//for users to see information about PCB
void Output_PCB(int numberProcess, struct ProcessControlBlock pcb[])
{
    printf("[*] PCB Before Sorting & Calculating [*] \n");
    printf("-------------------------------------------- \n");
    printf("\tSHORTEST REMAINING TIME FIRST \n");
    printf("-------------------------------------------- \n");
    printf("Process Name \t Arrival Time \t Burst Time \n");
    for (int i = 0; i < numberProcess; i++)
    {
        printf("%d \t\t %d \t\t %d", pcb[i].processName, pcb[i].arrivalTime, pcb[i].burstTime);
        printf("\n");
    }
    printf("-------------------------------------------- \n");

    //==================================================================================================

    Sorting_ArrivalTime(numberProcess, pcb);
    Calculating_WaitingTime(numberProcess, pcb);
    Calculating_TurnaroundTime(numberProcess, pcb);

    printf("\n[*] PCB After Sorting & Calculating [*] \n");
    printf("-------------------------------------------------------------------------------------------------- \n");
    printf("\t\t\t\t SHORTEST REMAINING TIME FIRST \n");
    printf("-------------------------------------------------------------------------------------------------- \n");
    printf("Process Name \t Arrival Time \t Burst Time \t Waiting Time \t Response Time \t Turnaround Time \n");
    for (int i = 0; i < numberProcess; i++)
    {
        printf("%d \t\t %d \t\t %d \t\t %d \t\t %d \t\t %d", pcb[i].processName, pcb[i].arrivalTime, pcb[i].burstTime, pcb[i].waitingTime, pcb[i].responseTime, pcb[i].turnaroundTime);
        printf("\n");
    }
    printf("-------------------------------------------------------------------------------------------------- \n");
}

//calculate average of waiting time or turnaround time (depends on 'which' variable)
double Calculating_Average(int numberProcess, struct ProcessControlBlock pcb[], int which)
{
    //which = 1 will calculate average waiting time
    //which = 2 will calculate average turnaround time
    int sum;
    switch (which)
    {
    case 1: //average waiting time
        sum = 0;
        for (int i = 0; i < numberProcess; i++)
        {
            sum += pcb[i].waitingTime;
        }
        return (float)sum / (float)numberProcess;
        break;
    
    case 2: //average turnaround time
        sum = 0;
        for (int i = 0; i < numberProcess; i++)
        {
            sum += pcb[i].turnaroundTime;
        }
        return (float)sum / (float)numberProcess;
        break;

    default:
        return 0;
        break;
    }
}


void main()
{
    int numberProcess;
    printf("\nPlease enter total number of processes that you want: ");
    scanf("%d", &numberProcess);
    printf("\n");

    struct ProcessControlBlock pcb[numberProcess];

    Input_PCB(numberProcess, pcb);
    printf("\n");
    Output_PCB(numberProcess, pcb);
    printf("\n");

    printf("Average of all Waiting Time is %.2lf \n", Calculating_Average(numberProcess, pcb, 1));
    printf("Average of all Turnaround Time is %.2lf \n\n", Calculating_Average(numberProcess, pcb, 2));

}