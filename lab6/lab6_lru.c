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
	int last_occur_index; 				 // the next time page will work
}PageFrame;

int findOccurLastOccurIndexID(struct PageFrame page_frame[])
{
	int Last_index = MAXM;
	int Frame_id = 0;
	for(int i=0; i<frame_number; i++)
	{
		if(page_frame[i].pageid == -1)
		{
			return i;
		}	
		//printf("%d and %d\n",Farest_index,page_frame[i].next_occur_index);
		if(Last_index > page_frame[i].last_occur_index)
		{
			Last_index = page_frame[i].last_occur_index;
			Frame_id = i;
		}
	}
	return Frame_id;
}

void lru(struct PageFrame page_frame[])
{
	
	printf("Welcome to use lru algorithm. \nNow, just input the number of pages to work\n");
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
				page_frame[i].last_occur_index = page_visiting_number - reverse_number;
			}	
		}
		
		if(hit_flag)
		{
			page_hit_number++;
			continue;
		}
		
		//PF fault
		int replace_id = findOccurLastOccurIndexID(page_frame);
		
		page_frame[replace_id].pageid = visiting_pageid[page_visiting_number - reverse_number - 1];
		page_frame[replace_id].last_occur_index = page_visiting_number - reverse_number;
		printf("Page frame:%d has repalaced, new working page id:%d\n",replace_id,page_frame[replace_id].pageid);

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
		page_frame[i].last_occur_index = 0;
	}
	lru(page_frame);
	
	return 0;
}