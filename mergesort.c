#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"

/*
 Author: Hersh Patel
 Project: CSV Data Sorter
 Date: 10/2/17
 */

/* Merge sort algorithm calls for recursively splitting array into separate elements
 and then merging off the individual elements while sorting them */

void merge(row_data orig[], header_data headers[], int left, int mid, int right, int col);
void merge1(row_data orig[], header_data headers[], int left, int mid, int right, int col);
void merge2(row_data orig[], header_data headers[], int left, int mid, int right, int col);

void mergeSort(row_data orig[], header_data headers[], int left, int right, int col){
	
	//Only perform recursive mergesort if working on valid portion of array
	if(left < right){
		
		//Find middle
		int mid = (left + right)/2;
		
		//Perform mergeSort on array values to left and right of middle point
		mergeSort(orig, headers, left, mid, col);
		mergeSort(orig, headers, mid+1, right, col);
		
		/* After mergeSort has reached arrays with "one value", merge back.
		 Although this is not the most efficient way, we decided to break
		 the merging aspect into 28 different merges based on the different
		 types of columns that are possible within the struct / dataset. */
		switch (headers[col].type){
			//data type - string
			case 0:
				merge(orig, headers, left, mid, right, col);
				break;
			 
			 //data type - int
			case 1:
				merge1(orig, headers, left, mid, right, col);
				break;
				
			//data type - double
			case 2:
				merge2(orig, headers, left, mid, right, col);
				break;
		}
	}
}


//data type - string
void merge(row_data orig[], header_data headers[], int left, int mid, int right, int col){
	//Create temporary arrays to copy elements from original array into.
	//Right Array is from middle + 1 to right index
	//Left Array is from left index to middle; add 1 to size because left index can be 0
	int right_size = right - mid;
	int left_size = mid - left + 1;
	
	row_data *RightArr= malloc(right_size*sizeof(row_data));
	row_data *LeftArr= malloc(left_size*sizeof(row_data));
	//row_data RightArr[right_size];
	//row_data LeftArr[left_size];
	
	//Copy elements into temporary arrays
	int i;
	for(i=0; i<right_size; i++){
		RightArr[i] = orig[mid + i + 1];
	}
	for(i=0; i<left_size; i++){
		LeftArr[i] = orig[left + i];
	}
	
	//Merge arrays back into original array,
	int x = 0;
	int y = 0;
	int z = left;
	
	//Use strcmp function for str1, str2
	//Returns < 0 if str1 < str2
	//Returns = 0 if str1 = str2
	//Returns > 0 if str1 > str2
	while(x<left_size && y<right_size){
		int xloc = LeftArr[x].locations[col];
		int yloc = RightArr[y].locations[col];
		if(strcmp(LeftArr[x].strs[xloc], RightArr[y].strs[yloc])>0){
			orig[z] = RightArr[y];
			y = y+1;
		}else{
			orig[z] = LeftArr[x];
			x = x+1;
		}
		z = z+1;
	}
	
	//Make sure all remaining elements of Left or Right are added back
	if(y<right_size){
		while(y<right_size){
			orig[z] = RightArr[y];
			z++;
			y++;
		}
	}
	
	if(x<left_size){
		while(x<left_size){
			orig[z] = LeftArr[x];
			z++;
			x++;
		}
	}
}



//data type - int
void merge1(row_data orig[], header_data headers[], int left, int mid, int right, int col){
	//Create temporary arrays to copy elements from original array into.
	//Right Array is from middle + 1 to right index
	//Left Array is from left index to middle; add 1 to size because left index can be 0
	int right_size = right - mid;
	int left_size = mid - left + 1;
	
	row_data *RightArr= malloc(right_size*sizeof(row_data));
	row_data *LeftArr= malloc(left_size*sizeof(row_data));
	//row_data RightArr[right_size];
	//row_data LeftArr[left_size];
	
	//Copy elements into temporary arrays
	int i;
	for(i=0; i<right_size; i++){
		RightArr[i] = orig[mid + i + 1];
	}
	for(i=0; i<left_size; i++){
		LeftArr[i] = orig[left + i];
	}
	
	//Merge arrays back into original array,
	int x = 0;
	int y = 0;
	int z = left;
	
	//Use strcmp function for str1, str2
	//Returns < 0 if str1 < str2
	//Returns = 0 if str1 = str2
	//Returns > 0 if str1 > str2
	while(x<left_size && y<right_size){
		int xloc = LeftArr[x].locations[col];
		int yloc = RightArr[y].locations[col];
		if(LeftArr[x].ints[xloc] > RightArr[y].ints[yloc]){
			orig[z] = RightArr[y];
			y = y+1;
		}else{
			orig[z] = LeftArr[x];
			x = x+1;
		}
		z = z+1;
	}
	
	//Make sure all remaining elements of Left or Right are added back
	if(y<right_size){
		while(y<right_size){
			orig[z] = RightArr[y];
			z++;
			y++;
		}
	}
	
	if(x<left_size){
		while(x<left_size){
			orig[z] = LeftArr[x];
			z++;
			x++;
		}
	}
}





//data type - doubles
void merge2(row_data orig[], header_data headers[], int left, int mid, int right, int col){
	//Create temporary arrays to copy elements from original array into.
	//Right Array is from middle + 1 to right index
	//Left Array is from left index to middle; add 1 to size because left index can be 0
	int right_size = right - mid;
	int left_size = mid - left + 1;
	
	row_data *RightArr= malloc(right_size*sizeof(row_data));
	row_data *LeftArr= malloc(left_size*sizeof(row_data));
	//row_data RightArr[right_size];
	//row_data LeftArr[left_size];
	
	//Copy elements into temporary arrays
	int i;
	for(i=0; i<right_size; i++){
		RightArr[i] = orig[mid + i + 1];
	}
	for(i=0; i<left_size; i++){
		LeftArr[i] = orig[left + i];
	}
	
	//Merge arrays back into original array,
	int x = 0;
	int y = 0;
	int z = left;
	
	//Use strcmp function for str1, str2
	//Returns < 0 if str1 < str2
	//Returns = 0 if str1 = str2
	//Returns > 0 if str1 > str2
	while(x<left_size && y<right_size) {
		int xloc = LeftArr[x].locations[col];
		int yloc = RightArr[y].locations[col];
		if(LeftArr[x].doubles[xloc] > RightArr[y].doubles[yloc]){
			orig[z] = RightArr[y];
			y = y+1;
		}else{
			orig[z] = LeftArr[x];
			x = x+1;
		}
		z = z+1;
	}
	
	//Make sure all remaining elements of Left or Right are added back
	if(y<right_size){
		while(y<right_size){
			orig[z] = RightArr[y];
			z++;
			y++;
		}
	}
	
	if(x<left_size){
		while(x<left_size){
			orig[z] = LeftArr[x];
			z++;
			x++;
		}
	}
}
