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
	ListDelete(&lst, 4);
	ListDump(&lst);
	ListInsertBack(&lst, 777);
        ListDump(&lst);
	ListLinearise(&lst);
	ListDump(&lst);
	DotDump(&lst);

	ListDtor(&lst);
	/*
	ListInsertBack(&lst, 1488);
	int el1487 = ListInsertBack(&lst, 1487);
	ListInsert(&lst, el1487, 790);
	ListDump(&lst);
	
	ListDelete(&lst, 2);
	ListDump(&lst);
	ListInsertFront(&lst, 69);
	ListDump(&lst);
	ListDtor(&lst);
	*/
}
