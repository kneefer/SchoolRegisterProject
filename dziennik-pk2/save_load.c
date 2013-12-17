/*
   ╔════════════════════════════════╗
   ║	    File save_load.c		║
   ╚════════════════════════════════╝
*/

#include "save_load.h"

BOOLEAN Save_sublist(Sublist* sublist){
	FILE* file;

	if((file = fopen(DATABASE_SUBJECTS, "w")) != NULL){
		while(sublist){
			fprintf(file, "%i %s\n", sublist->ID, sublist->description);
			sublist = sublist->next;
		}
		fclose(file);
		return TRUE;
	}
	Catch_error(ERR_SAVINGSUB);
	return FALSE;
}

Sublist* Load_sublist(void){
	FILE* file;
	char curr_ch;
	int i;
	Sublist *new_item, *mem = NULL;

	if((file = fopen(DATABASE_SUBJECTS, "r")) != NULL){
		while(!feof(file)){
			new_item = (Sublist*)malloc(sizeof(Sublist));
			/* reads the ID of subject and the space */
			if(!(fscanf(file, "%i ", &new_item->ID)) || feof(file)){
				free(new_item);
				break;
			}
			/* reads all chars till the end of line BUT saves only 15 first chars */
			for(i = 0; ((curr_ch = fgetc(file)) != '\n');){
				if(feof(file))
					break;
				if(i<(MAX_SUBJ_DESC_LENGTH))
					(new_item->description)[i++] = curr_ch;
			}
			(new_item->description)[i] = '\0';
			new_item->next = mem;
			mem = new_item;
		}
		fclose(file);
		return mem;
	}
	Catch_error(ERR_NOBASESUB);
	return NULL;
}

Ld_tag_return Ld_check_tag(FILE ** file){
	Ld_tag_return ret = {0,0};
	
	char curr_ch;
	char BUFF[MAX_TAG_LENGTH+1];								// +1 position for the '\0' character
	int i = 0;

	while(!feof(*file) && ((curr_ch = fgetc(*file)) != '<'));	// we're looking for the opening tag '<'
	if(feof(*file))
		return ret;
	if((curr_ch = fgetc(*file)) != '/')
		ungetc(curr_ch, *file);									// decrease a file position if we haven't a closing tag (with '/' character)
	else
		ret.closing = TRUE;										// we have closing tag (with '/' mark)
	while(!feof(*file) && ((curr_ch = fgetc(*file)) != '>') && i < MAX_TAG_LENGTH)  // now closing tag '>'
		BUFF[i++] = tolower(curr_ch);							// copying to the buffer in order to check the tag name
	if(feof(*file) || i >= MAX_TAG_LENGTH)
		return ret;
	BUFF[i] = '\0';												// inserting the end of the buffer
	strcpy(ret.tag_name, BUFF);									// providing the return value
	return ret;
}

float Ld_return_float(FILE **file, int max, char tag[]){
	float ret;
	Ld_tag_return next_tag;

	if(fscanf(*file, "%f", &ret) && ret <= max){
		next_tag = Ld_check_tag(file);
		if(!strcmp(tag, next_tag.tag_name) && next_tag.closing) // 1. the tag is close type?
			return ret;											// 2. the tag is closing the same tag?
	}
	return -1;								// value indicating the error appeared
}

char* Ld_return_string(FILE **file, int max, char tag[]){
	char curr_ch;
	int i = 0;
	Ld_tag_return next_tag;
	char *BUFF = (char*)malloc(max+1);		// we have to free it later

	while(!feof(*file) && ((curr_ch = fgetc(*file)) != '<') && (i < max))
		BUFF[i++] = curr_ch;
	if(!feof(*file) && i < max){
		ungetc('<', *file);					// next instructions need this '<' mark
		BUFF[i] = '\0';
		next_tag = Ld_check_tag(file);
		if(!strcmp(tag, next_tag.tag_name) && next_tag.closing)
			return BUFF;
	}
	free(BUFF);								// file is corrupted
	return NULL;
}

