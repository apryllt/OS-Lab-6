// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
#include <string.h>


// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
  /*
     1. Create an array *rem_bt[]* to keep track of remaining burst time of processes. This array is initially a copy of *plist[].bt* (all processes burst times)
     2. Store waiting times of processes in plist[].wt. Initialize this array as 0.
     3. Initialize time : t = 0
     4. Keep traversing the all processes while all processes are not done. Do following for i'th process if it is not done yet.
        - If rem_bt[i] > quantum
          (i)  t = t + quantum
          (ii) bt_rem[i] -= quantum;
        - Else // Last cycle for this process
          (i)  t = t + bt_rem[i];
          (ii) plist[i].wt = t - plist[i].bt
          (iii) bt_rem[i] = 0; // This process is over
   */  
  int *rem_bt = malloc(n * sizeof(int));
  for(int i = 0; i<n; i++){
    rem_bt[i] = plist[i].bt;
    plist[i].wt = 0;
  }


  int current_time = 0;
  int completed = 0;
  
  while (completed < n){
    int did_run = 0; //checks if any process ran in the current pass

    for (int i = 0; i < n; i++){
      //check if process is not completed
      if (rem_bt[i] == 0){
        continue;
      }

      //check if the process has arrived
      if (plist[i].art > current_time){
        continue;
      }

      did_run = 1;

      if (rem_bt[i] > quantum){
        //run process for the full quantum
        current_time += quantum;
        rem_bt[i] -= quantum;
      }
      else{
        //run for the remianing burst time
        current_time += rem_bt[i];
        plist[i].wt = current_time - plist[i].bt - plist[i].art;
        rem_bt[i] = 0;
        completed++;
      }
    }
    if (did_run == 0 && completed < n){
      current_time++;
    }
  }
  free(rem_bt);
   
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
      /*
       * 1 Traverse until all process gets completely executed.
         - Find process with minimum remaining time at every single time lap.
         - Reduce its time by 1.
         - Check if its remaining time becomes 0 
         - Increment the counter of process completion.
         - Completion time of *current process = current_time +1;*
         - Calculate waiting time for each completed process. *wt[i]= Completion time - arrival_time-burst_time*
         - Increment time lap by one.
 
     */
    //Array holds the remaining burst time
    int *rem_bt = (int *)malloc(n * sizeof(int));
    for (int i = 0; i<n; i++){
      rem_bt[i] = plist[i].bt;
    }

    int current_time = 0;
    int completed = 0;
    int min_bt;
    int shortest_proc_idx;
    int finish_time;

    while (completed < n){
      //Find process with the minimum remianing time
      min_bt = INT_MAX;
      shortest_proc_idx = -1;

      for (int i = 0; i<n; i++){
        if (plist[i].art <= current_time && rem_bt[i] > 0 && rem_bt[i] < min_bt){
          min_bt = rem_bt[i];
          shortest_proc_idx = i;
        }
      }

      //handle idle time
      if (shortest_proc_idx == -1){
        current_time++;
        continue;
      }

      rem_bt[shortest_proc_idx]--;
      current_time++;

      if (rem_bt[shortest_proc_idx] == 0){
        completed++;
        finish_time = current_time;

        //calculate waiting time
        plist[shortest_proc_idx].wt = finish_time - plist[shortest_proc_idx].bt - plist[shortest_proc_idx].art;

        if (plist[shortest_proc_idx].wt < 0){
          plist[shortest_proc_idx].wt = 0;
        }
      }
    }
    free(rem_bt);
    
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
  if (n == 0) return;

  //completion time of the previous process
  int prev_comp_time = 0; 

  for (int  i = 0; i < n ; i++ ) {
    //calculate the start time
    int start_time = prev_comp_time;

    //checks if CPU is idle
    if (plist[i].art > prev_comp_time){
      start_time = plist[i].art;
    }
    
    //calculate the waiting time
    plist[i].wt = start_time - plist[i].art;

    //update the completion time for the next process 
    prev_comp_time = start_time + plist[i].bt;
  }
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
    /*  
     * 1. Cast this and that into (ProcessType *)
     * 2. return 1 if this->pri < that->pri
     */ 
    ProcessType *p1 = (ProcessType*) this;
    ProcessType *p2 = (ProcessType*) that;

    if (p1->pri != p2->pri){
      return p2->pri - p1->pri;
    } 

    //tie-breaker if priorities are the start_time
    return p2->art - p1->art;
}
    

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
   /*
    * 1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
    * 2- Now simply apply FCFS algorithm.
    */

    //Function to sort the processes by priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 