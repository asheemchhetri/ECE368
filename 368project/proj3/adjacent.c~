#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVERTICES 10000
#define MAXEDGES    100000

typedef struct vertex_t {
  int x;
  int y;
  int node;
} * vertex;

typedef struct edge_t {
  int start;
  int end;
} * edge;

int ** Load_File (char * Filename, int * vertices, int * edges)
{
  FILE * fptr = NULL;
  fptr = fopen(Filename, "r");
  if (fptr == NULL)
  {
    return NULL;
  }
  vertex start;
  fscanf(fptr, "%d %d", vertices, edges);
  int ** edge;
  int i;
  edge = malloc((*edges) * sizeof(int*));
  for(i = 0; i <= *edges; i++)  edge[i] = malloc(2 * sizeof(int));

  start = malloc(sizeof(vertex) ** vertices);
  for(i = 0; i < *vertices; i++)  fscanf(fptr, "%d %d %d", &(start -> node), &(start -> x), &(start -> y));

  for(i = 0; i < *edges; i++) fscanf(fptr, "%d %d", &edge[i][0], &edge[i][1]);

  return edge;
}

int main(int argc, char ** argv)
{
  if(argc != 2) return EXIT_FAILURE;
  int ** edge;
  int vertices;
  int edges;
  edge = Load_File(argv[1], &vertices, &edges);
  // printf("%d\n", edges);
  int i;
  int j = 0;
  for(i = 0; i < vertices; i++)
  {
    printf("%d:", i);
    for(j = 0; j < edges; j++)
    {
      //edge[j][0] corresponds to column 1 of the edges(vertices pair).
      //edge[j][1] corresponds to column 2 of the edges(vertices pair).
      if(i == edge[j][0])	printf(" %d", edge[j][1]);
      if(i == edge[j][1])       printf(" %d", edge[j][0]);
    }
    printf("\n");
  }
  return 0;
}
