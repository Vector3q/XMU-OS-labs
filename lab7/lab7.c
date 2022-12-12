#include<stdio.h>
#include<stdbool.h>

#define MAXM 9999
int process_number;

struct PCB
{
	char PCB_Name[1];
	int PCB_ArrivalTime;
	int PCB_ServiceTime;
	int PCB_FinishTime;
	int PCB_TurnaroundTime;
	float PCB_Tr_Over_Ts;
};

void FCFS(struct PCB pcbarray[])
{
	int TotalTime = 0;
	for(int i=0; i<process_number; i++)
	{
		if(TotalTime < pcbarray[i].PCB_ArrivalTime)		//process may be not keep pace with the time
		{
			TotalTime = pcbarray[i].PCB_ArrivalTime;
		}
		TotalTime += pcbarray[i].PCB_ServiceTime;
		pcbarray[i].PCB_FinishTime = TotalTime;
		pcbarray[i].PCB_TurnaroundTime = pcbarray[i].PCB_FinishTime - pcbarray[i].PCB_ArrivalTime;
		pcbarray[i].PCB_Tr_Over_Ts = (float)pcbarray[i].PCB_TurnaroundTime / pcbarray[i].PCB_ServiceTime;
	}
}

void RR_q1(struct PCB pcbarray[])
{
	int TotalTime = 0;
	int process_finish_flag = 0;
	int running_process_number = 0;
	int running_process_index = 0;
	
	int time_served[process_number]; // -2:have finished, -1:haven't came, 0:just come, >0:served time
	
	int process_running_by_order[process_number];
	
	for(int i=0; i<process_number; i++)
	{
		time_served[i] = -1;
		process_running_by_order[i] = -1;
	}
	
	while(1)
	{
		int end_flag = 1;
		
		// just to find if there is a new process enter the ready queue.
		for(int i=0; i<process_number; i++)
		{
			if(TotalTime >= pcbarray[i].PCB_ArrivalTime && time_served[i] == -1)
			{
				if(process_finish_flag == 1)
				{
					process_running_by_order[running_process_number] = i;
					time_served[i] = 0;
					running_process_number++;
					continue;
				}
				// head insert the ready queue.
				time_served[i] = 0;
				process_running_by_order[running_process_number] = process_running_by_order[running_process_number - 1];
				process_running_by_order[running_process_number - 1] = i;
				running_process_number++;
			}
		}
		process_finish_flag = 0;
		
		// find the running process.
		running_process_index = process_running_by_order[0];
		//printf("now running process index: %d\n", running_process_index);

		// update the ready queue.
		for(int i=0; i<running_process_number; i++)
		{	
			process_running_by_order[i] = process_running_by_order[i+1];
		}
		
		TotalTime++;
		time_served[running_process_index] += 1;
		// owing to "q=1", there is no need to care about complex situation, such as "timeslice = 2", 2+2 > 3.
		// It's over just now. 
		if(time_served[running_process_index] >= pcbarray[running_process_index].PCB_ServiceTime)
		{
			pcbarray[running_process_index].PCB_FinishTime = TotalTime;
			pcbarray[running_process_index].PCB_TurnaroundTime = pcbarray[running_process_index].PCB_FinishTime - pcbarray[running_process_index].PCB_ArrivalTime;
			pcbarray[running_process_index].PCB_Tr_Over_Ts = (float)pcbarray[running_process_index].PCB_TurnaroundTime / pcbarray[running_process_index].PCB_ServiceTime;
			time_served[running_process_index] = -2;
			running_process_number--;
			process_finish_flag = 1;
		}
		else 
		{
			process_running_by_order[running_process_number-1] = running_process_index;
		}
		
		// just to confirm whether to the end.
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] != -2)
			{
				end_flag = 0;
				break;	
			}
		}
		
		if(end_flag)
			break;
	}
}

