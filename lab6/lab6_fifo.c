# include<stdio.h>
# include<stdbool.h>
# include<string.h>
# define MAXM 1000

int page_visiting_number;				 // set the number of page to work
int frame_number = 3;					 // set the frame number
int page_hit_number = 0;
int page_fault_number;
float page_hit_ratio;
float page_fault_ratio;

struct PageFrame
{
	int pageid; 						 // working page's id
	int page_stay_time; 				 // the next time page will work
}PageFrame;

void updetaPageStayTime(struct PageFrame page_frame[])
{
	for(int j = 0; j<frame_number; j++)
	{
		page_frame[j].page_stay_time++;
	}
}

int findStayLongestFrameID(struct PageFrame page_frame[])
{
	int Stay_Longest_Time = 0;
	int Stay_Longest_id = 0;
	for(int i=0; i<frame_number; i++)
	{
		if(page_frame[i].pageid == -1)
		{
			return i;
		}	

		if(Stay_Longest_Time < page_frame[i].page_stay_time)
		{
			Stay_Longest_Time = page_frame[i].page_stay_time;
			Stay_Longest_id = i;
		}
	}
	return Stay_Longest_id;
}

void fifo(struct PageFrame page_frame[])
{
	
	printf("Welcome to use fifo algorithm. \nNow, just input the number of pages to work\n");
	scanf("%d",&page_visiting_number);
	
	printf("Well, just input the pages' id by order\n");
	int visiting_pageid[page_visiting_number];
	for(int i=0; i<page_visiting_number; i++)
	{
		scanf("%d",&visiting_pageid[i]);
	}
	
	int reverse_number = page_visiting_number;			// just a help parameter
	while(reverse_number--)
	{
		int hit_flag = 0;
		// PF hit
		for(int i=0; i<frame_number; i++)
		{
			if(page_frame[i].pageid == visiting_pageid[page_visiting_number - reverse_number - 1])
			{
				printf("Page frame:%d has hit, page id:%d \n",i,page_frame[i].pageid);
				hit_flag = 1;
				updetaPageStayTime(page_frame);
			}	
		}
		
		if(hit_flag)
		{
			page_hit_number++;
			continue;
		}
		
		//PF fault
		int replace_id = findStayLongestFrameID(page_frame);
		page_frame[replace_id].pageid = visiting_pageid[page_visiting_number - reverse_number - 1];
		page_frame[replace_id].page_stay_time = 0;
		printf("Page frame:%d has repalaced, new working page id:%d\n",replace_id,page_frame[replace_id].pageid);
		updetaPageStayTime(page_frame);
	}
	
	printf("The number of page frame fault is %d \n", page_visiting_number - page_hit_number);
	printf("The ratio of page frame fault is %f \n", (float)(page_visiting_number - page_hit_number) / page_visiting_number);
	printf("The ratio of page frame hit is %f \n", (float)page_hit_number/page_visiting_number);
	printf("Goodbye!\n");
}

int main()
{
	struct PageFrame page_frame[frame_number];
	for(int i=0; i<frame_number; i++)
	{
		page_frame[i].pageid = -1;
		page_frame[i].page_stay_time = 0;
	}
	fifo(page_frame);
	
	return 0;
}