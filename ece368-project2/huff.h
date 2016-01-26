#include <stdio.h>
#include <stdlib.h>

typedef struct freqList
{
  int freq;
  int char_val;
  struct freqList * next;
} Node;

typedef struct treenode{
  int character;
  int weight;
  struct treenode * left;
  struct treenode * right;
} TreeNode;

typedef struct listnode{
  struct listnode * next;
  TreeNode * tree_ptr;
} ListNode;

void frequency_count(char *Filename, int * frequency);
Node * List_create(int value, int index);
Node * List_build(int * frequencies);
Node * List_insert_ascend(Node * head, int value, int index);
void List_destroy(Node * head);

ListNode * ListNode_create(TreeNode * tn);
ListNode * ListNode_insert(ListNode * head, ListNode * wn);
ListNode * ListNode_build(Node * freqList);

void Tree_print ( TreeNode * tn , int level );
void Weight_print(ListNode * head);

void Tree_destroy(TreeNode * node);
TreeNode * TreeNode_create(int char_val, int weight_val);
TreeNode * Tree_merge(TreeNode * tn1, TreeNode * tn2);

int writeBit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curbyte);
void char2bits(FILE * outfptr, int ch, unsigned char * whichbit, unsigned char * curbyte);
void Tree_headerHelper(TreeNode * tn, FILE * outfptr, unsigned char * whichbit, unsigned char * curbyte);
void Tree_header(TreeNode * tn, unsigned int numChar, char * Filename);
int compress(char * infile, char * outfile, int ** codebook, int * mapping);
int padZero(FILE * fptr, unsigned char * whichbit, unsigned char * curbyte);

int Tree_heightHelper(TreeNode * tn, int height);
int Tree_height(TreeNode * tn);
void Tree_leafHelper(TreeNode * tn, int * num);
int Tree_leaf(TreeNode * tn);
void buildCodeBookHelper(TreeNode * tn, int ** codebook, int * row, int col);
void buildCodeBook(TreeNode * root, int ** codebook);
void printCodeBook(int ** codebook, int numRow);
