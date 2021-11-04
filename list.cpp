#include "list.h"

static void init_list(LIST *list);
static int get_free_place(LIST *list);

int ListCtor(LIST *list, int pos, listv_t val)
{
	assert(list);
	
	list->list = (int *)calloc(sizeof(int), BUFF_SIZE);
	list->buff = (listv_t*)calloc(sizeof(listv_t), BUFF_SIZE);

	init_list(list);
	
	list->list[pos] = 0;
	list->buff[pos] = val;
	
	list->HEAD = pos;
	list->TAIL = pos;

	LIST_CHECK;
	return 0;
}

int ListDtor(LIST *list)
{
	LIST_CHECK;

	free(list->list);
	free(list->buff);

	return 0;
}

int ListInsertStart(LIST *list, listv_t val)
{
	//insert_side(list, val, __START);
	
	int next = get_free_place(list);
	if (next < 0)
		return -1;//TODO err

	list->buff[next] = val;
	list->list[next] = 0;

	list->list[list->HEAD] = next;
	list->HEAD = next;

	LIST_CHECK;
	return 0;
}

int ListInsertBack(LIST *list, listv_t val)
{                                          
	int next = get_free_place(list);
	if (next < 0)
		return -1;//TODO err

	list->buff[next] = val;
	list->list[next] = list->TAIL;
	list->TAIL = next; 

	LIST_CHECK;                     
	return next;
}                                          

int ListInsert(LIST *list, int pos, listv_t val)
{
	LIST_CHECK;

	int next = get_free_place(list);
	if (next < 0)
		return -1; //TODO err

	list->buff[next] = val;
	list->list[next] = list->list[pos];
	list->list[pos]  = next;

	LIST_CHECK;

	return next;
}

int ListDelete(LIST *list, int pos)
{
	LIST_CHECK;

	printf("pos = %d\n", pos);
	if (pos <= 0)
		return -1;// TODO err

	if (pos == list->TAIL) {
		list->TAIL = list->list[pos];
		list->list[pos] = -1;
		return 0;
	}

	if (pos == list->HEAD) {
		list->list[pos] = -1;
		return 0;
	}
	
	int prev = ListFindNode(list, pos);
	printf("pos = %d, prev = %d\n", pos, prev);

	list->list[prev] = list->list[pos];
	list->list[pos] = -1;

	LIST_CHECK;
	return 0;
}

int ListFindNode(LIST *list, int logical_pos)
{
	for (int ph_pos = 0; ph_pos != BUFF_SIZE; ph_pos++) {
		if (list->list[ph_pos] == logical_pos)
			return ph_pos;
	}

	return -1;
}
void ListDump(LIST *list)
{
	assert(list);
	assert(list->buff);
	assert(list->list);
	
	printf("HEAD = %d, TAIL = %d\n PHYSICAL LIST:\n", list->HEAD, list->TAIL);

	for (int i = 0; i != BUFF_SIZE; i++) 
		printf("<  %d  > [ %d ] == [ %d ]\n", i, list->buff[i], list->list[i]);

	printf("LOGICAL LIST:\n");
	
	// TODO ?
	int list_no = 0;
	int it = list->TAIL;
	do {
		printf("<  %d  > [ %d ] == [ %d ]\n", list_no++, list->buff[it], list->list[it]);
		it = list->list[it];
	} while(it != 0);

	printf("\n\n\n");
}

static void init_list(LIST *list)
{
	assert(list);

	for (int i = 1; i != BUFF_SIZE; i++)
		list->list[i] = -1;
	
	list->list[0] = 0;
	list->buff[0] = 0;
}

static int get_free_place(LIST *list)
{
	for (int i = 0; i != BUFF_SIZE; i++) {
		if (list->list[i] == -1)
			return i;
	}

	return -1; // no free place
}
