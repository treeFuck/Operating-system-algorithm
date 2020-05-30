#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#ifndef FIT_H_INCLUDED
#define FIT_H_INCLUDED

typedef struct LNode {
	 
	int id; // 分区号
	 
	int begin; // 起始地址
	
	int end; // 末尾地址 
	
	int status; // 状态 0代表空闲，1代表已占用 

	struct LNode* next;

}LNode, *LinkedList;

#endif
