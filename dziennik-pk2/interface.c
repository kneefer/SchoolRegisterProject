/*
   ╔════════════════════════════════╗
   ║		File interface.c		║
   ╚════════════════════════════════╝
*/

#include "interface.h"

BOOLEAN Prompt(char* str){
	char ch;

	printf("%s (t/n)",str);
	while((ch = _getch()) != 't' && ch != 'n');	// until getch() gets the supported key (t/n)
	printf("\n");

	switch (ch){
		case 't': return TRUE;
		case 'n': return FALSE;
	}
	return FALSE;
}

void Get_float(float* fl, int min, int max){
	while(TRUE){
		fflush(stdin);					// getting rid of trashes in keyboard buffer - Windows only
		if(!scanf("%f", fl)){
			Catch_error(ERR_WRONGTYPE);
			continue;
		}
		if(*fl < min){
			Catch_error(ERR_TOOLESS);
			continue;
		}
		if(*fl > max){
			Catch_error(ERR_TOOMUCH);
			continue;
		}
		break;
	}
}

void Get_str(char* string, int min, int max){	// ommits the characters beyond 'max' val
	char* str = (char*)malloc(max+1);			// incrementing 'max' because fgets reads [strlen(str)-1] characters
	BOOLEAN ill_chr;
	int i;

	while(TRUE){
		fflush(stdin);							// getting rid of trashes in keyboard buffer - Windows only
		if(!fgets(str, (max+1), stdin)){
			Catch_error(ERR_UNKNOWN);
			continue;
		}										// checking if the string length is lower than minimum
		if((int)strlen(str) < (min+1)){
			Catch_error(ERR_TOOLESS);
			continue;
		}
		ill_chr = FALSE;
		for(i=0;str[i];i++){
			/* checking if the string consists illegal characters */
			if((str[i] == '>') || (str[i] == '<') || (str[i] == '\\')){
				Catch_error(ERR_ILLEGALCHRS);
				ill_chr = TRUE;
			}
			/* checking if the new line mark has been read */
			if(str[i] == '\n')
				str[i] = '\0';
		}
		if(!ill_chr) break;
	}
	strcpy(string, str);
	free(str);
}

void Get_int(int* integer, int min, int max){
	while(TRUE){
		fflush(stdin);					// getting rid of trashes in keyboard buffer - Windows only
		if(!scanf("%i", integer)){
			Catch_error(ERR_WRONGTYPE);
			continue;
		}
		if(*integer < min){
			Catch_error(ERR_TOOLESS);
			continue;
		}
		if(*integer > max){
			Catch_error(ERR_TOOMUCH);
			continue;
		}
		break;
	}
}

int Write_grades_of_subject(Grade* grades){
	int i = 0;

	while(grades){
		printf("\n%*i. %.*f", ENUM_WIDTH, ++i, FLOAT_PRECISION, grades->value);
		grades = grades->next;
	}
	if(!i)
		Catch_error(ERR_NOGRADES);
	return i;
}

int Write_grades_of_student_of_subject_inline(Subject* subject, int new_line_offset){
	Grade* mem = subject->grade;
	int i = 0;

	/* spacing before grades block */
	printf("%*s", GRADES_BEF_SPACING, " ");
	while(mem){
		/* repeating instruction below every 5 iterations to align new line of grades */
		if(i++ && (!((i-1) % NUM_OF_GRADES_IN_LINE))){	// i incrementing
			printf("\n");
			/* inserting appropriate spacing in the new line of grades */
			printf("%*s", new_line_offset+GRADES_BEF_SPACING, " ");
		}
		printf("%-*.*f ", GRADE_WIDTH, FLOAT_PRECISION, mem->value);
		mem = mem->next;
	}
	return i;
}

int Write_available_subjects(Sublist* sublist){
	int i = 0;
	while(sublist){
		printf("\n%*i. %s", ENUM_WIDTH, ++i, sublist->description);
		sublist = sublist->next;
	}
	if(!i)
		Catch_error(ERR_NOSUBJECTS);
	return i;
}

