#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"
#define QUEUE 0
#define STACK 1
#define SORTED 2
#define ENCODER 0
#define DECODER 1

//Function reads header from compressed file to recreate the tree used for compression/decompression
TreeNode * readHeader(FILE * infptr)
{
  int done = 0;
  unsigned char whichbit = 0;
  unsigned char curbyte = 0;
  unsigned char onebit = 0;
  ListNode * head = NULL;
  while(done == 0)
  {
    readBit(infptr, &onebit, &whichbit, &curbyte);
    if(onebit == 1)
  	{
  	  int bitcount;
  	  unsigned char value = 0;
  	  for(bitcount = 0; bitcount < 7; bitcount++)
      {
        value <<= 1; 
        readBit(infptr, &onebit, &whichbit, &curbyte);
        value |= onebit;
      }
  	  TreeNode * tn = TreeNode_create(value, 0);
  	  ListNode * wn = ListNode_create(tn);
  	  head = ListNode_insert(head, wn, STACK);
  	}
    else
  	{
  	  if (head == NULL) printf("Error");
  	  if((head -> next) == NULL) done = 1;
  	  else head = MergeListNode(head, DECODER);
  	}
  }
  TreeNode * root = head -> tree_ptr;
  free(head);
  return(root);
}

//Decode function takes the tree and decodes bits with accordance to tree values. 
int decode(char * infile, char * outfile)
{
  FILE * infptr = fopen(infile, "r");
  if (infptr == NULL)
  {
    printf("Error reading file!");
    return 0;
  }
  TreeNode * root = readHeader(infptr);
  // Tree_print(root, 0);
  unsigned int numChar = 0;
  fread(&numChar, sizeof(unsigned int), 1, infptr);
  // printf("numChar = %d\n", numChar);
  if(numChar < 0)
  {
    printf("numChar is less than 0.\n");
    return(0);
  }
  unsigned char newline;
  fread(&newline, sizeof(unsigned char), 1, infptr);
  if(newline != '\n')
  {
    printf("ERROR! Newline\n");
    return(0);
  }
  unsigned char whichbit = 0;
  unsigned char onebit = 0;
  unsigned char curbyte = 0;
  FILE * outfptr = fopen(outfile, "w");
  // printf("test\n");
  while(numChar != 0)
  {
    TreeNode * tn = root;
    while ((tn -> left) != NULL)
  	{
  	  readBit(infptr, &onebit, &whichbit, &curbyte);
  	  if(onebit == 0) tn = tn -> left;
  	  else tn = tn -> right;
  	}
    // printf("%c", tn -> character);
    fprintf(outfptr, "%c", tn -> character);
    numChar--;
  }
    Tree_destroy(root);
    fclose(infptr);
    fclose(outfptr);
    return(0);
}

//Create's list node 
ListNode * ListNode_create(TreeNode * tn)
{
  ListNode * wn = malloc(sizeof(ListNode));
  wn -> next = NULL;
  wn -> tree_ptr = tn;
  return wn;
}

//Inserts list nodes into lists
ListNode * ListNode_insert(ListNode * head, ListNode * wn, int mode)
{
  if (wn == NULL)
  {
    printf("Error\n");
    return NULL;
  }
  if ((wn -> next) != NULL) printf("Error\n");
  if(head == NULL) return wn;
  if (mode == STACK)
  {
    wn -> next = head;
    return wn;
  }
  if (mode == QUEUE)
  {
    head -> next = ListNode_insert(head -> next, wn, mode);
    return head;
  }
  int weight1 = (head -> tree_ptr) -> weight;
  int weight2 = (wn -> tree_ptr) -> weight;
  if (weight1 > weight2)
  {
    wn -> next = head;
    return wn;
  }
  head -> next = ListNode_insert(head -> next, wn, mode);
  return head;
}


//Destroys tree
void Tree_destroy(TreeNode * node)
{
  if(node == NULL) return;
  Tree_destroy(node -> left);
  Tree_destroy(node -> right);
  free(node);
}

//Creates individual tree nodes
TreeNode * TreeNode_create(int char_val, int weight_val)
{
  TreeNode * node = malloc(sizeof(TreeNode));
  node -> character = char_val;
  node -> weight = weight_val;
  node -> left = NULL;
  node -> right = NULL;
  return node;
}

//Merges two treenodes together to form a tree whose parent value is the sum of the frequencies of the two nodes
TreeNode * Tree_merge(TreeNode * tn1, TreeNode * tn2)
{
  TreeNode * tn = malloc(sizeof(TreeNode));
  tn -> left = tn1;
  tn -> right = tn2;
  tn -> character = 0;
  tn -> weight = (tn1 -> weight) + (tn2 -> weight);
  return tn;
}

//Merges two lists nodes together into 1 list
ListNode * MergeListNode(ListNode * head, int endec)
{
  ListNode * second = head -> next;
  ListNode * third = second -> next;
  TreeNode * tn1 = head -> tree_ptr;
  TreeNode * tn2 = second -> tree_ptr;
  free(head);
  free(second);
  head = third;
  TreeNode * fin;
  if (endec == ENCODER) fin = Tree_merge(tn1, tn2);
  else fin = Tree_merge(tn2, tn1);
  ListNode * wn = ListNode_create(fin);
  if (endec == ENCODER) head = ListNode_insert(head, wn, SORTED);
  else head = ListNode_insert(head, wn, STACK);
  return head;
}

//Reads 1 bit from file
int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte)
{
  int ret = 1;
  if((*whichbit) == 0) ret = fread(curbyte, sizeof(unsigned char), 1, fptr);
  if(ret != 1) return -1;
  unsigned char temp = (*curbyte) >> (7 - (*whichbit));
  temp = temp & 0X01;
  *whichbit = ((*whichbit) + 1) % 8;
  *bit = temp;
  return 0;
}

//Prints whole tree
void Tree_print(TreeNode * tn, int level)
{
  if(tn == NULL) return;
  TreeNode * lc = tn -> left;
  TreeNode * rc = tn -> right;

  Tree_print(lc, level + 1);
  Tree_print(rc, level + 1);
  int depth ;
  for ( depth = 0; depth < level; depth ++) printf (" ");
  printf (" freq = %d" , tn -> weight);
  if ((lc == NULL) && (rc == NULL)) printf (" value = %d , %c", tn -> character, tn -> character);
  printf ("\n");
}

//Run everything
int main(int argc, char **argv)
{
  if(argc < 2)
    {
      printf("Error, no input given.\n");
      return(0);
    }

  char * filename = argv[1];
  int filename_size = strlen(filename);

  char * outfile = (char *)malloc(filename_size+5 * sizeof(char));

  strcpy(outfile, filename);
  outfile = strcat(outfile, ".unhuff");

  decode(filename, outfile);
  return(0);
}
