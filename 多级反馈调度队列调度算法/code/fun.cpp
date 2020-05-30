#include"main.h"

int nowTime = 0; // 当前时间

//  输入数字
int inputInt(int min) {
	int a, result;
	fflush(stdin);
	a = scanf("%d",&result);
	if(a == 0)
	{
		printf("\n【输入数字】输入错误，请重新输入: ");
        result = inputInt(min);
	} else if (result < min) {
		printf("\n【输入数字】请输入大于%d的数字: ", min);
        result = inputInt(min);
	}
	return result;
}

// 初始化作业
void initTask(pTask &thead) {
	thead = (pTask)malloc(sizeof(Task));
	thead->next = NULL;
}

// 销毁作业
void destroyTask(pTask head) {
	pTask help;
	while (head != NULL) {
		help = head;
		head = head->next;
		free(help);
	}
}
// 新增作业
pTask newTask(char name[10], int arriveTime, int serverTime) {
	pTask p = (pTask)malloc(sizeof(Task));
	strcpy(p->name, name);
	p->arriveTime = arriveTime;
	p->serverTime = serverTime;
	p->remainTime = serverTime;
	p->status = 0;
	p->next = NULL;
	return p;
}
// 插入到作业队列
void addTask(pTask head, pTask p) {
	
	while(head->next != NULL) {
		head = head->next;
	}
	head->next = p;
}

// 打印一个链队列的作业
void printTasks(pTask head) {
	pTask p = head->next;
	if (p == NULL) {
		printf("\n 【空】打印作业为空");
	}
	printf("\n 作业名 到达时间 服务时间 剩余执行时间 作业状态");
	while(p != NULL) {
		printf("\n %3s   %4d    %4d     %6d       ", p->name, p->arriveTime, p->serverTime, p->remainTime);
		if(p->status == 0) {
			printf(" %4s", "未到达");
		} else if (p->status == 1) {
			printf(" %4s", "就绪中");
		} else if (p->status == 2) {
			printf(" %4s", "执行中");
		} else if (p->status == 3) {
			printf(" %4s", "已完成");
		}
		p = p->next;
	}	
	printf("\n");
}

// 初始化反馈队列
void initQueue(pQueue &qhead) {
	qhead = (pQueue)malloc(sizeof(Queue));
	qhead->t_head = NULL;
	qhead->next = NULL;
}

// 销毁反馈队列
void destroyQueue(pQueue head) {
	pQueue help;
	while (head != NULL) {
		help = head;
		head = head->next;
		destroyTask(help->t_head);
		free(help);
	}
}

// 添加反馈队列
void addQueue(pQueue head, int queue_ID, int priority, int serverTime) {
	pQueue p = (pQueue)malloc(sizeof(Queue));
	initTask(p->t_head);
	p->queue_ID = queue_ID;
	p->priority = priority;
	p->serverTime = serverTime;
	p->remainTime = serverTime;
	p->next = NULL;

	while(head->next != NULL) {
		head = head->next;
	}
	head->next = p;
}

// 打印反馈队列状态
void printQueue(pQueue head) {
	pQueue p = head->next;
	if (p == NULL) {
		printf("\n 【空】打印反馈队列为空");
	}
	printf("\n 队列id 优先级 全部时间片 剩余时间片 就绪作业名");
	while(p != NULL) {
		printf("\n %3d    %3d    %5d      %5d     ", p->queue_ID, p->priority, p->serverTime, p->remainTime);
		if(p->t_head->next == NULL) { 
			printf("%7s", "无");
		} else { 
			pTask p2 = p->t_head->next;
			while(p2 != NULL) {
				printf("%6s(剩%d秒)->", p2->name, p2->remainTime);
				p2= p2->next;
			}
		}
		p = p->next;
	}
}

