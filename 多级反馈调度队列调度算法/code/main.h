#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#ifndef SJF_H_INCLUDED
#define SJF_H_INCLUDED 

// 作业
typedef struct Task {
	
	char name[10]; // 作业名称

	int arriveTime; // 到达时间

	int serverTime; // 服务时间

	int remainTime; // 剩余执行时间

	int status; // 是否入队，0->未入队，1->已入队
	
	struct Task* next; // 下一个作业地址

} Task, *pTask;

// 反馈队列
typedef struct Queue {

	int queue_ID; // 队列id

	pTask t_head; // 就绪作业队列

	int priority; // 优先级，数字越小等级越高

	int serverTime; // 全部时间片

	int remainTime; // 剩余时间片
	
	struct Queue* next; // 下一个作业地址

} Queue, *pQueue;


#endif
