#include "list.h"

static int get_free_place(LIST *lst);
static void init_list(LIST *list);
static void init_free_space(LIST *list);
static void add_free_node(LIST *list, int pos);
static int get_free_node(LIST *list);
static void set_list_node(LIST *list, int pos, listv_t val, int next, int prev);

int ListCtor(LIST *list, int pos, listv_t val)
{
	assert(list);
	
	list->buff = (_NODE*)calloc(sizeof(_NODE), BUFF_SIZE);
	list->MAX_SIZE = BUFF_SIZE; //TODO for future

	init_list(list);
	set_list_node(list, pos, val, 0, 0);
	init_free_space(list);
	
	list->size = 1;
	list->HEAD = pos;
	list->TAIL = pos;

	LIST_CHECK;
	return 0;
}

int ListDtor(LIST *list)
{
	LIST_CHECK;

	free(list->buff);

	return 0;
}

listv_t ListGetValue(LIST *list, int pos)
{
	LIST_CHECK;
	
	return list->buff[pos].val;

	LIST_CHECK;
}

int ListInsertBack(LIST *list, listv_t val)
{
	LIST_CHECK;

	int lnext = get_free_node(list);
	if (lnext < 0)
		return -1;
	
	list->size++;
	set_list_node(list, lnext, val, 0, list->TAIL);
	
	list->buff[list->TAIL].next = lnext;
	list->TAIL = lnext;
	
	DotDump(list);
	LIST_CHECK;
	return lnext;
}

int ListInsertFront(LIST *list, listv_t val)
{
	LIST_CHECK;

	int lnext = get_free_node(list);
	if (lnext < 0) 
		return -1;
	
	list->size++;
	set_list_node(list, lnext, val, list->HEAD, 0);
	
	list->buff[list->HEAD].prev = lnext;
	list->HEAD = lnext; 

	LIST_CHECK;                     
	
	return lnext;
}                                          

int ListInsert(LIST *list, int pos, listv_t val)
{
	LIST_CHECK;
	
	int lnext = get_free_node(list);
	if (lnext < 0)
		return -1;
	
	list->size++;
	/* setup new node */
	printf("\n\nUUUU       %d	\n\n", pos);
	set_list_node(list, lnext, val, list->buff[pos].next, pos);
	
	/* update old ones */
	list->buff[list->buff[pos].next].prev = lnext;
	list->buff[pos].next  = lnext;
	
	LIST_CHECK;

	return lnext;
}

int ListDelete(LIST *list, int pos)
{
	LIST_CHECK;

	if (pos <= 0) {
		ERRNUM = LIST_INV_POS_ERR;
		return -1;
	}	
	
	list->size--;

	if (pos == list->TAIL || pos == list->HEAD) {
		if (pos == list->TAIL) {
			list->TAIL = list->buff[pos].prev;
		
			list->buff[list->TAIL].next = 0;
		} else {
			list->HEAD = list->buff[pos].next;
                                  
			list->buff[list->HEAD].prev = 0;  
		}

		set_list_node(list, pos, 0, -1, -1);
		add_free_node(list, pos);
		return 0;
	}

	int lprev = list->buff[pos].prev;
	int lnext = list->buff[pos].next;

	list->buff[lprev].next = lnext;
	list->buff[lnext].prev = lprev;	

	set_list_node(list, pos, 0, -1, -1);
	add_free_node(list, pos);

	LIST_CHECK;
	return 0;
}

int ListLinearise(LIST *list)
{
	LIST_CHECK;
	
	_NODE *newbuff = list->buff;
	list->buff = (_NODE*)calloc(sizeof(_NODE), BUFF_SIZE);	
	
	if (list->buff == NULL)
		return ERRNUM = CALLOC_ERR;

	init_list(list);

	int node_cnt = 1;
	int it = list->HEAD;
	
	do {
		newbuff[it].prev = node_cnt - 1;
		list->buff[node_cnt++] = newbuff[it];
		it = newbuff[it].next;
	} while(it != 0 && node_cnt != list->MAX_SIZE);
	
	list->HEAD = 1;
	list->TAIL = node_cnt - 1;
	list->buff[1].prev = 0;

	free(newbuff);
	
	init_free_space(list);
	
	LIST_CHECK;
	return 0;
}

int ______ListFindNode(LIST *list, int logical_pos)
{
	LIST_CHECK;

	for (int ph_pos = 0; ph_pos != BUFF_SIZE; ph_pos++) {
		if (list->buff[ph_pos].next == logical_pos)
			return ph_pos;
	}
	
	ERRNUM = LIST_NODE_NOT_FOUND;

	return -1;
}

