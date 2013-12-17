/*
   ╔════════════════════════════════╗
   ║	  File del_free_ops.c		║
   ╚════════════════════════════════╝
*/


#include "del_free_ops.h"

void Free_sublist(Sublist* sublist){
	Sublist* mem;

	while(sublist){
		mem = sublist->next;
		free(sublist);
		sublist = mem;
	}
}

void Free_grades(Subject* subject){
	Grade* mem;

	while(subject->grade){
		mem = subject->grade->next;
		free(subject->grade);
		subject->grade = mem;
	}
}

void Free_subject(Subject** subject){
	Free_grades(*subject);
	free(*subject);
	*subject = NULL;
}

void Free_subjects(Record* student){
	Subject* mem;

	while(student->subjects){
		mem = student->subjects->next;
		Free_subject(&(student->subjects));
		student->subjects = mem;
	}
}

void Free_student(Record** student){
	Free_subjects(*student);
	free(*student);
	*student = NULL;
}

void Free_memory(Record** list){
	Record* mem;

	while(*list){
		mem = (*list)->next;
		Free_student(list);
		*list = mem;
	}
}

void Delete_student(Record** to_del, Record** list){
	Record *mem, *iteration;

	if(*to_del == *list){
		mem = (*to_del)->next;
		Free_student(to_del);
		*list = mem;
	}
	else {
		iteration = *list;		// we can't change the content of *sublist
		while(*to_del != iteration->next)
			iteration = iteration->next;
		mem = iteration->next->next;
		Free_student(to_del);
		iteration->next = mem;
	}
}

void Delete_subject_from_sublist(Sublist** to_del, Sublist** sublist){
	Sublist* mem, *iteration;

	if(*to_del == *sublist){
		mem = (*to_del)->next;
		free(*to_del);
		*sublist = mem;
	} else {
		iteration = *sublist;
		while(*to_del != iteration->next)
			iteration = iteration->next;
		mem = iteration->next->next;
		free(*to_del);
		iteration->next = mem;
	}
}

void Delete_subject_of_student(Subject** to_del, Subject** list){
	Subject *mem, *iteration;

	if(*to_del == *list){
		mem = (*to_del)->next;
		Free_subject(to_del);
		*list = mem;
	} else {
		iteration = *list;		// we must memorize it
		while(*to_del != iteration->next)
			iteration = iteration->next;
		mem = iteration->next->next;
		Free_subject(to_del);
		iteration->next = mem;
	}
}

void Delete_grade(Grade** to_del, Grade** list){
	Grade *mem, *iteration;

	if(*to_del == *list){
		mem = (*to_del)->next;
		free(*to_del);
		*list = mem;
	}
	else {
		iteration = *list;		// we must memorize it
		while(*to_del != iteration->next)
			iteration = iteration->next;
		mem = iteration->next->next;
		free(*to_del);
		iteration->next = mem;
	}
}