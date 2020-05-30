#include "SJF.h"
#include "SJF.cpp"

int main()
{
	LinkedList link;
	
	// createPro(link);
	// run(link);
	// fflush(stdin);
	// getchar();
	
	return;


	link = (LinkedList)malloc(sizeof(LNode));
	link->next = NULL;
	pro_num = 5;
	
	insertLNode(link, "A", 0, 4);
	insertLNode(link, "B", 1, 3);
	insertLNode(link, "C", 2, 5);
	insertLNode(link, "D", 3, 2);
	insertLNode(link, "E", 4, 4);

	run(link);
	getchar();


	

}


