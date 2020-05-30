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

	int finishTime; // 截止时间

	int remainTime; // 剩余执行时间
	
	int relaxation; // 松弛度

	int status; // 状态，0->未到达，1->就绪，2->执行中，3->正常完成，4->超时完成
	
	struct Task* next; // 下一个作业地址

} Task, *TaskList;



#endif
