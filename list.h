#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <cstring>
#include <stdio.h>
#include <cassert>
#include <cstdlib>
#include <typeinfo>
#include <stdint.h>
#include <inttypes.h>

#define LIST_CHECK 									\
	{										\
		assert(list);								\
		assert(list->buff);							\
		assert(list->list);							\
	}

typedef int listv_t;

const int BUFF_SIZE = 8; // char

/*	TODO
 * 1. errors
 * 2. list_chexk
 *
 */

struct LIST {
	int *list;
	int HEAD;
	int TAIL;
	listv_t *buff;
};

int ListCtor(LIST *list, int pos, listv_t val);
int ListInsertStart(LIST *list, listv_t val);
int ListInsertBack(LIST *list, listv_t val);
int ListInsert(LIST *list, int pos, listv_t val);
int ListDelete(LIST *list, int logical_pos);
int ListFindNode(LIST *lst, int pos);
int ListDtor(LIST *list);
void ListDump(LIST *list);
int ListCheck(LIST *list);
#endif // _LIST_H
