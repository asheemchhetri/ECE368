#include <stdio.h>
#include <stdlib.h>
#include <math.h>   // need -lm flag while compiling

struct intList
{
    int value;
    struct intList * next;
};

typedef struct Vertex
{
    int ID; 
    int xAxis, yAxis;
    struct intList *adjList;
    int predecessor;
    float distance;

}Vertice;

void printPath(Vertice *vertex, int *vertex_address, int destination);
void Vertice_Free(Vertice *vertex, int No_Vertices);
int populate(FILE *fmap, Vertice *vertex, int *vertex_address, int No_Vertices, int No_Edges);
void Djikstra_shortest_path(int source, int destination, Vertice *vertex, int *vertex_address, int No_Vertices);
void upward_heapify(Vertice *vertex, int *vertex_address, int loc);
void downward_heapify(Vertice *vertex ,int *vertex_address , int loc , int size_Minheap);
int isCompleted(int No_Vertices, int size_visited, Vertice *vertex, int id);
void swap_Vertex(int loc1, int loc2, Vertice *vertex);
void swap_vertexAddress(int loc1, int loc2, int *vertex_address);
void reset_values(Vertice *vertex, int No_Vertices);


int populate(FILE *fmap, Vertice *vertex, int *vertex_address, int No_Vertices, int No_Edges)
{
    if(vertex == NULL || vertex_address == NULL || No_Vertices <= 0)    return 0;
    int i = 0, src, dest;   // src and dest are for edge locations
    for(i = 0; i < No_Vertices; ++i)    // run until we read all vertice data
    {
        vertex_address[i] = i;  // initially ith value points to i index in "vertex"
        fscanf(fmap, "%d %d %d", &(vertex[i].ID), &(vertex[i].xAxis), &(vertex[i].yAxis) );
        vertex[i].adjList = NULL;
        vertex[i].distance -=1;
        vertex[i].predecessor -=1;
    }
    for(i = 0; i < No_Edges; ++i)   // then run until all edges have been added
    {
        fscanf(fmap, "%d %d", &src, &dest);
        struct intList *tempValA = malloc(sizeof(struct intList) * 1);
        if(tempValA == NULL)    return 0;
        tempValA->value = dest;                // set value
        tempValA->next = vertex[src].adjList;  // make it point to top of list
        vertex[src].adjList = tempValA;        // reset list top to tempValA
        struct intList *tempValB = malloc(sizeof(struct intList) * 1);
        if(tempValB == NULL)    return 0;
        tempValB->value = src;                 // set value
        tempValB->next = vertex[dest].adjList; // make it point to top of list
        vertex[dest].adjList = tempValB;       // reset list top to tempValB
    }
    return 1;
}

void Djikstra_shortest_path(int source, int destination, Vertice *vertex, int *vertex_address, int No_Vertices)
{
    int size_Minheap = No_Vertices;
    int size_visited = 0;
    vertex[vertex_address[source]].distance  = 0.0;
    swap_Vertex(0, vertex_address[source], vertex);
    swap_vertexAddress(vertex[vertex_address[source]].ID, source, vertex_address);

    while(size_Minheap > 0)   // runs Vertice Number of times
    {
        if(vertex[0].ID == destination) return;

        Vertice popped = vertex[0];
        swap_Vertex(0, size_Minheap - 1 , vertex);
        swap_vertexAddress(vertex[0].ID, vertex[size_Minheap-1].ID, vertex_address);
        size_Minheap -= 1; 
        size_visited += 1; 
        downward_heapify(vertex , vertex_address , 0 , size_Minheap);
        struct intList *p;
        p = popped.adjList;
        while(p != NULL)
        {
            if(isCompleted(No_Vertices , size_visited, vertex, p->value))
            {
                p = p->next;
                continue;
            }
            float DIST = sqrt((vertex[vertex_address[p->value]].xAxis-popped.xAxis)*(vertex[vertex_address[p->value]].xAxis-popped.xAxis)
                          + (vertex[vertex_address[p->value]].yAxis-popped.yAxis)*(vertex[vertex_address[p->value]].yAxis-popped.yAxis));
            if( vertex[vertex_address[p->value]].distance != -1 && vertex[vertex_address[p->value]].distance <= popped.distance + DIST )
            {   
                p = p->next;
                continue;
            }
            vertex[vertex_address[p->value]].distance = popped.distance + DIST;
            vertex[vertex_address[p->value]].predecessor = popped.ID;
            upward_heapify(vertex, vertex_address, vertex_address[p->value]);
            p = p->next;
        }
    }
}

void upward_heapify(Vertice *vertex, int *vertex_address, int loc)
{
    while(loc > 0)
    {
        if(vertex[loc].distance == -1)  return;
        if(vertex[(loc - 1)/2].distance != -1 && vertex[loc].distance >= vertex[(loc - 1)/2].distance)  return;
        swap_Vertex(loc, (loc-1)/2 , vertex);
        swap_vertexAddress(vertex[loc].ID, vertex[(loc-1)/2].ID, vertex_address);
        loc = (loc-1)/2;
    }
}

