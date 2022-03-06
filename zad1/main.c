#include <stdio.h>
#include <stdlib.h>
#define N 5

// Traveling Salesman Problem

// Traveling Salesman Problem using nearest neighbour algorithm

struct Graph
{
	int** adjList;
};

struct Graph* createGraph()
{
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph->adjList = malloc(N * sizeof(int*));
	
	int i, j;
	for(i = 0; i < N; i++)
		graph->adjList[i] = malloc(N * sizeof(int));
	
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			graph->adjList[i][j] = 0;
	
	return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight)
{
	graph->adjList[src][dest] = weight;
	graph->adjList[dest][src] = weight;
}

void printGraph(struct Graph* graph)
{
	int i, j;
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
			printf("%d ", graph->adjList[i][j]);
		
		printf("\n");
	}
}

// helper function to check if array contains given value
int arrayContains(int* array, int value)
{
	int i;
	for(i = 0; i < N; i++)
		if(array[i] == value) 
			return 1;
		
	return 0;
}

// helper function to find lowest nonvisited value in array
int arrayMinIndex(int* array, int* visited)
{
	int i, min = 1000, minInd = -1;
	int* cpArray = malloc(N * sizeof(int*));
	
	for(i = 0; i < N; i++) cpArray[i] = array[i];
	
	for(i = 0; i < N; i++)
		if(arrayContains(visited, i)) 
			cpArray[i] = 1000;
	
	for(i = 0; i < N; i++)
		if(cpArray[i] > 0 && cpArray[i] < min)
		{
			minInd = i;
			min = cpArray[i];
		}
	
	free(cpArray);
	return minInd;
}

void tsp(struct Graph* graph, int start)
{
	printf("Path starting from %d: ", start);
	
	int i, curr, tmp, length = 0;
	int* visited = malloc(N * sizeof(int));
	for(i = 0; i < N; i++) visited[i] = -1;
	
	curr = start;
	
	for(i = 0; i < N; i++)
	{
		visited[i] = curr;
		tmp = arrayMinIndex(graph->adjList[curr], visited);
		if(i < N - 1)
		{
			length += graph->adjList[curr][tmp];
			curr = tmp;
		}
	}
	
	length += graph->adjList[curr][start];

	for(i = 0; i < N; i++) printf("%d ", visited[i]);
	printf("%d ; length = %d\n", start, length);
	
	free(visited);
}

int main()
{
	struct Graph* graph = createGraph();
	
	addEdge(graph,0,1,15);
	addEdge(graph,0,2,12);
	addEdge(graph,0,3,6);
	addEdge(graph,0,4,21);
	addEdge(graph,1,2,3);
	addEdge(graph,1,3,10);
	addEdge(graph,1,4,4);
	addEdge(graph,2,3,11);
	addEdge(graph,2,4,26);
	addEdge(graph,3,4,17);
		
	printf("Graph:\n");
	printGraph(graph);

	int i;
	for(i = 0; i < N; i++) tsp(graph, i);
}
