#include "list.h"

int main()
{
	LIST lst = {};
	ListCtor(&lst, 1, 10);

	ListDump(&lst);
	ListInsertBack(&lst, 11);
	ListDump(&lst);
	ListInsertBack(&lst, 22);
	ListDump(&lst);
	ListInsertBack(&lst, 33);
	ListDump(&lst);
	ListInsertBack(&lst, 1488);
	ListDump(&lst);
	ListDelete(&lst, 1);
	ListDump(&lst);
	ListInsertBack(&lst, 777);
        ListDump(&lst);
	//ListInsertFront(&lst, 900);
	ListLinearise(&lst);
	//_NODE *op = ListGetValue(&lst, 1, 1);
	//printf("=====%d=====\n", op->val);
	//ListInsert(&lst, 3, 777777);
	//lst.buff[3].prev = 0;
	//ListDelete(&lst, 3);
	ListInsertFront(&lst, 9);
	ListDump(&lst);
	DotDump(&lst);
//	lst.buff[10].prev = 1;
	//ListDtor(&lst);
	
	
	
	ListDtor(&lst);
}
