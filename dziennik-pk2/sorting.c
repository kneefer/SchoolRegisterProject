/*
   ╔════════════════════════════════╗
   ║		File sorting.c			║
   ╚════════════════════════════════╝
*/

#include "sorting.h"

int Comp_by_names(const void* a, const void* b){
	return _stricmp(((Record*)a)->name, ((Record*)b)->name) > 0;
}

int Comp_by_ID(const void* a, const void* b){
	return (((Record*)a)->ID > ((Record*)b)->ID);
}

void Guard_delete(Record** list){
	Record *head;
	head = NULL;

	if(*list != NULL){								// considering the eventuality
		if((*list)->next == NULL){					// when the list is empty
			free(*list);
			*list = NULL;
		} else {
			head = *list;							// we mustn't change the pointer
			while (head->next->next != NULL)		// which could cause malfunction
				head = head->next;
			free(head->next);
			head->next = NULL;
		}
	}
}

void Sort_list(Record** list, int(*compare)(const void* p1, const void* p2)){
	Record *guard, *p, *temp, *mem;

	if(*list != NULL){
		guard = (Record*)malloc(sizeof(Record));		
		guard->next = (Record*)malloc(sizeof(Record));
		guard->next->next = NULL;
		guard->next->name[0] = GUARD_CHR_VAL;			// values causing the guard
		guard->next->ID = GUARD_INT_VAL;				// is always the highest

		while(*list != NULL){
			p = guard;
			while(compare(*list, p->next))
				p = p->next;
			temp = (Record*)malloc(sizeof(Record));		// inserting the element
			*temp = **list;								// to a new list after
			temp->next = p->next;						// successfully having found
			p->next = temp;								// appropriate position

			mem = (*list)->next;
			free(*list);
			*list = mem;
		}

		Guard_delete(&guard);							// deleting closing guard
		mem = guard;
		*list = guard->next;							// omitting starting guard
		free(guard);
	} else
		Catch_error(ERR_EMPTYLIST);
}

void Reverse_list(Record** rec){
	Record *p, *mem_next_iteration;

	if(*rec){	// if rec != NULL
		mem_next_iteration = *rec;
		while(mem_next_iteration->next){
			p = mem_next_iteration->next;
			mem_next_iteration->next = mem_next_iteration->next->next;
			p->next = *rec;
            *rec = p;
		}
	} else
		Catch_error(ERR_EMPTYLIST);
}