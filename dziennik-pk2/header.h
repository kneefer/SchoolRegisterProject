/*
   ╔════════════════════════════════╗
   ║		 File header.h			║
   ╚════════════════════════════════╝
*/

#ifndef _header_H_
#define _header_H_

// ##### Include's import #####

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <crtdbg.h> // memory leaks (DEBUG mode)

// ##### Constants Declarations #####

#define MANAGE_EDIT_PERSONAL	1
#define MANAGE_DEL_STUDENT		2
#define MANAGE_ADD_SUBJECTS		3
#define MANAGE_DEL_SUBJECTS		4
#define MANAGE_ADD_GRADES		5
#define MANAGE_DEL_GRADES		6
#define MANAGE_EDIT_COMMENT		7

#define MAX_GROUPS_AMOUNT		6
#define MAX_SURNAME_LENGTH		11
#define MAX_NAME_LENGTH			16
#define MAX_SUBJ_DESC_LENGTH	15
#define MAX_ID_VALUE			100
#define MAX_COMMENT_LENGTH		60		// <-- it will be also treated as the longest string among all structures
#define MAX_GRADE_VALUE			6		//     (don't let it to be less than NAME, SURNAME, SUBJ_DESC length properties
										//     in this part of define's)
#define MIN_NAMEs_LENGTH		1
#define MIN_GROUPS_AMOUNT		1
#define MIN_GRADE_VALUE			0
#define MIN_ID_VALUE			1
#define MIN_SUBJECTS_AMOUNT     1
#define MIN_COMMENT_LENGTH		0

#define MAX_CHRS_CMP_FIND		10
#define _CRTDBG_MAP_ALLOC		// checking the memory leaks (DEBUG mode)

typedef enum { FALSE, TRUE } BOOLEAN;

// ##### Structures Definitions #####

typedef struct Sublist
{
	int ID;
	struct Sublist* next;
	char description[MAX_SUBJ_DESC_LENGTH+1];
} Sublist;

typedef struct Grade
{
	struct Grade* next;
	float value;
} Grade;

typedef struct Subject
{
	int ID;
	struct Subject* next;
	char comment[MAX_COMMENT_LENGTH];
	struct Grade* grade;
} Subject;

typedef struct Record
{
	int ID;
	struct Record* next;
	char name[MAX_NAME_LENGTH];
	char surname[MAX_SURNAME_LENGTH];
	int group;
	struct Subject* subjects;
} Record;

// ##### Functions Definitions #####

Record** Find_students_by_txt(Record*, char*);
Record* Find_student_by_number(int, Record*);
Record* Find_student_by_number_and_group(int, Record*, int);
Record* Find_student_by_number_and_subject(int, Record*, int);
char* Find_subject_name_by_ID(int, Sublist*);
int Generate_new_ID(void*);
Sublist* Check_selected_subject_to_add_to_student(int, Sublist*, Record*);
Subject* Check_if_subject_attached_to_student(Record*, Sublist*);
Sublist* Select_subject_from_sublist(int, Sublist*);
Subject* Select_subject_of_student(int, Record*);
Grade* Select_grade_of_subject(int, Subject*);
void Add_student_to_list_of_students(Record**, Record*);
void Add_grades(Subject*);
void Add_subject_engine(Sublist**);
void Add_subjects_to_student_engine(Record*, Sublist*);
void Edit_comment(Record*, Sublist*);
void Edit_personal(Record*);
void Edit_subjects_names(Sublist*);

#endif