void ListDump(LIST *list)
{
	assert(list);
	assert(list->buff);
	
	printf("HEAD = %d, TAIL = %d, FREE = %d, size = %d\nPHYSICAL LIST:\n",
		       	list->HEAD, list->TAIL, list->FREE, list->size);

	for (int i = 0; i != BUFF_SIZE; i++) 
		printf("<  %d  > [ %d ] == [ %d ] == [ %d ]\n", i, list->buff[i].val, 
				list->buff[i].prev, list->buff[i].next);

	printf("LOGICAL LIST:\n");
	
	int list_no = 1;
	int it = list->HEAD;
	int k = 0;
	do {
		printf("<  %d  > [ %d ] == [ %d ] == [ %d ]\n", list_no++, list->buff[it].val, 
				list->buff[it].prev, list->buff[it].next);
		it = list->buff[it].next;
	} while(it != 0 && k++ != 10);

	printf("\n\n\n");
}

void DotDump(LIST *list)
{
	assert(list);
	assert(list->buff);

	int list_no = 0;
	int it = list->TAIL;
	
	FILE *fout = NULL; 
	fout = fopen("dump/dump.dot", "w");
	assert(fout);

	fprintf(fout, "digraph dump_graph {\n\trankdir=LR; \n");
		
	for (int i = 1; i != list->MAX_SIZE; i++) {
//		fprintf(fout, "\tstruct%d [label=\"<f0> %d| <f1>next: %d|<f2>prev: %d|<f3>data: %d\"];\n",
//				i, i, list->buff[i].next, list->buff[i].prev, list->buff[i].val);
		
		fprintf(fout, "\tstruct%d [shape=box3d\n\t\
				label=\n\t<\n\t<table border=\"0\" cellspacing=\"0\">\n\
				\t\t<tr><td PORT=\"port0\" border=\"1\"  bgcolor=\"lightskyblue\">%d</td></tr>\n\
				\t\t<tr><td PORT=\"port1\" border=\"1\" bgcolor=\"grey69\">next: %d</td></tr>\n\
				\t\t<tr><td PORT=\"port2\" border=\"1\" bgcolor=\"grey69\">prev: %d</td></tr>\n\
				\t\t<tr><td PORT=\"port3\" border=\"1\" bgcolor=\"grey69\">data: %d</td></tr>\n\
				\t</table>>];\n",
	                i, i, list->buff[i].next, list->buff[i].prev, list->buff[i].val);

		if (i == list->MAX_SIZE - 1) {
			break;
		}
		fprintf(fout, "\tstruct%d:port0 -> struct%d:port0[style=invis];\n", i, i+1);
		it = list->buff[it].next;
	}

	int next_label = list->HEAD;

	while (next_label != 0) {
		if (list->buff[next_label].next != 0)
        		fprintf(fout, "\tstruct%d:port1 -> struct%d:port1[style=bold, arrowhead=vee,\
					color=forestgreen];\n", next_label, list->buff[next_label].next);
		if (!list->buff[next_label].prev == 0)
			fprintf(fout, "\tstruct%d:port2 -> struct%d:port2[style=dashed, arrowhead=onormal,\
					color=grey];\n", next_label, list->buff[next_label].prev);
        	next_label = list->buff[next_label].next;                                                     
	}
	
	next_label = list->FREE;

	while (next_label != 0) {
		if (list->buff[next_label].next != 0)
			fprintf(fout, "\tstruct%d:port1 -> struct%d:port1;\n", 
					next_label, list->buff[next_label].next);
		next_label = list->buff[next_label].next;
	}

	fprintf(fout, "\tfree[fillcolor=\"pink\",style=filled, shape=doubleoctagon, label=\"FREE = %d\"];\n", 
			list->FREE);
	fprintf(fout, "\tfree -> struct%d:port1;\n", list->FREE);

	fprintf(fout, "\thead[fillcolor=\"pink\", style=filled, shape=doubleoctagon, label=\"HEAD = %d\"];\n", 
			list->HEAD);
	fprintf(fout, "\thead -> struct%d:port1;\n", list->HEAD);

	fprintf(fout, "\ttail[fillcolor=\"pink\", style=filled, shape=doubleoctagon, label=\"TAIL = %d\"];\n", 
			list->TAIL);
	fprintf(fout, "\ttail -> struct%d:port1;\n", list->TAIL);


	fprintf(fout, "}\n");

	if (fout != stdout)
		fclose(fout);
}