int Ld_return_int(FILE **file, int max, char tag[]){
	int ret;
	Ld_tag_return next_tag;

	if(fscanf(*file, "%i", &ret) && ret <= max){
		next_tag = Ld_check_tag(file);
		if(!strcmp(tag, next_tag.tag_name) && next_tag.closing) // 1. the tag is close type?
			return ret;											// 2. the tag is closing the same tag?
	}
	return -1;	// value indicating the error appeared
}

void Ld_add_subject_to_student(Subject* src, Subject** dst){
	src->next = *dst;
	*dst = src;
}

void Ld_add_student_to_list(Record* src, Record** dst){
	src->next = *dst;
	*dst = src;
}

void Ld_add_grade_to_subject(Grade* src, Grade** dst){
	src->next = *dst;
	*dst = src;
}

Grade* Ld_get_grade(FILE **file){
	Ld_tag_return tag;
	Grade* ret = NULL;
	float ret_float;

	while(TRUE){
		tag = Ld_check_tag(file);								// getting next tag

		if(!strcmp(tag.tag_name, REC_GRADEVAL)) {				// if <grade> tag
			if((ret_float = Ld_return_float(file, MAX_GRADE_VALUE, tag.tag_name)) == -1){
				free(ret);						 // if errors
				return NULL;
			}
			ret = (Grade*)malloc(sizeof(Grade)); // if ok
			ret->next = NULL;
			ret->value = ret_float;		
			return ret;
		} else return NULL;
	}
}

Subject* Ld_get_subject(FILE **file){
	Ld_tag_return tag;
	Subject* ret = NULL;
	Grade* grade = NULL;
	int ret_int;
	char* ret_string;

	tag = Ld_check_tag(file);

	if(strcmp(tag.tag_name, REC_SUBJECT))			// checking if the subject begins with <subject> tag
		return ret;
	ret = (Subject*)malloc(sizeof(Subject));
	ret->next = NULL;
	ret->grade = NULL;

	while(TRUE){
		tag = Ld_check_tag(file);										// getting next tag

		if(!strcmp(tag.tag_name, REC_ID)) {								// if <ID> tag
			if((ret_int = Ld_return_int(file, MAX_ID_VALUE, tag.tag_name)) == -1){
				free(ret);
				return NULL;		// if errors
			}
			ret->ID = ret_int;		// if ok
		} else if(!strcmp(tag.tag_name, REC_COMMENT)){					// if <comment> tag
			if(ret_string = Ld_return_string(file, MAX_COMMENT_LENGTH, tag.tag_name)){
				strcpy(ret->comment, ret_string);
				free(ret_string);	// if ok
				continue;
			}
			return NULL;			// if errors
		} else if(!strcmp(tag.tag_name, REC_GRADES)){					// if <grades> tag
			if(tag.closing) continue;									// if </grades> tag
			while(TRUE){
				if(grade = Ld_get_grade(file))
					Ld_add_grade_to_subject(grade, &ret->grade);
				else break;
			}
		} else if(!strcmp(tag.tag_name, REC_SUBJECT) && tag.closing)	// if </subject> tag
			return ret;				// if errors
		else {
			free(ret);
			return NULL;			// if ok
		}
	}
}

