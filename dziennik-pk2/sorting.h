/*
   ╔════════════════════════════════╗
   ║	    File sorting.h			║
   ╚════════════════════════════════╝
*/

#ifndef _sorting_H_
#define	_sorting_H_

// ##### Include's import #####

#include <string.h>
#include "header.h"
#include "errors.h"

// ##### Constants Declarations #####

#define GUARD_INT_VAL			100					// values to make sure that the guards will be
#define GUARD_CHR_VAL			255					// the highest value in every domain we consider

// ##### Functions Definitions #####

void Sort_list(Record**, int(*)(const void*, const void*));	// sorts the Record list and also takes pointer
int Comp_by_names(const void*, const void*); // comp #1  1	   to comparing function
int Comp_by_ID(const void*, const void*);	 // comp #2
void Reverse_list(Record**);

#endif