void Write_current_student(Record* rec, Sublist* sublist){
	Subject* mem = rec->subjects;
	int num_of_grades;
	int i = 0;

	printf("\nBiezacy student:\n\tImie:\t\t%s\n\tNazwisko:\t%s\n\tGrupa:\t\t%i\n\nOceny:", rec->surname, rec->name, rec->group);

	/* while there are any subjects attached to student */
	while(mem){		
		printf("\n%*i. %-*s", ENUM_WIDTH, ++i, MAX_SUBJ_DESC_LENGTH, Find_subject_name_by_ID(mem->ID, sublist));
		/* if no grades in current subject */						/* evaluated from values 1 line above */
		if(!(num_of_grades = Write_grades_of_student_of_subject_inline(mem, ENUM_WIDTH+2+MAX_SUBJ_DESC_LENGTH)))
			Catch_error(ERR_NOGRADES);

		/* if the comment exists */
		if(strlen(mem->comment))
			/* 16 - length of 'komentarz: ' + 5 letters */
			printf("\n%*s%s\n", (11+ENUM_WIDTH+2), "Komentarz: ",  mem->comment); 
		mem = mem->next;
	}
	if(!i)		// if there weren't any subjects of current student
		Catch_error(ERR_NOATTACHEDSUBS);
}

int Write_subjects_of_student(Subject* subject, Sublist* sublist){	// writes out all subjects attached
	int i = 0;														// to the current student

	while(subject){
		printf("\n%*i. %-*s", ENUM_WIDTH, ++i, MAX_SUBJ_DESC_LENGTH, Find_subject_name_by_ID(subject->ID, sublist));
		subject = subject->next;
	}
	if(!i)
		Catch_error(ERR_NOATTACHEDSUBS);
	return i;
}

BOOLEAN Fill_new_student(Record* list, Record* rec){
	printf("\n\nPodaj imie (max %i liter): ", MAX_SURNAME_LENGTH);
	Get_str(rec->surname, MIN_NAMEs_LENGTH, MAX_SURNAME_LENGTH);
	printf("Podaj nazwisko (max %i liter): ", MAX_NAME_LENGTH);
	Get_str(rec->name, MIN_NAMEs_LENGTH, MAX_NAME_LENGTH);
	printf("Podaj nr grupy: ");
	Get_int(&(rec->group), MIN_GROUPS_AMOUNT, MAX_GROUPS_AMOUNT);
	
	rec->ID = Generate_new_ID(list);	// looking for a free ID for a new student
	rec->subjects = NULL;		
	rec->next = NULL;

	return (Prompt("\nZapisac zmiany?"));
}

void Add_grades_to_student(Record* rec, Sublist* sublist){
	int i;
	Subject* subject;

	do{
		if(i = Write_subjects_of_student(rec->subjects, sublist)){
			printf("\nKtory przedmiot ucznia chcesz wybrac? (0 aby anulowac): ");
			if(subject = Select_subject_of_student(i, rec)){		//  if not abort
				do{
					printf("\n\nDodajesz oceny w przedmiocie \"%s\"! Podaj wartosc oceny: ", Find_subject_name_by_ID(subject->ID, sublist));
					Add_grades(subject);
				} while (Prompt("\nCzy chcesz dodac kolejna ocene w biezacym przedmiocie?"));
			} else break;
		}
	} while(Prompt("\nCzy chcesz dodac oceny do innych przedmiotow?"));
}

void Add_subject_interface(Sublist** sublist){
	do{
		printf("\nLista dostepnych przedmiotow: ");
		Write_available_subjects(*sublist);
		printf("\n\nPodaj nazwe nowego przedmiotu: ");
		Add_subject_engine(sublist);
	} while(Prompt("\nCzy chcesz dodac kolejne przedmioty?"));
}

void Add_subjects_to_student_interface(Record* rec, Sublist* subject){
	int i;
	Sublist* sub_to_add;

	do{
		if(i = Write_available_subjects(subject)){		// executing when there are any subjects
			printf("\n\nKtory przedmiot chcesz dodac? (0 aby wyjsc): ");
			/* takes a subject to add and prevents duplications */
			if(sub_to_add = Check_selected_subject_to_add_to_student(i, subject, rec))
				Add_subjects_to_student_engine(rec, sub_to_add);
			else break;
		} else {
			Catch_error(ERR_NOSUBJECTS);
			break;
		}
	} while(Prompt("\nPrzypisac wiecej przedmiotow?"));
}

BOOLEAN New_student(Record** list, Sublist* subject){
	Record* rec = (Record*)malloc(sizeof(Record));
	
	printf("\nDodawanie nowego studenta!");
	if(Fill_new_student(*list, rec)){
		if(Prompt("\nDodac przedmioty?")){
			printf("\nPowiazywanie studentow z przedmiotami!");
			Add_subjects_to_student_interface(rec, subject);
			if(Prompt("\nDodac oceny?")){
				printf("\nDodawanie ocen do przedmiotow!");
				Add_grades_to_student(rec, subject);
			}
		}
		Add_student_to_list_of_students(list, rec);
		return TRUE;
	} else {
		Free_student(&rec);
		return FALSE;
	}
}