static void init_list(LIST *list)
{
	assert(list);

	for (int i = 1; i != BUFF_SIZE; i++) {
		list->buff[i].next = -1;
		list->buff[i].prev = -1;
	}
	
	set_list_node(list, 0, 0, 0, 0);
	printf("!!!%d, %d, %d!!!\n", list->buff[0].val, list->buff[0].next, list->buff[0].prev);
}

static int get_free_place(LIST *list)
{
	for (int i = 1; i != BUFF_SIZE; i++) {
		if (list->buff[i].prev == -1)
			return i;
	}

	ERRNUM = LIST_NO_FREE_SPACE;
	return -1; // no free place
}

int _ListCheck(LIST *list)
{
	assert(list);
	assert(list->buff);

	if (list->buff[list->HEAD].next < 0  || (list->buff[list->HEAD].prev != 0 && list->size != 1))
		return ERRNUM = LIST_BAD_HEAD_ERR;

	if (list->buff[list->TAIL].next != 0)
		return ERRNUM = LIST_BAD_TAIL_ERR;

	int it = list->HEAD;//= list->buff[list->HEAD].next;
	
	for (int list_it = 1; list_it != list->MAX_SIZE; ) {
		it = list->buff[it].next;
		list_it++;
	
		/* check if it is valid */
		if (it < 0) {
			return ERRNUM = LIST_NODE_BAD_NEXT;
		}
		if (list->buff[it].next == 0) {
			if (list_it == list->size && it == list->TAIL) {
				break;
			} else {
				printf("ERROR on node %d: count is %d, size is %d\n",
						it, list_it, list->size);
	
				return ERRNUM = LIST_BROKEN;
			}
		}
	
		if (list_it == list->size)
			return ERRNUM = LIST_BROKEN; 


		if (list->buff[it].prev < 0) 
			return ERRNUM = LIST_NODE_BAD_PREV;

		/* Check connection between neighbor nodes */
		if (list->buff[list->buff[it].prev].next != it) {
			printf("ERROR on node %d: prev is %d, next[prev] is %d\n", 
					it, list->buff[it].prev, list->buff[list->buff[it].prev].next);
			return ERRNUM = LIST_CONNECTION_ERR;
		}
	}
	
	/* Check free list */
	if (list->FREE < 0)
		return ERRNUM = LIST_BAD_FREE_ERR;

	it = list->FREE; //list->buff[list->FREE].next;
	
	for (int list_cnt = 1; list_cnt != list->MAX_SIZE; ) {
		it = list->buff[it].next;
		list_cnt++;

		if (it < 0)
			return LIST_FREE_LIST_BROKEN;

		if (list_cnt > list->MAX_SIZE - list->size)
			return LIST_FREE_LIST_BROKEN;
		
		if (list->buff[it].prev != -1)
			return ERRNUM = LIST_BAD_FREE_NODE;

		if (list->buff[it].next == 0)
			break;
	}
	return ERRNUM = NO_ERR;
}

static void set_list_node(LIST *list, int pos, listv_t val, int next, int prev)
{
	assert(list);
	assert(list->buff);

	list->buff[pos].val  = val; 
	list->buff[pos].next = next;
	list->buff[pos].prev = prev;
}

static void add_free_node(LIST *list, int pos)
{
	assert(list);
	assert(list->buff);

	set_list_node(list, pos, 0, list->FREE, -1);

	list->FREE = pos;
}

static int get_free_node(LIST *list)
{
	//TODO check for -1
	assert(list);
	assert(list->buff);

	if (list->FREE == 0) {
		ERRNUM = LIST_NO_FREE_SPACE;
		return -1; //TODO add space
	}

	int free_node = list->FREE;
	list->FREE = list->buff[list->FREE].next;
	
	printf("free node found : %d.\n", free_node);
	return free_node;
}

static void init_free_space(LIST *list)
{
	assert(list);
	assert(list->buff);

	int it = 0;

	for (it = 1; it != list->MAX_SIZE; it++) {
		if (list->buff[it].prev == -1) {
			list->FREE = it;
			break;
		}
	}
	
	printf("free is : %d\n", list->FREE);
	int prev = list->FREE;
	int next = 0;
	
	for ( ; it != list->MAX_SIZE; it++) {
		if (list->buff[it].prev == -1) {
			printf("it is : %d\n", it);
			list->buff[prev].next = it;
			prev = it;
		}
	}

	list->buff[prev].next = 0;
}