void downward_heapify(Vertice *vertex ,int *vertex_address , int loc , int size_Minheap)
{
    while(loc < size_Minheap)
    {
        if(loc*2 + 1 >= size_Minheap)   return;
        if(loc*2 + 2 >= size_Minheap)
        {                           
            if(vertex[loc*2+1].distance == -1)  return;
            if(vertex[loc].distance == -1 || vertex[loc].distance > vertex[loc*2 + 1].distance )
            {
                swap_Vertex(loc, loc*2 + 1 , vertex);
                swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 1].ID, vertex_address);
            }
            return;
        }

        if(vertex[(loc*2 + 1)].distance == -1 && vertex[(loc*2 + 2)].distance == -1)    return;

        if(vertex[loc].distance == -1)
        {
            if(vertex[loc*2 + 1].distance == -1)
            {
                swap_Vertex(loc, loc*2 + 2 , vertex);
                swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 2].ID, vertex_address);
                loc = loc*2 + 2;
                continue;
            }
            else if(vertex[loc*2 + 2].distance == -1)
            {
                swap_Vertex(loc, loc*2 + 1 , vertex);
                swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 1].ID, vertex_address);
                loc = loc*2 + 1;
                continue;
            }
            else if(vertex[loc*2 + 2].distance != -1 && vertex[loc*2 + 2].distance != -1)
            {
                if(vertex[loc*2 + 1].distance <= vertex[loc*2 + 2].distance)
                {
                    swap_Vertex(loc, loc*2 + 1 , vertex);
                    swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 1].ID, vertex_address);
                    loc = loc*2 + 1;
                    continue;
                }
                else
                {
                    swap_Vertex(loc, loc*2 + 2 , vertex);
                    swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 2].ID, vertex_address);
                    loc = loc*2 + 2;
                    continue;
                }
            }
        }
        else
        {
            if(vertex[loc*2 + 1].distance == -1)
            {                         
                if(vertex[loc].distance > vertex[loc*2 + 2].distance)
                {
                    swap_Vertex(loc, loc*2 + 2 , vertex);
                    swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 2].ID, vertex_address);
                    loc = loc*2 + 2;
                    continue;
                }
                else    return;
            }
            else if(vertex[loc*2 + 2].distance == -1)
            {
                if(vertex[loc].distance > vertex[loc*2 + 1].distance)
                {
                    swap_Vertex(loc, loc*2 + 1 , vertex);
                    swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 1].ID, vertex_address);
                    loc = loc*2 + 1;
                    continue;
                }
                else    return;
            }
            else if(vertex[loc*2 + 2].distance != -1 && vertex[loc*2 + 2].distance != -1)
            {
                if( vertex[loc].distance <= vertex[loc*2 + 1].distance && vertex[loc].distance <= vertex[loc*2 + 2].distance )  return;

                else if(vertex[loc*2 + 1].distance <= vertex[loc*2 + 2].distance  && vertex[loc].distance > vertex[loc*2 + 1].distance)
                {
                    swap_Vertex(loc, loc*2 + 1 , vertex);
                    swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 1].ID, vertex_address);
                    loc = loc*2 + 1;
                    continue;
                }
                else if(vertex[loc*2 + 1].distance > vertex[loc*2 + 2].distance  && vertex[loc].distance > vertex[loc*2 + 2].distance)
                {
                    swap_Vertex(loc, loc*2 + 2 , vertex);
                    swap_vertexAddress(vertex[loc].ID, vertex[loc*2 + 2].ID, vertex_address);
                    loc = loc*2 + 2;
                    continue;
                }
            }
        }
    }
}

int isCompleted(int No_Vertices, int size_visited, Vertice *vertex, int id)
{
    int first = No_Vertices - 1;
    int last = No_Vertices - size_visited;
    int mid = (first + last)/2;
    while(last <= first)
    {
        mid = (first + last)/2;
        if(vertex[mid].ID == id)    return 1;
        else if(vertex[mid].ID > id)    first = mid - 1;
        else    last = mid + 1;
    }
    return 0;
}


void swap_Vertex(int loc1, int loc2, Vertice *vertex)
{
    // basic swapping function
    Vertice temp;
    temp.ID = vertex[loc1].ID;
    temp.xAxis = vertex[loc1].xAxis;
    temp.yAxis = vertex[loc1].yAxis;
    temp.predecessor = vertex[loc1].predecessor;
    temp.distance = vertex[loc1].distance;
    temp.adjList = vertex[loc1].adjList;

    vertex[loc1].ID = vertex[loc2].ID;
    vertex[loc1].xAxis = vertex[loc2].xAxis;
    vertex[loc1].yAxis = vertex[loc2].yAxis;
    vertex[loc1].predecessor = vertex[loc2].predecessor;
    vertex[loc1].distance = vertex[loc2].distance;
    vertex[loc1].adjList = vertex[loc2].adjList;

    vertex[loc2].ID = temp.ID;
    vertex[loc2].xAxis = temp.xAxis;
    vertex[loc2].yAxis = temp.yAxis;
    vertex[loc2].predecessor = temp.predecessor;
    vertex[loc2].distance = temp.distance;
    vertex[loc2].adjList = temp.adjList;
}