// 插入作业到反馈队列
void InsertTaskToQueue(pQueue queue, pTask task) {
	pTask pro = queue->t_head;
	while(pro->next != NULL) {
		pro = pro->next;
	}
	pTask p = (pTask)malloc(sizeof(Task));
	strcpy(p->name, task->name);
	p->arriveTime = task->arriveTime;
	p->serverTime = task->serverTime;
	p->remainTime = task->remainTime;
	p->status = 1;
	p->next = NULL;
	pro->next = p;
}
// 判断是否有新的作业插入
void runNewTask(pQueue qhead, pTask thead) {
	pTask task;
	for (task = thead->next; task != NULL; task = task->next) {
		if (task->arriveTime == nowTime) {
			task->status = 1;
			task->arriveTime = nowTime;
			printf("\n【入队】：%s(需%d秒) 到达就绪", task->name, task->serverTime);
			InsertTaskToQueue(qhead->next, task);
		}
	}
	return;
	pQueue q = qhead->next;
}
// 判断是否继续执行
int ifContinueRun(pQueue qhead, pTask thead) {
	pTask p = thead->next;
	pQueue q = qhead->next;
	while(p != NULL) {
		// 还有未就绪的
		if(p->status != 1) {
			return 1;
		}
		p = p->next;
	}
	while(q != NULL) {
		// 还有未完成的
		if(q->t_head->next != NULL) {
			return 1;
		}
		q = q->next;
	}
	return 0;
}
// 获取当前应该运行的反馈队列
pQueue getRunQueue(pQueue qhead) {
	pQueue q, now;

	q = qhead->next;
	now = NULL;
	while(q != NULL) {
		if(q->t_head->next != NULL) {
			now = q;
			break;
		} 
		q = q->next;
	}
	return now;
}
// 执行一个时间片
void runOneTime(pQueue qhead, pTask thead) {
	runNewTask(qhead, thead);
	pQueue now = getRunQueue(qhead); // 执行的反馈队列
	if(now == NULL) {
		return;
	}
	pTask runTask = now->t_head->next; // 执行的作业
	printf("\n【执行】：%s(剩%d秒) 执行", runTask->name, runTask->remainTime);
	// 执行
	now->remainTime--;
	runTask->remainTime--;
	// 执行完后
	if(runTask->remainTime == 0) {
		// 该作业已经执行完毕
		now->t_head->next = runTask->next;
		printf(" --> 执行完毕，移出反馈队列");
		free(runTask);
		if(now->remainTime == 0) { // 时间片用完，重置
			now->remainTime = now->serverTime;
		}
	} else {
		// 该作业作业没执行完毕
		if(now->remainTime == 0) { // 时间片用完，重置
			now->remainTime = now->serverTime;
			now->t_head->next = runTask->next;
			// 把该作业放到下一个队列
			printf(" --> 没执行完，移入下一个反馈队列");
			InsertTaskToQueue(now->next, runTask);
		}
	}
}

// 执行
void run(pQueue qhead, pTask thead) {
	if(qhead == NULL || qhead->next == NULL) {
		printf("\n【请添加反馈队列】\n");
		return;
	}
	if(thead == NULL || thead->next == NULL) {
		printf("\n【无作业】\n");
		return;
	}
	while(ifContinueRun(qhead, thead) == 1) { // 还有作业没执行完
		
		printf("\n ————————————————————————");
		printf("\n【第 %d 秒】", nowTime);
		runOneTime(qhead, thead); // 执行一个时间片
		printf("\n【结果】：");
		printQueue(qhead);
		printf("\n ————————————————————————\n");
		nowTime++;
	}
	
}

void test(pQueue qhead, pTask thead) {

	// 初始化 队列
	initQueue(qhead);
	addQueue(qhead, 1, 1, 2);
	addQueue(qhead, 2, 2, 4);
	addQueue(qhead, 3, 3, 8);
	addQueue(qhead, 4, 4, 16);
	// 初始化作业
	initTask(thead);
	addTask(thead, newTask("作业1", 0, 3));
	addTask(thead, newTask("作业2", 1, 2));
	addTask(thead, newTask("作业3", 3, 1));
	addTask(thead, newTask("作业4", 3, 6));
	addTask(thead, newTask("作业5", 7, 2));
	
	// 开始跑作业
	run(qhead, thead);
	
	// 销毁数据，释放内存
	destroyTask(thead);
	destroyQueue(qhead);

	printf("\n\n【程序执行完毕，感谢使用】");

}