void Delete_subjects_of_student(Record* rec, Sublist* sublist){
	int i;						// > 0 if there are any subjects
	Subject* sub_to_delete;		// no comment ;)
	
	do{
		printf("\n\nUsuwanie przedmiotow studenta \"%s %s\"!", rec->surname, rec->name);
		if(i = Write_subjects_of_student(rec->subjects, sublist)){
			printf("\nKtory przedmiot ucznia chcesz wybrac? (0 aby anulowac): ");
			if(sub_to_delete = Select_subject_of_student(i, rec))
				Delete_subject_of_student(&sub_to_delete, &rec->subjects);
			else break;
		} else break;
	} while(Prompt("\nCzy chcesz usunac kolejny przedmiot tego studenta?"));
}

void Delete_grades_of_subject(Subject* subject, Sublist* sublist){
	int i;
	Grade* gr_to_delete;

	do{
		printf("\nUsuwanie ocen w przedmiocie \"%s\"!", Find_subject_name_by_ID(subject->ID, sublist));
		if(i = Write_grades_of_subject(subject->grade)){
			printf("\n\nKtora ocene chcesz wybrac do usuniecia? (0 aby anulowac): ");
			if(gr_to_delete = Select_grade_of_subject(i, subject))
				Delete_grade(&gr_to_delete, &subject->grade);
			else break;
		} else break;
	} while(Prompt("\nCzy chcesz usunac inne oceny z tego przedmiotu?"));
}

void Delete_grades_of_student(Record* rec, Sublist* sublist){
	int i;
	Subject* selected_subject;
	
	do{
		printf("\n\nUsuwanie ocen w przedmiotach studenta \"%s %s\"!", rec->name, rec->surname);
		if(i = Write_subjects_of_student(rec->subjects, sublist)){
			printf("\nKtory przedmiot ucznia chcesz wybrac? (0 aby anulowac): ");
			if(selected_subject = Select_subject_of_student(i, rec))
				Delete_grades_of_subject(selected_subject, sublist);
		} else break;
	} while(Prompt("\nCzy chcesz usunac oceny w innym przedmiocie?"));
}

void Manage_student(Record* rec, Record** list, Sublist* sublist){
	int option;

	do{
		Write_current_student(rec, sublist);

		printf("\n\nWybierz opcje:\n\
			\r\t1. Edytuj informacje osobiste\n\
			\r\t2. Usun biezacego studenta\n\
			\r\t3. Powiaz przedmioty z biezacym studentem\n\
			\r\t4. Usun powiazane przedmioty studenta\n\
			\r\t5. Dodaj oceny (do konkretnego przedmiotu)\n\
			\r\t6. Usun oceny (od konkretnego przedmiotu)\n\
			\r\t7. Edytuj komentarz (do konkretnego przedmiotu)\n\
			\r\t0. Wyjscie\n\nWybor opcji: ");
		
		Get_int(&option, 0, 7);
		if(!option) return;		// if user wants to exit

		switch(option){
			case MANAGE_EDIT_PERSONAL:	Edit_personal(rec);									break;
			case MANAGE_DEL_STUDENT:	Delete_student(&rec, list);							return;
			case MANAGE_ADD_SUBJECTS:	Add_subjects_to_student_interface(rec, sublist);	break;
			case MANAGE_DEL_SUBJECTS:	Delete_subjects_of_student(rec, sublist);			break;
			case MANAGE_ADD_GRADES:		Add_grades_to_student(rec, sublist);				break;
			case MANAGE_DEL_GRADES:		Delete_grades_of_student(rec, sublist);				break;
			case MANAGE_EDIT_COMMENT:	Edit_comment(rec, sublist);							break;
			default:					Catch_error(ERR_UNK_OPT);
		}
	} while(Prompt("\nCzy chcesz wykonac kolejne operacje na tym studencie?"));
}