void SPN(struct PCB pcbarray[])
{
	int TotalTime = 0;
	int running_process_number = 0;
	int running_process_index = 0;
	
	int time_served[process_number]; // -2:have finished, -1:haven't came, 0:just come, >0:served time
	
	for(int i=0; i<process_number; i++)
	{
		time_served[i] = -1;
	}
	
	while(1)
	{
		int end_flag = 1;
		
		// just to find if there is a new process enter the ready queue.
		for(int i=0; i<process_number; i++)
		{
			if(TotalTime >= pcbarray[i].PCB_ArrivalTime && time_served[i] == -1)
			{
				time_served[i] = 0;
			}
		}
		
		// just to find the process whose service time is the minimum.
		int choice = -1;
		int min = MAXM;
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] == 0 && pcbarray[i].PCB_ServiceTime < min)
			{
				min = pcbarray[i].PCB_ServiceTime;			
				choice = i;
			}
		}
		
		// process!
		if(choice != -1)
		{
			TotalTime += pcbarray[choice].PCB_ServiceTime;
			pcbarray[choice].PCB_FinishTime = TotalTime;
			pcbarray[choice].PCB_TurnaroundTime = pcbarray[choice].PCB_FinishTime - pcbarray[choice].PCB_ArrivalTime;
			pcbarray[choice].PCB_Tr_Over_Ts = (float)pcbarray[choice].PCB_TurnaroundTime / pcbarray[choice].PCB_ServiceTime;
			time_served[choice] = -2;
		}
		
		// just to confirm whether to the end.
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] != -2)
			{
				end_flag = 0;
				break;	
			}
		}
		
		if(end_flag)
			break;
	}
}

void SRT(struct PCB pcbarray[])
{
	int TotalTime = 0;
	
	int time_served[process_number]; // -2:have finished, -1:haven't came, 0:just come, >0:served time
	
	for(int i=0; i<process_number; i++)
	{
		time_served[i] = -1;
	}
	
	while(1)
	{
		int end_flag = 1;
		
		// just to find if there is a new process enter the ready queue.
		for(int i=0; i<process_number; i++)
		{
			if(TotalTime >= pcbarray[i].PCB_ArrivalTime && time_served[i] == -1)
			{
				time_served[i] = 0;
			}
		}
		
		// just to find the process whose rest service time is the minimum.
		int choice = -1;
		int min = MAXM;
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] >= 0 && pcbarray[i].PCB_ServiceTime - time_served[i] < min)
			{
				min = pcbarray[i].PCB_ServiceTime - time_served[i];			
				choice = i;
			}
		}
		
		// process!
		if(choice != -1)
		{
			TotalTime++;
			time_served[choice]++;
			
			if(time_served[choice] == pcbarray[choice].PCB_ServiceTime)
			{
				pcbarray[choice].PCB_FinishTime = TotalTime;
				pcbarray[choice].PCB_TurnaroundTime = pcbarray[choice].PCB_FinishTime - pcbarray[choice].PCB_ArrivalTime;
				pcbarray[choice].PCB_Tr_Over_Ts = (float)pcbarray[choice].PCB_TurnaroundTime / pcbarray[choice].PCB_ServiceTime;
				time_served[choice] = -2;
			}
		}
		
		// just to confirm whether to the end.
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] != -2)
			{
				end_flag = 0;
				break;	
			}
		}
		
		if(end_flag)
			break;
	}
}

