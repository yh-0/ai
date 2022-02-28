#include <stdio.h>
#include <stdlib.h>
#define N 7

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

int arrayContains(int* array, int value)
{
	int i;
	for(i = 0; i < N; i++)
		if(array[i] == value) return 1;
	
	return 0;
}

int arrayMinIndex(int* array, int* visited)
{
	int i, min = 1000, minInd = -1;
	int* cpArray = array;
	
	for(i = 0; i < N; i++)
		if(arrayContains(visited, i)) cpArray[i] = 1000;
	
	for(i = 0; i < N; i++)
		if(cpArray[i] > 0 && cpArray[i] < min)
		{
			minInd = i;
			min = cpArray[i];
		}
	
	return minInd;
}

void tsp(struct Graph* graph)
{
	int i, curr = 0;
	int* visited = malloc(N * sizeof(int));
	for(i = 0; i < N; i++) visited[i] = 0;
	
	
	for(i = 0; i < N; i++)
	{
		visited[i] = curr;
		curr = arrayMinIndex(graph->adjList[curr], visited);
	}
	

	for(i = 0; i < N; i++) printf("%d ", visited[i]);
}

int main()
{
	struct Graph* graph = createGraph();
	
	addEdge(graph, 0, 1, 10);
	addEdge(graph, 0, 3, 4);
	addEdge(graph, 0, 4, 1);
	addEdge(graph, 1, 2, 1);
	addEdge(graph, 1, 4, 3);
	addEdge(graph, 1, 5, 15);
	addEdge(graph, 2, 3, 1);
	addEdge(graph, 2, 4, 2);
	addEdge(graph, 3, 4, 3);
	addEdge(graph, 3, 5, 7);
	addEdge(graph, 3, 6, 5);
	addEdge(graph, 5, 6, 6);
	
		
	printf("Graph (association matrix):\n");
	printGraph(graph);
	
	printf("\nPath starting from 0: ");
	tsp(graph);
}
