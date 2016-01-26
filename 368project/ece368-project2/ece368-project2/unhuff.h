#include <stdio.h>
#include <stdlib.h>

typedef struct treenode {
  int character;
  int weight;
  struct treenode * left;
  struct treenode * right;
} TreeNode;

typedef struct listnode {
  struct listnode * next;
  TreeNode * tree_ptr;
} ListNode;

typedef struct freqList
{
  int freq;
  int char_val;
  struct freqList *next;
} Node;

TreeNode * readHeader(FILE * infptr);

ListNode * MergeListNode(ListNode * head, int mode);
ListNode * ListNode_create(TreeNode * tn);
ListNode * ListNode_insert(ListNode * head, ListNode * wn, int mode);

void Tree_destroy(TreeNode * node);
TreeNode * TreeNode_create(int char_val, int weight_val);
TreeNode * Tree_merge(TreeNode * tn1, TreeNode * tn2);

int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte);
int decode(char * infile, char * outfile);
void Tree_print(TreeNode * tn, int level);
