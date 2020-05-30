#include "Fit.h"
#include "Fit.cpp"

void main()
{
	LinkedList head;
	
	printf("\n【首次适应算法】：\n");
	init(head);
	test(head, 0);
	destory(head);
	
	//printf("\n【最佳适应算法】：\n");
	//init(head);
	//test(head, 1);
	//destory(head);

	getchar();
}


