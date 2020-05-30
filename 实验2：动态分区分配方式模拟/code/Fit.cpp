#include "Fit.h"


int AllSize = 640;

//初始化
void init(LinkedList& head) {
	head = (LinkedList)malloc(sizeof(LNode));
	if (NULL == head) {
		printf("\n 【初始化】分配内存失败！");
		exit(0);
	}
	head->id = -1;
	head->status = -1;
	head->next = (LinkedList)malloc(sizeof(LNode));
	head->next->id = 0;
	head->next->begin = 1;
	head->next->end = AllSize;
	head->next->status = 0;
	head->next->next = NULL;

}

//销毁所有节点数据
void destory(LinkedList &head) {
	LinkedList help;
	while (head != NULL) {
		help = head;
		head = head->next;
		free(help);
	}
	printf("\n 【销毁】所有空间已释放！");
}
// 打印一个段内存状况
void printOne(LinkedList p) {
	if(p->status==0) {
		printf("\n  |%6s|", "无");
	} else {
		printf("\n  |%6d|", p->id);
	}
	printf("   |%8d|    |%8d|    |%7dKB|", p->begin, p->end, p->end - p->begin + 1);
	printf("     |%6s|", p->status==0 ? "空闲":"已占用");
}

// 打印全部作业信息
void printAll (LinkedList head) {
	LinkedList p;
	printf("\n ————————————————————————————————");
	printf("\n  |分区号|   |起始地址|    |末尾地址|    |内存空间 |     |状态  |");
	for (p = head->next; p != NULL; p = p->next) {
		printOne(p);
	}
	printf("\n ————————————————————————————————\n");
}

// 获取新作业节点区号 
int getId(LinkedList head) {
	LinkedList p;
	int result = 0;
	for (p = head->next; p != NULL; p = p->next) {
		if (p->id > result) {
			result = p->id;
		}
	}
	return result + 1;
}


// 【首次适应算法】获取适合插入新节点的上一个节点
LinkedList getPro1(LinkedList head, int size) {
	LinkedList p;
	for (p = head; p->next != NULL; p = p->next) {
		if (p->next->status == 0 && (p->next->end - p->next->begin + 1) >= size) {
			break;
		}
	}
	if(size == 140) {
		// printOne(p->next);
	}
	return p;
}
// 【最佳适应算法】获取适合插入新节点的上一个节点
LinkedList getPro2(LinkedList head, int size) {
	LinkedList p, resultPro;
	int minSize = AllSize;
	resultPro = NULL;
	for ( p = head; p->next != NULL; p = p->next) {
		if (p->next->status == 0) {
			int p_size = p->next->end - p->next->begin + 1;
			// printf("\n  p_size = %d, size= %d,  minSize=  %d \n", p_size, size, minSize);
			if (p_size >= size && p_size <= minSize) {
				
				minSize = p_size;
				resultPro = p;
			}
		}
	}
	return resultPro;
}
// 插入节点（作业申请空间）
void addNode(LinkedList head, int size, int type) {
	LinkedList p;
	int id = getId(head);
	printf("\n【作业%d】：", id);
	if(type == 0) {
		// 首次适应算法
		p = getPro1(head, size);
	} else {
		// 最佳适应算法
		p = getPro2(head, size);
	}
	
	if(p==NULL || p->next == NULL) {
		// 分配作业内存空间失败
		printf("申请内存空间失败，内存不足");
	} else {
		
		LinkedList newNode = (LinkedList)malloc(sizeof(LNode));
		newNode->status = 1;
		newNode->id = id;
		newNode->begin = p->next->begin;
		newNode->end = newNode->begin + size - 1; 
		p->next->begin = newNode->end + 1;
		newNode->next = p->next;
		p->next = newNode;
		printf("成功申请 %4d KB内存空间", newNode->end - newNode->begin + 1);
	}
	printAll(head);
}

// 删除节点（作业释放空间）
void delNode(LinkedList head, int id) {
	LinkedList p;
	printf("\n【作业%d】：", id);
	for (p = head; p->next != NULL; p = p->next) {
		if (p->next->id == id) {
			LinkedList pro = p;
			LinkedList del = p->next;
			LinkedList next = del->next;
			if(del->status == 1) {
				printf("成功释放 %4d KB内存空间",  del->end - del->begin + 1);
				// 合并
				if( pro->id == -1) { // pro为头节点
					if(next->status == 0){ 
						// next空闲
						del->status = 0;
						del->end = next->end;
						del->next = next->next;
						free(next);
					} else { 
						//next被占用 
						del->status = 0;
					} 
				} else { // pro不为头节点
					if(pro->status == 0 && next->status == 1){ 
						// pro空闲，next被占用 
						pro->next = next;
						pro->end = del->end;
						free(del);
					} else if(pro->status == 1 && next->status == 0){ 
						// pro被占用，next空闲
						pro->next = next;
						next->begin = del->begin;
						free(del);
					} else if(pro->status == 0 && next->status == 0) { 
						// pro，next都空闲
						pro->next = next->next;
						pro->end = next->end;
						free(del);
						free(next);
					} else {
						// pro，next都被占用
						del->status = 0;
					}
					
				}
			} else {
				// 没有该作业
				printf("释放内存空间失败，没有该作业");
			}
			break;
		}
	}
	printAll(head);
}



void test(LinkedList head, int type) {
	printf("作业1 申请130 KB\n");
	//作业1 申请130KB
	addNode(head, 130, type);
	//作业2 申请60KB
	addNode(head, 60, type);
	//作业3 申请100KB
	addNode(head, 100, type);
	//作业2 释放60KB
	delNode(head, 2);
	//作业4 申请200KB
	addNode(head, 200, type);
	//作业3 释放100KB
	delNode(head, 3);
	//作业1 释放130KB
	delNode(head, 1);
	//作业5 申请140KB
	addNode(head, 140, type);
	//作业6 申请60KB
	addNode(head, 60, type);
	//作业7 申请50KB
	addNode(head, 50, type);
	//作业8 申请60KB
	addNode(head, 60, type);
}
