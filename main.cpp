#include "list.h"

int main()
{
	LIST lst = {};
	ListCtor(&lst, 1, 10);
	ListInsertBack(&lst, 1488);
	int el1487 = ListInsertBack(&lst, 1487);
	ListInsert(&lst, el1487, 790);
	ListDump(&lst);
	
	ListDelete(&lst, 2);
	ListDump(&lst);
	ListInsertStart(&lst, 69);
	ListDump(&lst);
	ListDtor(&lst);
}
