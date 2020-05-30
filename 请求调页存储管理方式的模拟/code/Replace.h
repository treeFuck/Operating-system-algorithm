#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#ifndef REPLACE_H_INCLUDED
#define REPLACE_H_INCLUDED 


//内存块结构体
typedef struct LNode {

	int page; // 页码
	
	int inTime; // 进入时间（FIFO）

	int useTime; // 预计使用次数（OPT）
	
	int unUsedTime; // 未使用次数（FIFO、LRU）

	LNode* next;

}LNode, *LinkedList;

#endif