#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#ifndef SJF_H_INCLUDED
#define SJF_H_INCLUDED 

typedef struct LNode {

	char name[10]; //进程名称

	int status; //进程状态, 0->未到达，1->就绪，2->正在执行，3->完成

	int arriveTime; //到达时间

	int serverTime; //服务时间

	int remainTime; // 剩余执行时间

	int startTime; //开始时间

	int finishTime; //完成时间

	int turnoverTime; //周转时间(完成时间-到达时间)

	float weightTime; //带权周转时间(周转时间/服务时间)
	
	struct LNode* next; //下一个进程地址

} LNode, *LinkedList;


#endif