Record* Ld_get_student(FILE **file){
	Record *rec = NULL;
	Subject *subject = NULL;
	Ld_tag_return tag;
	int ret_int;
	char* ret_string;

	tag = Ld_check_tag(file);					// we're looking for <student> tag
	if(strcmp(tag.tag_name, REC_STUDENT))		// checking if the record begins with '<student>' tag
		return rec;
	rec = (Record*)malloc(sizeof(Record));
	rec->next = NULL;
	rec->subjects = NULL;

	while(TRUE){								// getting next tag
		tag = Ld_check_tag(file);

		if(!strcmp(tag.tag_name, REC_ID)) {							// if <ID> tag
			if((ret_int = Ld_return_int(file, MAX_ID_VALUE, tag.tag_name)) == -1){
				free(rec);
				return NULL;					// if errors
			}
			rec->ID = ret_int;					// if ok
		} else if(!strcmp(tag.tag_name, REC_NAME)) {				// if <name> tag
			if(ret_string = Ld_return_string(file, MAX_NAME_LENGTH, tag.tag_name)) {
				strcpy(rec->name, ret_string);	// if ok
				free(ret_string);
				continue;
			}
			return NULL;						// if errors
		} else if(!strcmp(tag.tag_name, REC_SURNAME)) {			// if <surname> tag
			if(ret_string = Ld_return_string(file, MAX_SURNAME_LENGTH, tag.tag_name)) {
				strcpy(rec->surname, ret_string);
				free(ret_string);				// if ok
				continue;
			}
			return NULL;						// if errors
		} else if(!strcmp(tag.tag_name, REC_GROUP)){			// if <group> tag
			if((ret_int = Ld_return_int(file, MAX_GROUPS_AMOUNT, tag.tag_name)) == -1){
				free(rec);						// if errors
				return NULL;
			}	
			rec->group = ret_int;				// if ok
		} else if(!strcmp(tag.tag_name, REC_SUBJECTS)){			// if <subjects> tag
			if(tag.closing) continue;							// if </subjects> tag
			while(TRUE){
				if(subject = Ld_get_subject(file))
					Ld_add_subject_to_student(subject, &rec->subjects);
				else break;
			}
		} else if(!strcmp(tag.tag_name, REC_STUDENT) && tag.closing)	// if </student> tag
			return rec;	
		else {									// the rest of possible errors
			free(rec);							
			return NULL;
		}
	}
}

Record* Load_list(void){
	FILE* file;
	Record *new_item, *list = NULL;
	if((file = fopen(DATABASE_DATA, "r")) != NULL){
		while(!feof(file))
			if(new_item = Ld_get_student(&file))			// while there are any <student>[...]</student> entries
				Ld_add_student_to_list(new_item, &list);	// ... take next student and add it to the list
		fclose(file);										
		return list;
	}
	Catch_error(ERR_NOBASEREC);
	return NULL;
}

BOOLEAN Save_list(Record * rec){
	FILE* file;
	Subject* subject;
	Grade* grade;

	if((file = fopen(DATABASE_DATA, "w")) != NULL){
		// ##### BEGIN OF WRITING OUT "STUDENT" #####
		while(rec){
			fprintf(file, "<"REC_STUDENT">\n"
			 "\t<"REC_ID">%i</"REC_ID">\n"
			 "\t<"REC_NAME">%s</"REC_NAME">\n"
			 "\t<"REC_SURNAME">%s</"REC_SURNAME">\n"
			 "\t<"REC_GROUP">%i</"REC_GROUP">\n"
			 "\t<"REC_SUBJECTS">\n",
			 rec->ID, rec->name, rec->surname, rec->group);

			// ##### BEGIN OF WRITING OUT "SUBJECTS" #####
			subject = rec->subjects;
			while(subject){
				fprintf(file, "\t\t<"REC_SUBJECT">\n"
				 "\t\t\t<"REC_ID">%i</"REC_ID">\n"
				 "\t\t\t<"REC_COMMENT">%s</"REC_COMMENT">\n"
				 "\t\t\t<"REC_GRADES">\n",
				 subject->ID, subject->comment);

				// ##### BEGIN OF WRITING OUT "GRADES" #####
				grade = subject->grade;
				while(grade){
					fprintf(file, "\t\t\t\t<"REC_GRADEVAL">%f</"REC_GRADEVAL">\n", grade->value);
					grade = grade->next;
				}

				fprintf(file, "\t\t\t</"REC_GRADES">\n\t\t</"REC_SUBJECT">\n");
				// ##### END OF WRITING OUT "GRADES" #####
				subject = subject->next;
			}

			fprintf(file, "\t</"REC_SUBJECTS">\n</"REC_STUDENT">\n");
			// ##### END OF WRITING OUT "SUBJECTS" #####
			rec = rec->next;
		}
		// ##### END OF WRITING OUT "STUDENT" #####
		fclose(file);
		return TRUE;
	}
	Catch_error(ERR_SAVINGREC);
	return FALSE;

}