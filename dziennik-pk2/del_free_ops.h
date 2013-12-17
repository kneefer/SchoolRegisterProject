/*
   ╔════════════════════════════════╗
   ║	  File del_free_ops.h		║
   ╚════════════════════════════════╝
*/


#ifndef _del_free_ops_H_
#define _del_free_ops_H_

#include "header.h"

// Functions declarations

void Free_sublist(Sublist*);
//void Free_grades(Subject*);   - not used externaly
//void Free_subject(Subject**); - not used externaly
//void Free_subjects(Record*);  - not used externaly
void Free_student(Record**);
void Free_memory(Record**);

/*
	functions below delete element by known pointer from lists
	therefore they've to find this element in that lists
*/
void Delete_student(Record**, Record**);
void Delete_subject_from_sublist(Sublist**, Sublist**);
void Delete_subject_of_student(Subject**, Subject**);
void Delete_grade(Grade**, Grade**);

#endif
