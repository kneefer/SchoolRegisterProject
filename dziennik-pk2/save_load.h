/*
   ╔════════════════════════════════╗
   ║	   File save_load.h			║
   ╚════════════════════════════════╝
*/

#ifndef _save_load_H_
#define	_save_load_H_

// ##### Include's import #####

#include "header.h"
#include "errors.h"
#include <ctype.h>

// ##### Constants Declarations #####

#define REC_STUDENT				"student"
#define REC_ID					"id"
#define REC_NAME				"name"
#define REC_SURNAME				"surname"
#define REC_GROUP				"group"
#define	REC_SUBJECTS			"subjects"
#define REC_SUBJECT				"subject"
#define REC_COMMENT				"comment"
#define REC_GRADES				"grades"
#define REC_GRADEVAL			"grade"

#define DATABASE_SUBJECTS		"subjects.dat"
#define DATABASE_DATA			"data.dat"

#define MAX_TAG_LENGTH			10	// among all REC_(...) values above

// ##### Structures Definitions #####

typedef struct Ld_tag_return {		// stores information about read tag
	char tag_name[MAX_TAG_LENGTH+1];
	BOOLEAN closing;
} Ld_tag_return;

// ##### Functions Definitions #####

BOOLEAN Save_sublist(Sublist*);		// saves the list of subjects to file of subjects and returns success of operation
Sublist* Load_sublist(void);		// loads the list of subjects from file and returns it
BOOLEAN Save_list(Record *);			// saves the list of students (main list) to file and returns success of operation
Record* Load_list(void);			// loads the list of students (main list) from file and returns it

#endif
