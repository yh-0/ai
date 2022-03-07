#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <random>
#define N 7

// Simulated Annealing Algorithm - Traveling Salesman Problem

using namespace std;

struct Graph
{
    int** adjList;
};

struct Graph* createGraph()
{
    // allocating graph and adjList
    struct Graph* graph = new Graph;
    graph->adjList = new int* [N];

    // allocating every array in adjList
    for (int i = 0; i < N; i++)
        graph->adjList[i] = new int [N];

    // filling adjList with 0s
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            graph->adjList[i][j] = 0;

    return graph;
}

void deallocateGraph(struct Graph* graph)
{
    for (int i = 0; i < N; i++)
        delete[] graph->adjList[i];
    delete[] graph->adjList;
}

// adding edges to a non-oriented graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    graph->adjList[src][dest] = weight;
    graph->adjList[dest][src] = weight;
}

void printArray(int* array)
{
    for (int i = 0; i < N; i++)
        cout << array[i] << " ";
    cout << endl;
}

void printGraph(struct Graph* graph)
{
    cout << "Graph:" << endl;
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            cout << graph->adjList[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

int pathLength(struct Graph* graph, int* array)
{
    // calculating length from index 0 to index N-1 (last element of array)
    int length = 0;
    for (int i = 0; i < N - 1; i++)
        length += graph->adjList[array[i]][array[i + 1]];

    // increasing length by weight between last and first elements of array
    length += graph->adjList[array[N - 1]][array[0]];

    return length;
}

// finding if array contains given value
int arrayContains(int* array, int value)
{
    int i;
    for (i = 0; i < N; i++)
        if (array[i] == value)
            return 1;

    return 0;
}

// generating random int in bounds
int randomInRange(int start, int end)
{
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> distr(start, end);

    int x = distr(rng);

    return x;
}

int* generateInitial()
{
    int* visited = new int[N];
    int i = 0, random;

    while (i < N)
    {
        random = randomInRange(0, N - 1);

        // if visited doesn't contain random, add random to visited
        if (!arrayContains(visited, random))
        {
            visited[i] = random;
            i++;
        }
    }

    cout << endl;
    
    return visited;
}

void swapRandom(int* array)
{
    int rand1, rand2, tmp;
    
    rand1 = randomInRange(0, N - 1);
    rand2 = randomInRange(0, N - 1);

    while (rand1 == rand2)
        rand2 = randomInRange(0, N - 1);

    tmp = array[rand1];
    array[rand1] = array[rand2];
    array[rand2] = tmp;
}

int* arraySwapCheck(struct Graph* graph, int* array, double t0)
{
    int len1, len2, dS, random;
    double P;

    // allocating cpArray, copying values from array
    int* cpArray = new int [N];
    for (int i = 0; i < N; i++)
        cpArray[i] = array[i];

    // swapping random 2 elements in cpArray
    swapRandom(cpArray);

    // checking if length of cpArray is less than length of array, if so returning cpArray
    len1 = pathLength(graph, array);
    len2 = pathLength(graph, cpArray);

    if (len2 < len1) return cpArray;

    // checking probability of accepting worse solution (higher value of length)
    dS = len2 - len1;
    P = 100 * exp(-1 * dS / t0);
    random = randomInRange(0, 100);

    // if probability is higher than random value between 0 and 100, accept worse solution
    if (P > random) return cpArray;

    delete[] cpArray;
    return array;
}

int* wyzarzanie(struct Graph* graph, int* solution, int epoki, int proby, double n, double t0)
{
    // repeat arraySwapCheck for given amount of epochs and tries in each epoch
    for (int i = 0; i < epoki; i++)
    {
        for (int j = 0; j < proby; j++)
            solution = arraySwapCheck(graph, solution, t0);

        // change t0 at the end of every epoch
        t0 *= n;
    }

    return solution;
}

int main()
{
    struct Graph* graph = createGraph();

    addEdge(graph, 0, 1, 15);
    addEdge(graph, 0, 2, 12);
    addEdge(graph, 0, 3, 6);
    addEdge(graph, 0, 4, 21);
    addEdge(graph, 0, 5, 3);
    addEdge(graph, 0, 6, 4);
    addEdge(graph, 1, 2, 7);
    addEdge(graph, 1, 3, 10);
    addEdge(graph, 1, 4, 14);
    addEdge(graph, 1, 5, 12);
    addEdge(graph, 1, 6, 16);
    addEdge(graph, 2, 3, 1);
    addEdge(graph, 2, 4, 11);
    addEdge(graph, 2, 5, 25);
    addEdge(graph, 2, 6, 8);
    addEdge(graph, 3, 4, 19);
    addEdge(graph, 3, 5, 23);
    addEdge(graph, 3, 6, 13);
    addEdge(graph, 4, 5, 9);
    addEdge(graph, 4, 6, 7);
    addEdge(graph, 5, 6, 11);

    printGraph(graph);

    int* solution = generateInitial();

    cout << "Initial solution: ";
    printArray(solution);
    cout << "Length: " << pathLength(graph, solution) << endl << endl;

    solution = wyzarzanie(graph, solution, 5, 3, 0.5, 100);

    cout << "Final solution: ";
    printArray(solution);
    cout << "Length: " << pathLength(graph, solution) << endl;

    deallocateGraph(graph);
    delete[] solution;
}
