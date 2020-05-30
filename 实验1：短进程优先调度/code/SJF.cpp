#include"SJF.h"




int nowTime = 0; // 当前时间
int pro_num = 0; // 执行进程个数



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

// 打印全部进程状态
void printAll(LinkedList link) {
	LinkedList p;
	printf("\n 第【 %d 】秒\n", nowTime);
	printf("\n  进程名称 到达时间 服务时间 剩余时间 开始时间 完成时间 周转时间 带权周转时间 进程状态");
	for(p = link->next; p != NULL; p = p->next) {
		printf("\n %5s  ", p->name);
		printf("  %5d  ", p->arriveTime);
		printf("  %5d  ", p->serverTime);
		printf("  %5d  ", p->remainTime);
		if (p->startTime == -1) {
			printf("   %5s ", "无");
		} else {
			printf("   %5d ", p->startTime);
		}
		if (p->finishTime == -1) {
			printf("   %5s ", "无");
		} else {
			printf("   %5d ", p->finishTime);
		}
		if (p->turnoverTime == -1) {
			printf("   %5s ", "无");
		} else {
			printf("   %5d ", p->turnoverTime);
		}
		if (p->weightTime == -1.0) {
			printf("   %7s   ", "无");
		} else {
			printf("   %7.3f   ", p->weightTime);
		}
		if (p->status == 0) {
			printf("    未到达  ");
		} else if (p->status == 1) {
			printf("    就绪中  ");
		} else if (p->status == 2) {
			printf("    执行中  ");
		} else if (p->status == 3) {
			printf("    已完成  ");
		}
	}
	printf("\n");
}

// 新进程的 [到达时间+服务时间] 来插入到进程链表
void insertLNode(LinkedList link, char name[10], int arriveTime, int serverTime) {
	LinkedList temp;
	LinkedList p = (LinkedList)malloc(sizeof(LNode));
	
	strcpy(p->name, name);
	p->arriveTime = arriveTime;
	p->remainTime = p->serverTime = serverTime;
	
	p->startTime = -1;
	p->finishTime = -1;
	p->turnoverTime = -1;
	p->weightTime = -1;
	p->status = 0;
	// p->next = NULL;

	p->next = link->next;
	link->next = p;

	return;

	for(temp = link; temp->next != NULL; temp = temp->next) {
		if (temp->next == NULL) {
			// 进程链表为空，直接插入第一个节点
			temp->next = p;
			break;
		} else {
			int pre_arriveTime = temp->next->arriveTime;
			int pre_serverTime = temp->next->serverTime;
			if (p->arriveTime < pre_arriveTime 
				|| (p->arriveTime == pre_arriveTime && p->serverTime < pre_serverTime)) {
				// 比较 到达时间+服务时间，选择插入位置
				p->next = temp->next;
				temp->next = p;

			}
			break;
		}
	}
	if(temp->next == NULL) {
		// 插入到队尾
		temp->next = p;
	}
}


// 添加一个进程
void addPro(LinkedList link) {
	
	int arriveTime, serverTime;
	char name[10];

	printf("  【添加】输入[进程名](char *)：");
	fflush(stdin);
	gets(name);

	printf("  【添加】输入[达到时间](int)：");
	arriveTime = inputInt(0) ;

	printf("  【添加】输入[服务时间](int)：");
	serverTime = inputInt(1);
	
	insertLNode(link, name, arriveTime, serverTime);
}

// 创建进程
void createPro(LinkedList &link) {
	// 初始化
	link = (LinkedList)malloc(sizeof(LNode));
	link->next = NULL;
	// 输入 pro_num 个进程
	printf("\n 【进程数量】 请输入你想创建的进程数：");
	pro_num = inputInt(1);
	for(int i = 0; i < pro_num; i++) {
		printf("\n 【创建进程】 正在创建第 %d 个进程\n", i);
		addPro(link);
	}
}

// 获取当前时间片应该执行的进程
LinkedList getPro(LinkedList waitArr[], int num) {
	if(num == 0) {
		// 没有就绪进程
		return NULL;
	}
	LinkedList result = waitArr[0];
	for(int i = 1; i < num; i++) {
		if(waitArr[i]->remainTime > result->remainTime) {
			continue;
		} else if(waitArr[i]->remainTime < result->remainTime) {
			result = waitArr[i];
		} else if(waitArr[i]->arriveTime < result->arriveTime) {
			result = waitArr[i];
		}
	}
	return result;
}

// 执行一个时间片
LinkedList runOneTime(LinkedList link) {
	int num = 0;
	LinkedList p, p2;
	LinkedList *waitArr = (LinkedList*)malloc(sizeof(LinkedList)*pro_num);
	// 获取就绪进程列表
	for(p = link->next; p != NULL; p = p->next) {
		if(p->status == 1) { 
			waitArr[num] = p;
			num++;
		}
	}
	// 时间片执行p2指向的进程
	p2 = getPro(waitArr, num);
	if(p2 != NULL) {
		if(p2->remainTime == p2->serverTime) {
			p2->startTime = nowTime;
		}
		p2->remainTime--;
		p2->status = 2; // p2进程正在执行
		
	} 
	
	return p2;
}
// 根据进程的到达时间和当前时间，判断是否推入新的进程
void pushPro(LinkedList link) {
	LinkedList p;
	// 判断是否有新的进程需要到达
	for(p = link->next; p != NULL; p = p->next) {
		if(p->arriveTime == nowTime) {
			p->status = 1; // 未到达-->就绪
			// break;
		}
	}
}

// 判断是否全部进程都执行完了
int ifAllFinish(LinkedList link) {
	int result = 1;
	LinkedList p;
	for(p = link->next; p != NULL; p = p->next) {
		if(p->status != 3) {
			result = 0;
			break;
		}
	}
	return result; // 0--> 未全部完成，1--> 已全部完成
}
// 执行完成后, 计算周转时间
void finished(LinkedList link) {
	LinkedList p;
	for(p = link->next; p != NULL; p = p->next) {
		p->turnoverTime = p->finishTime - p->arriveTime;
		p->weightTime = (float)p->turnoverTime / (float)p->serverTime;
	}
}

// 执行进程
void run(LinkedList link) {
	LinkedList p2;
	while(ifAllFinish(link) == 0) {
		pushPro(link); // 推入新进程
	
		p2 = runOneTime(link); // 执行一个时间片，并返回当前执行的进程
		
		printAll(link); // 打印第nowTime秒内的各个进程状态 
		nowTime ++;
		if(p2 != NULL) { // 将执行完的进程回归到 就绪/完成
			if(p2->remainTime == 0) {
				p2->status = 3;
				p2->finishTime = nowTime;
			} else {
				p2->status = 1;
			}
		}
	}
	finished(link);
	printAll(link);
}
