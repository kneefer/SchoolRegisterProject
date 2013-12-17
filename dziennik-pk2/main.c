/*
   ╔════════════════════════════════╗
   ║		  File main.c			║
   ╚════════════════════════════════╝
*/

#include "header.h"
#include "sorting.h"
#include "save_load.h"
#include "errors.h"
#include "del_free_ops.h"
#include "interface.h"

int Generate_new_ID(void* pointer)
{
	int i;								// the function takes Sublist or Subject structure
	void* mem;							// ###VERY IMPORTANT!### is that order of fields of structures
	BOOLEAN duplication;					// has to be: 1# (int)ID 2# (pointer_to_foo_structure)next

	for(i=1;;i++){						// we want to have an ID property
		mem = pointer;					// without any repeats
		duplication = FALSE;
		while(mem){
			if(*((int*)mem) == i){		// checking if the current ID property is reserved
				duplication = TRUE;		// ... setting the flag 'duplication' if yes
				break;
			}
			mem = *(void**)((unsigned long)mem+sizeof(int));	// universal equivalent of
		}														// "foo_struct = foo_struct->next_item"
		if(!duplication)										// (assuming the "foo_struct" is adapted)
			break;
	}
	return i;
}

void Add_student_to_list_of_students(Record** list, Record* item){
	if (*list == NULL){
		item->next = NULL;
		*list = item;
	} else {
		item->next = *list;
		*list = item;
	}
}

BOOLEAN Cmp_txt(char* str_1, char* str_2, int max){
	int i;

	for(i=0; (str_1[i] && str_2[i]); i++)
		if(tolower(str_1[i]) != tolower(str_2[i])) return FALSE;
	return TRUE;
}

char* Find_subject_name_by_ID(int ID, Sublist* sublist){
	while(sublist){
		if(sublist->ID == ID)
			return sublist->description;
		sublist = sublist->next;
	}
	return NULL;	// if not found
}

Record** Find_students_by_txt(Record* rec, char* str){
	int num_of_positions = 0;
	Record **table_of_results = NULL, **temp = NULL;

	while(rec){
		if(Cmp_txt(rec->name, str, MAX_NAME_LENGTH) || Cmp_txt(rec->surname, str, MAX_NAME_LENGTH)){
			/* increasing size of table_of_results for the next found student */
			temp = (Record**)realloc(table_of_results, ((++num_of_positions)*sizeof(Record*)));

			if(temp){	// if temp != NULL (error during reallocation)
				table_of_results = temp;
				table_of_results[num_of_positions-1] = rec;		// adding to the array
			} else {
				free(table_of_results);
				Catch_error(ERR_REALLOC);
				return NULL;
			}
		}
		rec = rec->next;
	}

	/* adding null pointer at the end of the array to prevent errors */
	if(num_of_positions){
		temp = (Record**)realloc(table_of_results, ((++num_of_positions)*sizeof(Record*)));
		if(temp){	
			table_of_results = temp;
			table_of_results[num_of_positions-1] = NULL;
			return table_of_results;
		} else {
			free(table_of_results);
			Catch_error(ERR_REALLOC);
			return NULL;
		}
	}
	Catch_error(ERR_NOTFOUND);  // if not found anything
	return NULL;
}

Record* Find_student_by_number(int num, Record* list){
	int i = 1;		// students are numerated from 1

	while(list && (i++ < num))
		list = list->next;
	return list;	// returning the found student (or NULL if not found)
}

Record* Find_student_by_number_and_group(int num, Record* list, int group){
	while(list) {
		if(list->group == group)
			num--;
		if(!num)		// if num=0 - we have wanted record
			return list;
		list = list->next;
	}
	return NULL;
}

Record* Find_student_by_number_and_subject(int num, Record* list, int sub_id){
	Subject* subject;
	/* iterating on the Record* list */
	while(list){
		subject = list->subjects;
		/* iterating on the Subject* list (of current student) */
		while(subject){		
			if(subject->ID == sub_id){
				num--;
				break;
			}
			subject = subject->next;
		}
		if(!num)			// if we have record we needed
			return list;
		list = list->next;
	}
	return NULL;
}

Sublist* Select_subject_from_sublist(int amount_of_subjects, Sublist* subject){
	int num, i;

	Get_int(&num, 0, amount_of_subjects);	// takes the integer between 0 (for exit)
											// and amount of subjects
	if(!num)
		return NULL;						// if exit selected

	for(i=1;i<num;i++)						// iterating to the moment when we're at the correct
		subject = subject->next;			// position of Sublist (mem variable)
	
	return subject;
}

