#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mergesort.c"


/*
 Author: Hersh Patel
 Project: CSV Data Sorter
 Date: 10/2/17
 */

int main(int argc, char*argv[]) {
	
	//Error 1 - Not Enough Arguments Inputted
	if (argc<2) {
		printf("error - not enough arguments\n");
		return 0;
	}
	
	//Error 2 - First Argument is not a '-c'
	if (strcmp(argv[1],"-c")) {
		printf("error - first argument must be -c\n");
		return 0;
	}

	//Pre-Set Number of Rows and Columns to expect
	//Dynamically changes based on actual #s
	int num_rows=10000;
	int num_cols=15;
	
	char header_line[1000];
	char header_print[1000];
	
	fgets(header_line,1000,stdin);
	strcpy(header_print, header_line);
	//printf("$%s$\n", header_print);
	char *header_ptr=header_line;
	
	//Allocates space for 30 column headers (num_cols)
	header_data *headers = malloc(num_cols*sizeof(header_data));
	
	//Reads in row 0 as column names into array of header_data structs
	int col_count=0;
	while(col_count<num_cols) {
		if(col_count==(num_cols-1)) {
			num_cols=num_cols+15;
			headers = realloc(headers,num_cols*sizeof(header_data));
		}
		char *header_token = (char*)malloc(sizeof(char*)*500);
		header_token=strsep(&header_ptr, ",");
		if (!header_token) break;
		strcpy(headers[col_count].name,header_token);
		headers[col_count].num=col_count;
		headers[col_count].type=-1;
		col_count++;
	}
	
	//Error 3 - Second Argument is not a column name in csv
	//sort_on is the respective column number of the second argument
	int sort_on;
	for(sort_on=0; sort_on<=col_count; sort_on++){
		if (sort_on==col_count) {
			printf("error - column does not exist in csv\n");
			return 0;
		}
		if (strcmp(headers[sort_on].name,argv[2])==0)
			break;
	}
	
	//Allocates space for 10,000 rows of data (num_rows)
	row_data *data =malloc(num_rows*sizeof(row_data));
	char temp[1000];
	int row_count=0;
	while (fgets(temp,1000,stdin)) {
		char *line = temp;
		if(row_count==(num_rows-1)) {
			num_rows=num_rows*2;
			data=realloc(data, num_rows*sizeof(row_data));
		}
		data[row_count].fields=(char**) malloc(sizeof(char*)*col_count);
		data[row_count].special=(int*) calloc(col_count,sizeof(int));
		data[row_count].row_num=row_count;
		char *token = (char*)malloc(sizeof(char*)*500);
		int i;
		for(i=0; i<col_count; i++) {
			token = strsep(&line, ",");
		
			//removes quotes from string that has comma, stores value that indicates quotes were there
			if (hasQuote(token)) {
				data[row_count].special[i]=1;
				char *forward = (char*)malloc(sizeof(char*)*500);
				strcat(forward, token);
				strcat(forward,",");
				do {
					token=strsep(&line, ",");
					strcat(forward, token);
					if (!hasQuote(token)) strcat(forward, ",");
				} while (!hasQuote(token));
				char *temp = (char*)malloc(sizeof(char*)*500);
				int q;
				for(q=1;q<strlen(forward)-1;q++){
					temp[q-1]=forward[q];
				}
				token=temp;
			}
			
			//trims whitepsace for all inputs
			char *trimmed = (char*) malloc(sizeof(char*)*500);
			int start = 0;
			int end = strlen(token)-1;
			while(token[start]==' '){
				if (start==end) trimmed[0]=' ';
				start++;
			}
			while(token[end]==' '){
				if (start==end) trimmed[0]=' ';
				end--;
			}
			int j;
			for(j=start; j<=end; j++) {
				if (token[j+1]=='\n') continue;
				trimmed[j-start]=token[j];
			}
			if (!strcmp(trimmed,"") || !strcmp(trimmed," ")) {
				data[row_count].special[i]=-1;
				trimmed="";
			}
			
			//assigns or overrides column data type
			int typ3=getDataType(trimmed);
			if (typ3>-1) headers[i].type=typ3;
			
			//sets the token value in the row_data struct array
			data[row_count].fields[i]=(char*)malloc(sizeof(char)*500);
			strcpy(data[row_count].fields[i],trimmed);
		}
		row_count++;
	}
	
	//counts the number of data types in each row
	int c;
	for (c=0; c<col_count; c++) {
		if (headers[c].type>-1)
			num_each_type[headers[c].type]++;
		else num_each_type[3]++;
	}
	
	/*int r=1;
	int c1=11, c2=5, c3=26;
	printf("%d: $%d$ $%d$ $%d$\n", r, data[r].row_num, data[r].row_num,  data[r].row_num);
	printf("%d: $%s$ $%s$ $%s$\n", r, data[r].fields[c1], data[r].fields[c2],  data[r].fields[c3]);
	
	printf("%d %d %d %d\n", num_each_type[0],  num_each_type[1],  num_each_type[2],  num_each_type[3] );
	*/
	
	//reads all data again and stores as recognized data type in preparation for mergesort
	int row,col;
	for(row=0; row<row_count; row++) {
		data[row].strs=(char**) malloc(sizeof(char*)*num_each_type[0]);
		data[row].ints=(int*) malloc(sizeof(int)*num_each_type[1]);
		data[row].doubles=(double*) malloc(sizeof(double)*num_each_type[2]);
		data[row].locations=(int*) malloc(sizeof(int)*col_count);
		int str_ptr=0;
		int int_ptr=0;
		int double_ptr=0;
		for (col=0; col<col_count; col++) {
			char *temp = data[row].fields[col];
		
			//data type = 0 (strings)
			if (headers[col].type==0) {
				data[row].strs[str_ptr]=(char*)malloc(sizeof(char)*500);
				strcpy(data[row].strs[str_ptr],temp);
				data[row].locations[col]=str_ptr;
				str_ptr++;
			}
			
			//data type = 1 (ints)
			if (headers[col].type==1) {
				data[row].ints[int_ptr] = atoi(temp);
				data[row].locations[col]=int_ptr;
				int_ptr++;
			}
			
			//data type = 2 (doubles)
			if (headers[col].type==2) {
				sscanf(temp, "%lf", &data[row].doubles[double_ptr]);
				data[row].locations[col]=double_ptr;
				double_ptr++;
			}
		}
	}
	
	//sorts the csv based on the second argument (column name) using mergesort
	//handles the exception case where an empty csv is inputted with only a header row
	//handles the exception case where sorting is called on an entirely empty column
	if(row_count!=0 || headers[sort_on].type>-1)
		mergeSort(data, headers, 0, row_count-1, sort_on);
	
	
	//prints out the sorted data in csv format
	
	/*c=0;
	for (c=0; c<col_count; c++) {
		if (c==(col_count-1)) fprintf(stdout, "%s\n", headers[c].name);
		fprintf(stdout, "%s,", headers[c].name);
	}*/
	
	FILE *fp = fopen("SORTED.csv", "w");

	fprintf(fp, "%s", header_print);
	row=0,col=0;
	for (row=0; row<row_count; row++) {
		for (col=0; col<col_count; col++) {
			if (data[row].special[col]==-1)
				fprintf(stdout, ",");
			else if (data[row].special[col]==1)
				fprintf(fp, "\"%s\",", data[row].fields[col]);
			else
				fprintf(fp, "%s,", data[row].fields[col]);
		}
		printf("\n");
	}

	fclose(fp);	
	
	return 0;
}