void swap_vertexAddress(int loc1, int loc2, int *vertex_address)
{
    // basic swapping function
    int temp = vertex_address[loc1];
    vertex_address[loc1] = vertex_address[loc2];
    vertex_address[loc2] = temp;
}

void reset_values(Vertice *vertex, int No_Vertices)
{
    int i;
    for(i = 0; i < No_Vertices; ++i)    // run until we read all vertice data
    {
        vertex[i].distance = -1;
        vertex[i].predecessor = -1;
    }
}

void printPath(Vertice *vertex, int *vertex_address, int destination)
{
    if(vertex[vertex_address[destination]].predecessor == -1)
    {
        printf("%d ", vertex[vertex_address[destination]].ID);
        return;
    }
    printPath(vertex, vertex_address, vertex[vertex_address[destination]].predecessor);
    printf("%d ", vertex[vertex_address[destination]].ID);
}

void Vertice_Free(Vertice *vertex, int No_Vertices)
{
    if(vertex == NULL)  return;
    int i = 0;
    struct intList *p, *q;
    for(i = 0; i < No_Vertices; ++i) 
    {                               
        p = vertex[i].adjList;
        while(p != NULL)
        {
            q = p;
            p = p->next;
            free(q);
        }
    }
    free(vertex);   // array vertex is freed.
}

int main(int argc, char **argv)
{
    if(argc != 3)// error
    {
        fprintf(stderr, "Function call must be of type:\n\
        ./shortestpath map.txt query.txt\n");
        return EXIT_FAILURE;
    }

    FILE *fmap;
    fmap = fopen(argv[1], "r");
    if(fmap == NULL)// error
    {
        fprintf(stderr, "map file (%s) could not be opened\n", argv[1]);
        return EXIT_FAILURE;
    }

    FILE *fquery = fopen(argv[2], "r");
    if(fquery == NULL)// error
    {
        fprintf(stderr, "query file (%s) could not be opened\n", argv[2]);
        fclose(fmap);
        return EXIT_FAILURE;
    }

    /**
        FILE format:
            maps:
                First Line: # of Vertices # of Edges
                Vertices Lines: Vertice # x y
                            <Seems to be in ascending order>
                Edge Lines: starting_vertice ending_vertice
            query:
                First Line: # of Queries
                Query Lines: Source_Vertx Destination_Vertx
    */

    int No_Vertices = -1, No_Edges = -1, No_Queries = -1;
    fscanf(fmap, "%d %d", &No_Vertices, &No_Edges);
    if(No_Vertices == -1 || No_Edges == -1)
    {
        fprintf(stderr, "Error. Did not read (%s) header properly.", argv[1]);
        fclose(fmap);
        fclose(fquery);
        return EXIT_FAILURE;
    }

    Vertice *vertex = malloc(sizeof(Vertice)*No_Vertices);   // this stores graph data and has minheap
    int *vertex_address = malloc(sizeof(int)*No_Vertices);  // this is used to maintain initial addresses

    if( !populate(fmap, vertex, vertex_address, No_Vertices, No_Edges) )
    {
        fprintf(stderr, "Could Not successfully popluate arrays.\n");
        fclose(fmap);
        fclose(fquery);
        Vertice_Free(vertex, No_Vertices);
        free(vertex_address);
        return EXIT_FAILURE;
    }
    fclose(fmap);

    fscanf(fquery, "%d", &No_Queries);
    if(No_Queries == -1)
    {
        fprintf(stderr, "Error. Did not read (%s) header properly.", argv[2]);
        fclose(fquery);
        Vertice_Free(vertex, No_Vertices);  // custom free function to free list then array
        free(vertex_address);
        return EXIT_FAILURE;
    }
    int source, destination;
    while(No_Queries-- > 0) // loop until all queries are read
    {
        fscanf(fquery, "%d %d", &source, &destination);

        Djikstra_shortest_path(source, destination, vertex, vertex_address, No_Vertices);
        printf("%.0f\n", vertex[vertex_address[destination]].distance);
        printPath(vertex, vertex_address, destination);
        printf("\n");
        reset_values(vertex, No_Vertices);
    }

    fclose(fquery);
    Vertice_Free(vertex, No_Vertices);
    free(vertex_address);   // free only after allocation
    return EXIT_SUCCESS;
}