Subject* Select_subject_of_student(int amount_of_subjects, Record* rec){
	Subject* mem;
	int num, j;

	mem = rec->subjects;
	Get_int(&num, 0, amount_of_subjects);		// 0 for abort

	if(num){
		/* iterating to the moment when we have appropriate Subject */
		for(j=1;j<num;j++)
			mem = mem->next;
	} else
		mem = NULL;								// if 0 (exit) selected
	return mem;
}

Grade* Select_grade_of_subject(int amount_of_grades, Subject* sub){
	Grade* mem;
	int num, j;

	mem = sub->grade;
	Get_int(&num, 0, amount_of_grades);			// 0 for abort

	if(num){
		/* iterating to the moment when we have appropriate Grade */
		for(j=1;j<num;j++)
			mem = mem->next;
	} else
		mem = NULL;								// if 0 (exit) selected
	return mem;
}

Sublist* Check_selected_subject_to_add_to_student(int amount, Sublist* subject, Record* rec){
	Subject* mem = rec->subjects;			// we mustn't change the "rec->subjects" pointer

	if(!(subject = Select_subject_from_sublist(amount, subject)))
		return NULL;					// if exit

	/* checking if the new subject already exists with current student */
	while(mem){
		if(mem->ID == subject->ID){		// collide found!
			Catch_error(ERR_SUBEXISTS);
			return NULL;
		}
		mem = mem->next;
	}
	return subject;
}

Subject* Check_if_subject_attached_to_student(Record* rec, Sublist* subject){
	Subject* mem;

	mem = rec->subjects;
	while(mem){
		if(mem->ID == subject->ID)
			return mem;
		mem = mem->next;
	}
	return NULL;
}

void Add_grades(Subject* subject){
	float fl;

	Grade *new_grade, *mem;

	Get_float(&fl, MIN_GRADE_VALUE, MAX_GRADE_VALUE);
	new_grade = (Grade*)malloc(sizeof(Grade));
	new_grade->value = fl;
		
	mem = subject->grade;
	subject->grade = new_grade;
	new_grade->next = mem;
}

void Add_subject_engine(Sublist** sublist){
	Sublist *new_subject;

	new_subject = (Sublist*)malloc(sizeof(Sublist));

	Get_str(new_subject->description, MIN_NAMEs_LENGTH, MAX_SUBJ_DESC_LENGTH);
	new_subject->ID = Generate_new_ID(*sublist);	// looking for a free ID for a new subject
	
	new_subject->next = *sublist;					// adding to the list
	*sublist = new_subject;
}

void Add_subjects_to_student_engine(Record* rec, Sublist* sub_to_add){
	Subject *new_pos, *mem;

	new_pos = (Subject*)malloc(sizeof(Subject));	// preparing a new subject 
	new_pos->next = NULL;							// which is going to be added
	new_pos->ID = sub_to_add->ID;					// to the current student
	new_pos->grade = NULL;
	new_pos->comment[0] = '\0';

	mem = rec->subjects;							// restoring the previous value of "mem2" variable
	rec->subjects = new_pos;						// ... and adding at the beginning of the list
	new_pos->next = mem;
}

void Edit_comment(Record* rec, Sublist* sublist){
	int i;
	Subject* subject;

	if(i = Write_subjects_of_student(rec->subjects, sublist)){
		printf("\nKtory przedmiot ucznia chcesz wybrac? (0 aby anulowac): ");
		subject = Select_subject_of_student(i, rec);
		printf("\nPodaj nowy komentarz do przedmiotu (max %i znakow): ", MAX_COMMENT_LENGTH);
		Get_str(subject->comment, MIN_COMMENT_LENGTH, MAX_COMMENT_LENGTH);
	}
}

void Edit_personal(Record* rec){
	int choice;

	do{
		printf("\nCo chcesz zmienic?\n\t1. imie\n\t2. nazwisko\n\t3. grupe\n\t0. wyjscie\n\nPodaj wybor: ");
		Get_int(&choice, 0, 3);
		switch(choice){
			case 1: printf("\nPodaj nowe imie (max %i liter): ", MAX_SURNAME_LENGTH);
					Get_str(rec->surname, MIN_NAMEs_LENGTH, MAX_SURNAME_LENGTH);
					break;
			case 2: printf("\nPodaj nowe nazwisko (max %i liter): ", MAX_NAME_LENGTH);
					Get_str(rec->name, MIN_NAMEs_LENGTH, MAX_NAME_LENGTH);
					break;
			case 3: printf("\nPodaj nowa grupe: ");
					Get_int(&rec->group, MIN_GROUPS_AMOUNT, MAX_GROUPS_AMOUNT);
					break;
			default:return;
		}
	} while(Prompt("\nCzy chcesz zmienic cos innego u tego studenta?"));
}

