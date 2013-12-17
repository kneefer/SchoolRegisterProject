/*
   ╔════════════════════════════════╗
   ║		File interface.h		║
   ╚════════════════════════════════╝
*/

#ifndef _interface_H_
#define	_interface_H_

// ##### Include's import #####

#include <string.h>
#include "header.h"
#include "errors.h"
#include "del_free_ops.h"

// ##### Constants Declarations #####

#define ENUM_WIDTH				3
#define GROUP_WIDTH				4
#define FLOAT_PRECISION			1
#define GRADE_WIDTH				4
#define GRADES_BEF_SPACING		3
#define NUM_OF_GRADES_IN_LINE	8

// ##### Functions Definitions #####

void Get_float(float*, int, int);
void Get_str(char*, int, int);
void Get_int(int*, int, int);

BOOLEAN Prompt(char*);

void Write_students_by_all(Record**, Sublist*);
void Write_students_by_group(Record**, Sublist*);
void Write_students_by_subject(Record**, Sublist*);
int Write_subjects_of_student(Subject*, Sublist*);
int Write_available_subjects(Sublist*);
void Show_info_and_help(void);
void Delete_grades_of_subject(Subject*, Sublist*);
void Delete_subjects_and_connections(Record*, Sublist**);
void Add_subject_interface(Sublist**);
void Filter_students_by_txt(Record**, Sublist*);
BOOLEAN New_student(Record**, Sublist*);

#endif