void Filter_students_by_txt(Record** rec, Sublist* sublist){
	Record** results = NULL;
	char str[MAX_CHRS_CMP_FIND];
	int i;

	do{
		if(!(*rec)){	// for empty list of students
			Catch_error(ERR_EMPTYLIST);
			break;
		}

		printf("\nPodaj ciag znakow do znalezienia (max %i znakow): ", MAX_CHRS_CMP_FIND);
		Get_str(str, 1, MAX_CHRS_CMP_FIND);
		/* writing out all found students */
		if(results = Find_students_by_txt(*rec, str)){
			/* writing header only when there is any record */
			if(results[0])
				printf("\n%-*s  %-*s%-*s%*s", ENUM_WIDTH, "Lp", MAX_NAME_LENGTH, "Nazwisko", MAX_SURNAME_LENGTH, "Imie", GROUP_WIDTH, "Grupa\n");
			for(i=0;results[i];i++)
				printf("\n%*i. %-*s%-*s%*i", ENUM_WIDTH, (i+1), MAX_NAME_LENGTH, results[i]->name, MAX_SURNAME_LENGTH, results[i]->surname, GROUP_WIDTH, results[i]->group);
			printf("\n\nWybierz studenta do dalszych operacji (0 dla wyjscia): ");
			/* now 'i' variable is choice of user */
			Get_int(&i, 0, i);
			if(i)	// if user doesn't want to exit
				Manage_student(results[i-1], rec, sublist);
		}
		free(results);
		results = NULL;
	} while(Prompt("\nCzy chcesz sprobowac wyszukac ponownie?"));
}

void Write_students_by_all(Record** rec, Sublist* sublist){
	int num_of_students, choice;
	Record* mem;

	do{
		mem = *rec;
		if(mem)		// writing header only when there is any record
			printf("\n%-*s  %-*s%-*s%*s", ENUM_WIDTH, "Lp", MAX_NAME_LENGTH, "Nazwisko", MAX_SURNAME_LENGTH, "Imie", GROUP_WIDTH, "Grupa\n");
		for(num_of_students = 0; mem; num_of_students++){
			printf("\n%*i. %-*s%-*s%*i", ENUM_WIDTH, (num_of_students+1), MAX_NAME_LENGTH, mem->name, MAX_SURNAME_LENGTH, mem->surname, GROUP_WIDTH, mem->group);
			mem = mem->next;
		}

		if(num_of_students){	// if there were any students 
			printf("\n\nPodaj nr studenta do dalszych operacji! (0 dla wyjscia): ");
			Get_int(&choice, 0, num_of_students);
			if(choice)			// if 0 not chosen
				Manage_student(Find_student_by_number(choice, *rec), rec, sublist);
			else break;
		} else {
			Catch_error(ERR_NOSTUDENTS);
			break;
		}
	} while(Prompt("\n\nCzy chcesz wypisac wszystkich studentow ponownie i wykonac inne akcje?"));
}

void Write_students_by_group(Record** rec, Sublist* sublist){
	int num_of_students, choice, group;
	Record* mem;

	do {
		num_of_students = 0;
		mem = *rec;
		printf("\n\nZ ktorej grupy chcesz wypisac studentow? (0 dla wyjscia): ");
		Get_int(&group, 0, MAX_GROUPS_AMOUNT);
		if(!group) break;		// if 0 chosen (for exit)

		if(mem)		// writing header only when there is any record
			printf("\n%-*s  %-*s%-*s%*s", ENUM_WIDTH, "Lp", MAX_NAME_LENGTH, "Nazwisko", MAX_SURNAME_LENGTH, "Imie", GROUP_WIDTH, "Grupa\n");

		while(mem){
			if(mem->group == group)
				printf("\n%*i. %-*s%-*s%*i", ENUM_WIDTH, (++num_of_students), MAX_NAME_LENGTH, mem->name, MAX_SURNAME_LENGTH, mem->surname, GROUP_WIDTH, mem->group);
			mem = mem->next;
		}

		if(num_of_students){	// if there were any students 
			printf("\n\nPodaj nr studenta do dalszych operacji! (0 dla wyjscia): ");
			Get_int(&choice, 0, num_of_students);
			if(choice)			// if 0 not chosen
				Manage_student(Find_student_by_number_and_group(choice, *rec, group), rec, sublist);
		} else {
			Catch_error(ERR_NOSTUDENTS);
		}
	} while(Prompt("\n\nCzy chcesz wypisac studentow z innych grup?"));
}

