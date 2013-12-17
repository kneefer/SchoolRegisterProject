/*
   ╔════════════════════════════════╗
   ║	   File err_handle.c		║
   ╚════════════════════════════════╝
*/


#include "errors.h"
#include <stdio.h>

void Catch_error(int err_code){
	switch(err_code){
		case ERR_TOOLESS:
			fprintf(stderr, "\n\tZbyt mala wartosc! Sprobuj ponownie: ");
			break;
		case ERR_TOOMUCH:
			fprintf(stderr, "\n\tZbyt duza wartosc! Sprobuj ponownie: ");
			break;
		case ERR_WRONGTYPE:
			fprintf(stderr, "\n\tZly typ danych wejsciowych! Sprobuj ponownie: ");
			break;
		case ERR_NOSUBJECTS:
			fprintf(stderr, "\n\tBrak przedmiotow! Dodaj je z menu glownego!");
			break;
		case ERR_NOATTACHEDSUBS:
			fprintf(stderr, "\n\tBrak przypisanych przedmiotow do biezacego studenta! Dodaj je!");
			break;
		case ERR_UNK_OPT:
			fprintf(stderr, "\n\tNieznana wybrana opcja!");
			break;
		case ERR_NOGRADES:
			fprintf(stderr, "\n\tBrak ocen! Dodaj zanim usuniesz ;)");
			break;
		case ERR_NOSTUDENTS:
			fprintf(stderr, "\n\tBrak studentow o tych kryteriach!");
			break;
		case ERR_SUBEXISTS:
			fprintf(stderr, "\n\tWybrany przedmiot juz istnieje!");
			break;
		case ERR_ILLEGALCHRS:
			fprintf(stderr, "\n\tUzyto niedozwolonych znakow! ('<', '>')!\nSprobuj ponownie: ");
			break;
		case ERR_NOBASEREC:
			fprintf(stderr, "\n\tNie wykryto bazy ze studentami! Pracujesz na czystej liscie!");
			break;
		case ERR_NOBASESUB:
			fprintf(stderr, "\n\tNie wykryto bazy z przedmiotami! Pracujesz na czystej liscie!");
			break;
		case ERR_REALLOC:
			fprintf(stderr, "\n\tBlad podczas powiekszania pamieci listy!!!");
			break;
		case ERR_NOTFOUND:
			fprintf(stderr, "\n\tNie znaleziono zadnego studenta o tych kryteriach!");
			break;
		case ERR_EMPTYLIST:
			fprintf(stderr, "\n\tLista jest pusta i nie wykonasz na niej tej operacji!");
			break;
		case ERR_SAVINGREC:
			fprintf(stderr, "\n\tNiepowodzenie zapisywania bazy studentow!");
			break;
		case ERR_SAVINGSUB:
			fprintf(stderr, "\n\tNiepowodzenie zapisywania bazy z przedmiotami!");
			break;
		default:
			fprintf(stderr, "\n\tNieznany blad! (ups :<)");
	}
}