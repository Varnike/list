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
#include "error.h"

/*
 * 	TODO
 * 	1)Check+
 * 		nulls
 * 		double list
 * 		free list
 * 	2)Dump+
 * 		size
 * 		size in func
 * 		new errs
 * 	3)WhileLoop+
 * 	4)Resize
 * 	5)More deds functions / naming?
 * 	6)Linearising+
 * 	7)Ctor  check+
 */

#define $ printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);

#define LIST_CHECK 									\
	{										\
		assert(list);								\
		assert(list->buff);							\
		ERRNUM = 0;								\
		_ListCheck(list);							\
		if (ERRNUM) {								\
			perror(errmsg(ERRNUM));						\
			assert(!"HUGE ERR!");						\
		}									\
	}

typedef int listv_t;

const int BUFF_SIZE = 5;

enum get_data_mode {
	PHYS_ADDR = 0,
	LINEAR_LOGIC_ADDR = 1 // only for linearised list!
};

struct _NODE {
	listv_t val;
	int next;
	int prev;
};

struct LIST {
	struct _NODE *buff;
	int MAX_SIZE;
	int size;
	int HEAD;
	int TAIL;
	int FREE;
	int sorted;
};

int ListCtor(LIST *list, int pos, listv_t val);
int ListInsertBack(LIST *list, listv_t val);
int ListInsertFront(LIST *list, listv_t val);
int ListInsert(LIST *list, int pos, listv_t val);
int ListDelete(LIST *list, int logical_pos);
int ______ListFindNode(LIST *lst, int pos);
int ListDtor(LIST *list);
int ListLinearise(LIST *list);
_NODE *ListGetValue(LIST *list, int pos, int mode = 0);

void ListDump(LIST *list);
void DotDump(LIST *list);
int _ListCheck(LIST *list);

#endif // _LIST_H