void Write_students_by_subject(Record** rec, Sublist* sublist){
	int num_of_students, choice, num_of_subjects;
	Record* mem;
	Sublist* subject;
	Subject* subject_of_student;

	do {
		num_of_students = 0;
		mem = *rec;
		printf("\n\nZ ktorego przedmiotu chcesz wypisac studentow? (0 dla wyjscia): ");

		if(!(num_of_subjects = Write_available_subjects(sublist))){
			Catch_error(ERR_NOSUBJECTS);
			break;
		}
		printf("\n\nWybor opcji: ");
		if(!(subject = Select_subject_from_sublist(num_of_subjects, sublist))) break;	// if the will to exit

		if(mem)		// writing header only when there is any record
			printf("\n%-*s  %-*s%-*s%*s  %s", ENUM_WIDTH, "Lp", MAX_NAME_LENGTH, "Nazwisko", MAX_SURNAME_LENGTH, "Imie", GROUP_WIDTH, "Grupa", "Oceny\n");

		while(mem){		// writing out all students who fulfill conditions
			if(subject_of_student = Check_if_subject_attached_to_student(mem, subject)){
				printf("\n%*i. %-*s%-*s%*i", ENUM_WIDTH, (++num_of_students), MAX_NAME_LENGTH, mem->name, MAX_SURNAME_LENGTH, mem->surname, GROUP_WIDTH, mem->group);
				Write_grades_of_student_of_subject_inline(subject_of_student, ENUM_WIDTH+2+MAX_NAME_LENGTH+MAX_SURNAME_LENGTH+GROUP_WIDTH);
			}
			mem = mem->next;
		}

		if(num_of_students){	// if there were any students 
			printf("\n\nPodaj nr studenta do dalszych operacji! (0 dla wyjscia): ");
			Get_int(&choice, 0, num_of_students);
			if(choice)			// if 0 not chosen
				Manage_student(Find_student_by_number_and_subject(choice, *rec, subject->ID), rec, sublist);
			else break;
		} else {
			Catch_error(ERR_NOSTUDENTS);
			break;
		}
	} while(Prompt("\n\nCzy chcesz wypisac studentow z innych przedmiotow?"));
}

void Delete_subjects_and_connections(Record* rec, Sublist** sublist){
	Sublist* selected_subject;
	Subject* curr_subject_of_student_to_del;
	int num_of_subjects;

	do{
		if(num_of_subjects = Write_available_subjects(*sublist)){	// if > 0
			printf("\n\nWybierz przedmiot do usuniecia z listy przedmiotow\ni powiazanych studentow wraz z ocenami (0 dla wyjscia)!: ");
			if(!(selected_subject = Select_subject_from_sublist(num_of_subjects, *sublist))) break; // if the will to exit

			while(rec){
				/* if selected subject is attached to current student */
				if(curr_subject_of_student_to_del = Check_if_subject_attached_to_student(rec, selected_subject))
					Delete_subject_of_student(&curr_subject_of_student_to_del, &(rec->subjects));
				rec = rec->next;
			}

			Delete_subject_from_sublist(&selected_subject, sublist);
		} else {
			Catch_error(ERR_EMPTYLIST);
			break;
		}
	} while(Prompt("\nCzy chcesz usunac inne przedmioty?"));
}

void Show_info_and_help(void){
	printf("\t#-------------------------------------------------#\n");
	printf("\t|          POMOC I INFORMACJE O PROGRAMIE         |\n");
	printf("\t#-------------------------------------------------#\n");
	printf("\n\t#1. Autorem programu jest Szymon Bartnik z 2 gr. (2012)");
	printf("\n\t    kierunku informatyka Politechniki Slaskiej\n");
	printf("\n\t#2. Kontakt: kneefer@gmail.com\n");
	printf("\n\t#3. Program jest praca zaliczeniowa na zajeciach z PK2");
	printf("\n\t    w semestrze letnim roku akademickiego 2012/2013\n");
	printf("\n\t#4. Celem zadania bylo napisanie programu - dziennika");
	printf("\n\t    umozliwiajacego przechowywanie ocen i uwag\n");
	printf("\n\t#5. Obowiazkowe bylo uzycie struktur dynamicznych");
	printf("\n\t    w jezyku ANSI C - program ma 3 listy podwieszane\n");
	printf("\n\t#6. Po wprowadzeniu zmien w bazie NALEZY pamietac");
	printf("\n\t    o zapisaniu wszystkich zmian. W przeciwnym razie");
	printf("\n\t    zostana one utracone\n");
	printf("\n\t#7. W katalogu z programem znajduja sie 2 pliki...");
	printf("\n\t    - data.dat - zawiera dane o studentach i ocenach");
	printf("\n\t    - subjects.dat - zawiera liste przedmiotow");
	printf("\n\t    ... ktore zawieraja przykladowy zestaw danych\n");
	printf("\n\t#8. Gdy program nie wykryje ktoregos z tych plikow");
	printf("\n\t    to zostanie on automatycznie stworzony\n");
	printf("\n\t#9. Dane w pliku dane.dat sa zapisane w formacie XML,");
	printf("\n\t    umozliwiajacym latwa edycje danych i eksport\n");

	printf("\n\nNacisnij dowolny klawisz, by powrocic do menu glownego!\n\n");
	_getch();
}