void Edit_subjects_names(Sublist* sublist){
	Sublist* sub_to_edit;
	int num_of_subjects;

	do{
		if(num_of_subjects = Write_available_subjects(sublist)){	// if > 0
			printf("\n\nWybierz przedmiot do edycji nazwy (0 dla wyjscia): ");
			if(sub_to_edit = Select_subject_from_sublist(num_of_subjects, sublist)){
				printf("\nPodaj nowa nazwe dla wybranego przedmiotu (max %i znakow): ", MAX_SUBJ_DESC_LENGTH);
				Get_str(sub_to_edit->description, MIN_NAMEs_LENGTH, MAX_SUBJ_DESC_LENGTH);
			} else break;
		}
	} while(Prompt("\nCzy chcesz edytowac nazwy innych przedmiotow?"));
}


/* ##########    THE MAIN ENTRY POINT - MAIN FUNCTION    ########## */
int main(void)
{
	int choice = 1;		// to prevent closing the menu after starting

	/*
	   trying to open databases - if can't then setting up
	   NULL value to their pointers - clean database cases
	*/
	Record* rec = Load_list();
	Sublist* sublist = Load_sublist();

	while(choice){
		printf("\n\n\t#-------------------------------------------------#\n");
		printf("\t|                 DZIENNIK OCEN                   |\n");
		printf("\t#-------------------------------------------------#\n");
		printf("\n\t# 1. Zarzadzaj wszystkimi uczniami!");
		printf("\n\t# 2. Zarzadzaj uczniami z konkretnej grupy!");
		printf("\n\t# 3. Zarzadzaj uczniami z konkretnym przedmiotem!");
		printf("\n\t# 4. Znajdz ucznia!\n");
		printf("\n\t# 5. @@@@@@@ ZAPISZ WSZYSTKIE ZMIANY @@@@@@@\n");
		printf("\n\t# 6. Sortuj liste uczniow po nazwiskach!");
		printf("\n\t# 7. Sortuj liste uczniow po kolejnosci dodawania (ID)!");
		printf("\n\t# 8. Obroc liste uczniow!\n");
		printf("\n\t# 9. Dodaj nowego ucznia!");
		printf("\n\t#10. Usun wszystkich uczniow!\n");
		printf("\n\t#11. Dodaj nowy przedmiot!");
		printf("\n\t#12. Edytuj nazwy przedmiotow!");
		printf("\n\t#13. Usuwaj przedmioty!\n");
		printf("\n\t#14. Pomoc i informacje o programie!");
		printf("\n\t# 0. Wyjscie (nie zapomnij zapisac zmian)!");

		printf("\n\nWybierz opcje: ");
		Get_int(&choice, 0, 14);		// select an option

		switch(choice){
			case  0: /* EXITING */										break;
			case  1: Write_students_by_all(&rec, sublist);				break;
			case  2: Write_students_by_group(&rec, sublist);			break;
			case  3: Write_students_by_subject(&rec, sublist);			break;
			case  4: Filter_students_by_txt(&rec, sublist);				break;
			case  5: Save_list(rec);
					 Save_sublist(sublist);								break;
			case  6: Sort_list(&rec, Comp_by_names);					break;
			case  7: Sort_list(&rec, Comp_by_ID);						break;
			case  8: Reverse_list(&rec);								break;
			case  9: New_student(&rec, sublist);						break;
			case 10: Free_memory(&rec);									break;
			case 11: Add_subject_interface(&sublist);					break;
			case 12: Edit_subjects_names(sublist);						break;
			case 13: Delete_subjects_and_connections(rec, &sublist);	break;
			case 14: Show_info_and_help();								break;
			default: break;
		}
	}

	// cleaning up the environment before program exits
	Free_memory(&rec);
	Free_sublist(sublist);

	// checking the memory leaks (DEBUG mode)
	_CrtDumpMemoryLeaks();

	return 0;
}