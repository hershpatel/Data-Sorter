/*
 Author: Hersh Patel
 Project: CSV Data Sorter
 Date: 10/2/17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the number of each data type in each row
//index 0 (# of strings), index 1 (# of ints), index 2 (# of doubles), index 3 (# of blanks)
int num_each_type[4] = {0};

//getDataType - returns -1 for blanks, 0 for strings, 1 for ints, 2 for doubles
int getDataType(char* token) {
	int isDouble=0;
	int found_dot=0;
	int i;
	if (!strcmp(token,"") || !strcmp(token," ")) return -1;
	for (i=0; i<strlen(token); i++) {
		if (isdigit(token[i]) || (token[i]=='.' && !found_dot)) {
			if (token[i]=='.') {
				isDouble=1;
				found_dot=1;
			}
		}
		else return 0;
	}
	if (isDouble) return 2;
	return 1;
}

//hasQuote - returns 1 if given string has a quote and 0 if it does not
int hasQuote(char* token) {
	int i;
	for (i=0; i<strlen(token); i++)
		if (token[i]=='"')
			return 1;
	return 0;
}

//Header Value struct
//int type = 0 (char), 1 (int), 2 (double)
typedef struct {
	char name[100];				//name of column
	int num;					//respective column #
	int type;					//(blanks=-1, strings=0, ints=1, doubles=2)
} header_data;

//Row_Data Struct
typedef struct {
	char **fields;				//array of strings with all of the fields
	int *special;				//handles blank values and quotes (blanks=-1, no special=0, quotes=1)
	int row_num;				//respective row # data is associated with
	char **strs;				//holds associated data values that are strs in array
	int *ints;					//holds associated data  values that are ints
	double *doubles;			//holds associated data values that are doubles in array
	int *locations;				//holds location of all data values in array (blank ints= -1)
} row_data;