void HRRN(struct PCB pcbarray[])
{
	int TotalTime = 0;
	
	int time_served[process_number]; // -2:have finished, -1:haven't came, 0:just come, >0:served time
	
	float seraddwait_over_service[process_number];
	
	for(int i=0; i<process_number; i++)
	{
		time_served[i] = -1;
		seraddwait_over_service[i] = 0;
	}
	
	while(1)
	{
		int end_flag = 1;
		
		// just to find if there is a new process enter the ready queue.
		for(int i=0; i<process_number; i++)
		{
			if(TotalTime >= pcbarray[i].PCB_ArrivalTime && time_served[i] == -1)
			{
				time_served[i] = 0;
			}
			if(time_served[i] == 0)
			{
				seraddwait_over_service[i] = ((TotalTime - pcbarray[i].PCB_ArrivalTime) + pcbarray[i].PCB_ServiceTime ) / pcbarray[i].PCB_ServiceTime;
			}
		}
		
		// just to find the process whose service time is the minimum.
		int choice = -1;
		int max = 0;
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] == 0 && seraddwait_over_service[i] > max)
			{
				max = seraddwait_over_service[i];			
				choice = i;
			}
		}
		
		// process!
		if(choice != -1)
		{
			TotalTime += pcbarray[choice].PCB_ServiceTime;
			pcbarray[choice].PCB_FinishTime = TotalTime;
			pcbarray[choice].PCB_TurnaroundTime = pcbarray[choice].PCB_FinishTime - pcbarray[choice].PCB_ArrivalTime;
			pcbarray[choice].PCB_Tr_Over_Ts = (float)pcbarray[choice].PCB_TurnaroundTime / pcbarray[choice].PCB_ServiceTime;
			time_served[choice] = -2;
		}
		
		// just to confirm whether to the end.
		for(int i=0; i<process_number; i++)
		{
			if(time_served[i] != -2)
			{
				end_flag = 0;
				break;	
			}
		}
		
		if(end_flag)
			break;
	}
}

void ShowInfo(struct PCB pcbarrary[])
{
	printf("\t|------------------------Process Schedualing Time Information------------------------|\n");
	printf("\t| process name | arrival time | service time | finish time | turnaround time | Tr/Ts |\n");
	for(int i=0; i<process_number; i++)
	{
		printf("\t|                                                                                    |\n");
		printf("\t|\t%s\t       %d\t     %d\t\t   %d\t            %d\t\t%.2f |\n",pcbarrary[i].PCB_Name, pcbarrary[i].PCB_ArrivalTime, pcbarrary[i].PCB_ServiceTime, pcbarrary[i].PCB_FinishTime, pcbarrary[i].PCB_TurnaroundTime, pcbarrary[i].PCB_Tr_Over_Ts);
		printf("\t|                                                                                    |\n");
	}
	printf("\t|------------------------------------------------------------------------------------|\n");
}

int main(void)
{
	printf("\tPlease input the number of processes: \n\t");
	
	scanf("%d",&process_number);
	printf("\n");
	struct PCB pcb_array[process_number];
	
	printf("\tPlease input information of each process, just the process name, the service time and arrival time by order: \n");
	int count = process_number;

	for(int i=0; i<process_number; i++)
	{
		printf("\t");
		scanf("%s%d%d",&pcb_array[i].PCB_Name,&pcb_array[i].PCB_ArrivalTime,&pcb_array[i].PCB_ServiceTime);
		pcb_array[i].PCB_FinishTime = 0;
		pcb_array[i].PCB_TurnaroundTime = 0;
		pcb_array[i].PCB_Tr_Over_Ts = 0;	
	}
	printf("\n");
	
	int select;
	printf("\tThere are some method for processer schedualing \n\n");
	printf("\t1.FCFS 2.RR(q=1) 3.SPN 4.SRT 5.HRRN\n\n");
	printf("\tPlease select what you want: \n\t");
	scanf("%d",&select);
	printf("\n");
	
	if(select == 1)
		FCFS(pcb_array);
	else if(select == 2)
		RR_q1(pcb_array);
	else if(select == 3)
		SPN(pcb_array);
	else if(select == 4)
		SRT(pcb_array);
	else if(select == 5)
		HRRN(pcb_array);
	else
	{
		printf("Please input the correct option!\n");
		return 0;	
	}
	
	ShowInfo(pcb_array);
	
	return 0;
}