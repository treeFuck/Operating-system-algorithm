#include "Replace.h"

int task[320]; // 存放指令的数组
int taskIndex; // 当前执行指令 task[taskIndex]
int nowTime; // 记录时间
int pageNum; // 当前内存页数
int interrupt; // 缺页中断次数


//随机生成320个随机数
void randomNum() {
	int flag = 0;
	srand((unsigned)time(NULL));   //系统时间为随机数种子 
	int m = rand() % 320;
	for (int i = 0; i < 320; i++)
	{
		task[i] = m;
		if (flag % 2 == 0) m = ++m % 320;
		if (flag == 1) m = rand() % (m - 1);
		if (flag == 3) m = m + 1 + (rand() % (320 - (m + 1)));
		flag = ++flag % 4;
	}
}

// 初始化内存链表
void init(LinkedList &head) {
	taskIndex = 0; 
	nowTime = 1; 
	pageNum = 0; 
	interrupt = 0;
	head = (LinkedList)malloc(sizeof(LNode));
	head->next = NULL;
}

// 销毁内存链表
void distory(LinkedList &head) {
	LinkedList p = head;
	while (p != NULL) {
		head = head->next;
		free(p);
		p = head;
	}
	head = NULL;
}

// 打印内存链表
void print(LinkedList head, int type) {
	LinkedList p;
	printf("\n 【内存状况】：");
	printf("\n  ——————————");
	if (type == 0) {
		// 先进先出（FIFO）
		printf ("\n  页码 进入时间 ");
		for(p = head->next; p != NULL; p = p->next) {
			printf("\n %4d %6d", p->page, p->inTime);
		}
	} else if (type == 1) {
		// 最近最久未使用（LRU）
		printf ("\n  页码 进入时间 未使用次数");
		for(p = head->next; p != NULL; p = p->next) {
			printf("\n %4d %6d %8d", p->page, p->inTime, p->unUsedTime);
		}
	} else if (type == 2) {
		// 最佳置换（OPT）
		int i = 1;
		printf ("\n  编码 页码");
		for(p = head->next; p != NULL; p = p->next) {
			printf("\n %4d %4d", i, p->page);
			i++;
		}
	}
	printf("\n  ——————————\n ");
}


// 【先进先出（FIFO）】缺页中断时，将指令页调入内存
void FIFO(LinkedList head, LinkedList temp) {
	LinkedList p, pro;
	int minInTime = head->next->inTime; // 最早进入时间

	for(p = pro = head; p->next != NULL; p = p->next) {
		if (p->next->inTime < minInTime) {
			minInTime = p->next->inTime;
			pro = p;
		}
	}
	// pro 为最早进入的页的上一个节点
	printf("\n 【页面置换】：置换第 %d 页为第 %d 页", pro->next->page, temp->page);
	temp->next = pro->next->next;
	free(pro->next);
	pro->next = temp;
}

// 【最近最久未使用（LRU）】缺页中断时，将指令页调入内存
void LRU(LinkedList head, LinkedList temp) {
	LinkedList p, pro;
	int minInTime = head->next->inTime; // 最早进入时间
	int maxUnUsedTime = head->next->unUsedTime; // 最大未使用次数

	for(p = pro = head; p->next != NULL; p = p->next) {
		if (p->next->unUsedTime > maxUnUsedTime) {
			maxUnUsedTime = p->next->unUsedTime;
			minInTime = p->next->inTime;
			pro = p;
		} else if (p->next->unUsedTime == maxUnUsedTime) {
			if (p->next->inTime < minInTime) {
				minInTime = p->next->inTime;
				pro = p;
			}
		}
	}
	// pro 为最近最久未使用的页的上一个节点
	printf("\n 【页面置换】：置换第 %d 页为第 %d 页", pro->next->page, temp->page);
	temp->next = pro->next->next;
	free(pro->next);
	pro->next = temp;
	
}

// 【最佳置换（OPT）】缺页中断时，将指令页调入内存
void OPT(LinkedList head, LinkedList temp) {
	LinkedList p, pro;
	int minUseTime = 320; // 预计最少使用次数
	for(p = head->next; p != NULL; p = p->next) {
		p->useTime = 0;
	}
	for (int i = taskIndex + 1; i < 320; i++) {
		for(p = head->next; p != NULL; p = p->next) {
			if (p->page == task[i]/10) {
				p->useTime ++;
			}
		}
	}
	for(p = head; p->next != NULL; p = p->next) {
		printf("\n   第 %d 页预计将被调用 %d 次数", p->next->page, p->next->useTime);
		if (p->next->useTime < minUseTime ) {
			minUseTime = p->next->useTime;
			pro = p;
		}
	}
	printf("\n 【页面置换】：置换第 %d 页为第 %d 页", pro->next->page, temp->page);
	temp->next = pro->next->next;
	free(pro->next);
	pro->next = temp;
}

// 获取指令所在页的内存指针
LinkedList getLnodeP(LinkedList head, int page) {
	LinkedList p;
	for(p = head->next; p != NULL; p = p->next) {
		if (p->page == page) {
			break;
		}
	}
	return p;
}

// 执行一条指令
void runTask(LinkedList head, int task, int type) {
	LinkedList p = getLnodeP(head, task/10);
	// 缺页中断
	if(p == NULL) {
		printf("【缺页中断】");
		// 初始化新页
		p = (LinkedList)malloc(sizeof(LNode));
		p->page = task/10;
		p->inTime = nowTime;
		p->unUsedTime = 0;
		p->next = NULL;
		p->useTime = 0;
		// 调用新页
		if(pageNum < 16) {
			printf("\n 【调用新页】：调用第 %d 页", p->page);
			p->next = head->next;
			head->next = p;
			pageNum ++;
		} else {
			if (type == 0) {
				FIFO(head, p);
			} else if (type == 1) {
				LRU(head, p);
			} else  {
				OPT(head, p);
			}
		}
		print(head, type);
		interrupt++;
	} else {
		printf("正常执行\n");
	}
	p->unUsedTime = 0;
	// 记录其他页未使用次数
	for(p = head->next; p != NULL; p = p->next) {
		if (p->page != task/10) {
			p->unUsedTime ++;
		}
	}
	nowTime++;
}

void test(LinkedList &head, int type) {
	if (type == 0) {
		printf("\n【先进先出（FIFO）算法】\n");
	} else if (type == 1) {
		printf("\n【最近最久未使用（LRU）】\n");
	} else if (type == 2) {
		printf("\n【最佳置换（OPT】\n");
	}
	init(head);
	randomNum();
	for( ; taskIndex < 320; taskIndex++) {
		printf("\n【执行】第%d页的指令 %d：", task[taskIndex]/10, task[taskIndex]);
		runTask(head, task[taskIndex], type);
	}
	printf("\n\n【执行完毕】 全部指令执行完毕\n");
	distory(head);
	if (type == 0) {
		printf("\n【先进先出（FIFO）】：\n");
	} else if (type == 1) {
		printf("\n【最近最久未使用（LRU）】：\n");
	} else if (type == 2) {
		printf("\n【最佳置换（OPT）】：\n ");
	}
	printf("   缺页中断次数：%d, \n    缺页中断概率 %2.2f%% \n\n", interrupt, interrupt*100.0/320);
}