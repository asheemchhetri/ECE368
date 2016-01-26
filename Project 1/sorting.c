#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

/**
* Function that takes Filename and Size as parameters and loads a file
* data read from the file is stored into an array that is then returned.
*/
long *Load_File(char *Filename, int *Size)
{
  FILE *fptr = NULL;
  fptr = fopen(Filename, "r");
  if (fptr == NULL) return NULL;
  int N; 
  int i;
  int temp;
  long * array;
  
  fscanf(fptr, "%d", &N);
  array = malloc(sizeof(long) * N);
  
  for (i = 0; i < N; i++)
  {
  	if(feof(fptr))
	{
		fprintf(stderr, "Empty file encountered, closing file!\n");
		free(array);
		*Size = i;
		fclose(fptr);
		return NULL;
	}
    fscanf(fptr, "%d", &temp);
    array[i] = temp;
  }
  
  *Size = N;
  fclose(fptr);
  return array;
}

/**
* Function that takes a Filename, size and an Array and
* Writes all elements of the array into the File following
* Format specifications given (Size followed by array)
* function returns the number of elements written to file.
*/
int Save_File(char *Filename, long *Array, int Size)
{
	FILE *fptr = fopen(Filename, "w");			
	int numbers_written = 0;
	if(fptr==NULL) return ERROR;			
	fprintf(fptr, "%d\n", Size);			
	int i;
	for(i = 0; i < Size; ++i)
	{
		fprintf(fptr, "%ld\n", Array[i]);
		++numbers_written;
	}
	fclose(fptr);				 
	return numbers_written;
}

/**
* Function to generate sequence 1 for a Size N and save
* The obtained sequence into a file with N on the first
* line followed by the gap sizes.
*/
void Save_Seq1 (char *Filename, int N)
{
  int i;
  int j = 0;
  
  int k = 1;
  int p = 0;
  while(k < N)
  {
    k = k * 3;
    p = p + 1;
  }
  k = k / 3;
  p = p - 1;
  int counter = 0;
  int Array[counter];
  
  while(p >= 0)
  {
    int gap = k;
    int seq_count = p;
    do {
		Array[counter] = gap;
		gap = (gap / 3) * 2;
		seq_count = seq_count - 1;
		counter++;
     }while (seq_count >= 0);
     k = k / 3;
     p = p - 1;
  }
  
  FILE * fptr = NULL;
  fptr = fopen(Filename, "w");
  if(fptr == NULL) return;
  while (Array[j + 1])
  {
    j++;
  }
  fprintf(fptr, "%d\n", j);
  i = 0;
  while (i < j)
  {
  	fprintf(fptr, "%d\n", j);
    i++;
  }
  fclose(fptr);
  free(Array);
}

/**
* Function to generate sequence 1 for a Size N and save
* The obtained sequence into a file with N on the first
* line followed by the gap sizes.
*/
void Save_Seq2 (char *Filename, int N)
{
  int counter = 0;
  int integer = N;
  int gap[counter];
  
  while(N >= 1)
  {
  	gap[counter] = integer / 1.3;
    counter++;
    if (integer == 9 || integer == 10) integer = 11;
  }
  
  FILE * fptr = NULL;
  fptr = fopen(Filename, "w");
  if(fptr == NULL) return;
  
  int j = 0;
  while (gap[j + 1])
  {
	j++;
  }
  fprintf(fptr, "%d\n", j);
  int i = 0;
  while (i < j)
  {
    fprintf(fptr, "%d\n", j);
    i++;
  }
  fclose(fptr);
  free(gap);
}

/**
* Function to shell sort given Array. It takes Size of the array as a parameter
* *N_Comp and *N_Move are parameters that help count no of comp/moves respectively
* for analysis purposes.
*/
void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
  int k = 1;
  int p = 0;
  int Comp = 0;
  int Move = 0;
  while (k < Size)
    {
      k = k * 3;
      p = p + 1;
    }
  k = k / 3;
  p = p - 1;
  while (p >= 0)
    {
      int gap = k;
      int seq_count = p;
      do {
	int j;
	for(j = gap; j <= Size - 1; j++)
	  {
	    long temp_r = Array[j];
	    int i = j;
	    while (i >= gap && Array[i - gap] > temp_r)
	      {
		Array[i] = Array[i - gap];
		Move++;
		i = i - gap;
	      }
	    Array[i] = temp_r;
	    Comp++;
	  }
	gap = (gap / 3) * 2;
	seq_count = seq_count - 1;
      } while (seq_count >= 0);
      k = k / 3;
      p = p - 1;
    }
  * N_Move = Move;
  * N_Comp = Comp;
}

/**
* Function to bubble sort given Array using modified algorithm.
* It takes Size of the array as a parameter and *N_Comp and *N_Move
* As other parameters that help count no of comp/moves respectively
* for analysis purposes.
*/
void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
  int i, j, gap, swapped = 1;
  double temp;
  gap = Size;
  while (gap > 1 || swapped == 1)
  {
	gap = gap * 10 / 13;
	if (gap == 9 || gap == 10) gap = 11;
    if (gap < 1) gap = 1;
    swapped = 0;
    for(i = 0, j = gap; j < Size; i++, j++)
    {
    	*N_Comp = *N_Comp + 1.0;
    	if(Array[i] > Array[j])
      	{
        	*N_Move = *N_Move + 3.0;
        	temp = Array[i];
        	Array[i] = Array[j];
        	Array[j] = temp;
        	swapped = 1;
      	}
    }
  }
}