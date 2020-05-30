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
void init(TaskList &head) {
	head = (TaskList)malloc(sizeof(Task));
	head->next = NULL;
}

// 销毁作业
void destroy(TaskList head) {
	TaskList help;
	while (head != NULL) {
		help = head;
		head = head->next;
		free(help);
	}
}
// 新增作业
TaskList newTask(char name[10], int arriveTime, int serverTime, int finishTime) {
	TaskList p = (TaskList)malloc(sizeof(Task));
	strcpy(p->name, name);
	p->arriveTime = arriveTime;
	p->finishTime = finishTime;
	p->serverTime = serverTime;
	p->remainTime = serverTime;
	p->relaxation = finishTime - nowTime - serverTime;
	p->status = 0;
	p->next = NULL;
	return p;
}
// 插入到作业链表
void add(TaskList head, TaskList p) {
	while(head->next != NULL) {
		head = head->next;
	}
	head->next = p;
}

// 打印作业
void printAll(TaskList head) {
	TaskList p = head->next;

	printf("\n【第 %d 秒】:", nowTime);
	if (p == NULL) {
		printf("\n 【空】打印作业为空");
	}
	printf("\n 作业名 到达时间 服务时间 截止时间 剩余执行时间 松弛度   状态");
	while(p != NULL) {
		printf("\n %3s   %4d    %4d      %4d      %4d       ", p->name, p->arriveTime, p->serverTime, p->finishTime, p->remainTime);
		if (p->status == 0) {
			printf("%4s    %4s", "无", "未到达");
		} else if (p->status == 1) {
			printf("%4d    %4s", p->relaxation, "就绪中");
		} else if (p->status == 2) {
			printf("%4d    %4s", p->relaxation, "执行中");
		} else if (p->status == 3) {
			printf("%4s    %4s", "无", "已完成");
		} else if (p->status == 4) {
			printf("%4s    %4s", "无", "超时完成");
		}
		p = p->next;
	}	
	printf("\n");
}

// 判断是否继续执行
int ifContinueRun(TaskList head) {
	TaskList p = head->next;
	while(p != NULL) {
		// 还有未完成的
		if(p->status < 3) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}

// 计算松弛度
void setRelax(TaskList head) {
	TaskList p = head->next;
	while(p != NULL) {
		p->relaxation = p->finishTime - nowTime - p->remainTime;
		// 松弛度 = 截止时间 - 当前 - 剩余执行时间
		p = p->next;
	}
}
// 选出就绪作业里面松弛度最低的作业
TaskList getRunTask(TaskList head) {
	int min;
	TaskList result, p;

	p = head->next;
	result = NULL;

	min = -1001; // 初始化最小值的标志

	while(p != NULL) {
		if(p->status != 1) {
			p = p->next;
			continue;
		} 
		if (min == -1001) {
			// 初始化最小值
			min = p->relaxation;
			result = p;
			p = p->next;
			continue;
		}
		if(p->relaxation < min) {
			min = p->relaxation;
			result = p;
		} else if (p->relaxation == min) {
			// 松弛度相同，则选择“先来先去服务”算法
			if(p->arriveTime < result->arriveTime) {
				result = p;
			}
		}
		p = p->next;
	}
	return result;
}
// 执行一个作业
void runTask(TaskList head, TaskList task) {
	task->status = 2;
	task->remainTime --;
	if(task->remainTime == 0) {
		// 作业完成
		if (nowTime+1 <= task->finishTime) {
			task->status = 3; 
		} else {
			task->status = 4;
		}
	} 
}
// 作业到达
void arriveTask(TaskList head) {
	TaskList p = head->next;
	while(p != NULL) {
		if(p->arriveTime == nowTime) {
			p->status = 1;
		}
		p = p->next;
	}
}
// 执行
void run(TaskList head) {
	
	arriveTask(head);
	printAll(head); 
	while(ifContinueRun(head) == 1 ) { // 还有作业没执行完
		TaskList task = getRunTask(head); // 获取要执行的作业
		if(task != NULL) {
			runTask(head, task); // 执行
		} 
		nowTime++;
		setRelax(head); // 重设全部作业松弛度
		arriveTask(head); // 新作业就绪到达
		printAll(head);  // 打印
		if(task != NULL && task->status == 2) {
			// 刚执行的作业，如果没完成，则重置为就绪
			task->status = 1;
		}
	}
}

void test(TaskList head) {

	// 初始化作业
	init(head);
	// 添加作业
	add(head, newTask("作业1", 0, 4, 13)); // 名字 到达时间 服务时间 截止时间
	add(head, newTask("作业2", 1, 4, 9));
	add(head, newTask("作业3", 1, 3, 12));
	add(head, newTask("作业4", 2, 2, 6));
	add(head, newTask("作业5", 3, 1, 12));
	// 开始跑作业
	run(head);
	// 销毁数据，释放内存
	destroy(head);
	printf("\n\n【程序执行完毕，感谢使用】");

}