#include <iostream>
#include <vector>
#include <tuple>
#include <math.h>
#include <random>

struct Node;

struct Edge
{
    Node* src;
    Node* dest;
    int weight;
	double pher;
};

struct Node
{
	int index;
	std::vector<Edge*> edges;
};

struct Ant
{
	int index;
	Node* currNode;
	std::vector<Node*> visitedNodes;
	std::vector<Node*> nvn;
	std::vector<std::tuple<Node*, double>> moveProbs;
	int pathLength;
};

void popCurrentNode(Ant* ant);
double moveProbability(Ant* ant, Edge* edge, double intensParam, double visibParam);

class Graph
{
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;
	std::vector<Ant*> ants;
	std::vector<Edge*> visitedEdges;
	std::tuple<std::vector<Node*>, int> shortestPath;

public:
	std::vector<Node*>& getNodes() { return nodes; }
	std::vector<Node*> getNodesCopy() { return nodes; }

	std::vector<Edge*>& getEdges() { return edges; }

	std::vector<Ant*>& getAnts() { return ants; }
	std::vector<Ant*> getAntsCopy() { return ants; }

	std::tuple<std::vector<Node*>, int>& getShortestPath() { return shortestPath; }

	Node* addNode(int index)
	{
		if (index <= 0)
		{
			std::cout << "Index musi byc wiekszy niz 0\n";
			return nullptr;
		}

		Node* node = new Node();
		if (node == nullptr)
		{
			std::cout << "Nie mozna stworzyc nowego Node\n";
			return nullptr;
		}

		(*node).index = index;
		nodes.push_back(node);
		return node;
	}

	Edge* addEdge(Node* src, Node* dest, int weight, double pher)
	{
		Edge* edge = new Edge();
		(*edge).src = src;
		(*edge).dest = dest;
		(*edge).weight = weight;
		(*edge).pher = pher;
		edges.push_back(edge);
		(*src).edges.push_back(edge);
		return edge;
	}

	Edge* addEdge(int srcIndex, int destIndex, int weight, double pher)
	{
		Node* src = nullptr, * dest = nullptr;
		for (auto& node : nodes)
		{
			if ((*node).index == srcIndex) src = node;
			
			if ((*node).index == destIndex)	dest = node;
			
			if (src != nullptr && dest != nullptr) break;
		}
		if (src == nullptr || dest == nullptr)
		{
			std::cout << "Nie mozna odnalezc node'ow o zadanych indexach\n";
			return nullptr;
		}
		return addEdge(src, dest, weight, pher);
	}

	void addNonDirectedEdge(int n1index, int n2index, int weight, double pher)
	{
		addEdge(n1index, n2index, weight, pher);
		addEdge(n2index, n1index, weight, pher);
	}

	// add 1 ant in each node
	void addAnts()
	{
		int index = 1;
		for (auto& node : nodes)
		{
			Ant* ant = new Ant();
			(*ant).index = index++;
			(*ant).currNode = node;
			(*ant).visitedNodes = {};
			(*ant).nvn = nodes;
			popCurrentNode(ant);
			(*ant).moveProbs = {};
			(*ant).pathLength = 0;
			ants.push_back(ant);
		}
	}

	// create probabilities for each node for each ant
	void pushProbs(double intensParam, double visibParam)
	{
		for (auto& ant : ants)
		{
			for (auto& edge : (*(*ant).currNode).edges)
			{
				for (auto& node : (*ant).nvn)
				{
					if ((*(*edge).dest).index == (*node).index)
					{
						(*ant).moveProbs.push_back(std::make_tuple(node, moveProbability(ant, edge, intensParam, visibParam)));
					}
				}
			}
		}
	}

	// find given edges weight
	int findEdge(Node* src, Node* dest)
	{
		for (auto& edge : (*src).edges)
			if ((*(*edge).dest).index == (*dest).index) return (*edge).weight;
	}

	int findEdge(int srcIndex, int destIndex)
	{
		Node* startNode = nullptr;
		Node* endNode = nullptr;
		for (auto& node : nodes)
		{
			if ((*node).index == srcIndex) startNode = node;
			if ((*node).index == destIndex) endNode = node;
		}

		return findEdge(startNode, endNode);
	}

	// update pheromones on every edge
	void pheromoneUpdate(double rho, double Q)
	{
		double dT = 0;

		for (auto& ant : ants)
		{
			int lvIndex = (*ant).index;
			for (auto& node : (*ant).visitedNodes)
			{
				for (auto& edge : edges)
				{
					// if ant travelled on that edge, add it to visited edges
					if ((*(*edge).src).index == lvIndex && (*(*edge).dest).index == (*node).index)
					{
						visitedEdges.push_back(edge);
					}
				}
			}
		}

		// update edges
		for (auto& edge : edges)
		{
			(*edge).pher *= (1 - rho);
		}

		//update only visited edges
		for (auto& ant : ants)
		{
			for (auto& edge : visitedEdges)
			{
				(*edge).pher += ((double)Q / (double)(*ant).pathLength);
			}
		}
	}

	// remove all ants, so new generation can be born
	void reset()
	{
		ants.clear();
	}

	// initialize shortest path, set it to empty vector
	void initSP()
	{
		std::vector<Node*> v = {};
		shortestPath = std::make_tuple(v, INT_MAX);
	}
};

// remove ants current node from not visited nodes
void popCurrentNode(Ant* ant)
{
	std::vector<Node*> notVisitedNodes = {};
	for (auto& node : (*ant).nvn)
	{
		if ((*node).index == (*(*ant).currNode).index) continue;
		
		notVisitedNodes.push_back(node);
		
	}
	(*ant).nvn = notVisitedNodes;
}

// part of probability calculations
double citiesSum(Ant* ant)
{
	double result = 0;

	for (auto& node : (*ant).nvn)
		for (auto& edge : (*(*ant).currNode).edges)
		{
			if ((*(*edge).dest).index == (*node).index) result += (*edge).pher / (*edge).weight;
		}

	return result;
}

// generate probability for single edge for single ant
double moveProbability(Ant* ant, Edge* edge, double intensParam, double visibParam)
{
	double u = pow((*edge).pher, intensParam) * pow(pow((*edge).weight, -1) , visibParam);
	double l = citiesSum(ant);

	return u / l;
}

// print path on screen
void showPath(std::vector<Node*>& path)
{
	for (auto& node : path)	std::cout << " -> " << (*node).index;
}


std::vector<Edge*> antColonyAlgo(Graph& graph)
{
	int size = graph.getAntsCopy().size();

	// random number between 0 and 1 generator
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> distr(0, 1);

	double rand = 0;

	// while there are nodes that weren't visited
	while ((size--) > 0)
	{
		for (auto& ant : graph.getAnts())
		{
			double total = 0;
			rand = distr(eng);

			// check where in probabilities spread did random number land
			for (auto& prob : (*ant).moveProbs)
			{
				total += std::get<1>(prob);
				if (rand > total) continue;
				// if random number landed here, add this node to ants visited nodes and set it to current node
				else
				{
					(*ant).pathLength += graph.findEdge((*ant).currNode, std::get<0>(prob));
					(*ant).currNode = std::get<0>(prob);
					(*ant).visitedNodes.push_back((*ant).currNode);
					break;
				}
			}
		}

		for (auto& ant : graph.getAnts())
		{
			// for each ant, remove current node from not visited nodes
			popCurrentNode(ant);

			// for each ant, clear move probabilities
			(*ant).moveProbs.clear();

			//for each ant, generate new probabilities with 1 less node
			graph.pushProbs(1, 1);
		}
	}
	
	for (auto& ant : graph.getAnts())
	{
		// create temporary node, we will use it to check if theres new shortest path
		Node* fn = new Node();
		for (auto& node : graph.getNodes())
		{
			if ((*node).index == (*ant).index) fn = node;
		}

		(*ant).pathLength += graph.findEdge((*(*ant).currNode).index, (*ant).index);
		
		// if new path is shorter, replace graphs current shortest path with it
		if ((*ant).pathLength < std::get<1>(graph.getShortestPath()))
		{
			std::vector<Node*> sp = {};
			sp.push_back(fn);
			sp.insert(sp.end(), (*ant).visitedNodes.begin(), (*ant).visitedNodes.end());
			sp.push_back(fn);
			std::get<0>(graph.getShortestPath()) = sp;
			std::get<1>(graph.getShortestPath()) = (*ant).pathLength;
		}
	}

	// update pheromones
	graph.pheromoneUpdate(0.25, 400);

	//remove all ants
	graph.reset();

	return {};
}

int main()
{
	Graph g;
	g.addNode(1);
	g.addNode(2);
	g.addNode(3);
	g.addNode(4);
	g.addNode(5);

	g.addNonDirectedEdge(1, 2, 38, 3);
	g.addNonDirectedEdge(1, 3, 74, 2);
	g.addNonDirectedEdge(1, 4, 59, 2);
	g.addNonDirectedEdge(1, 5, 45, 2);

	g.addNonDirectedEdge(2, 3, 46, 1);
	g.addNonDirectedEdge(2, 4, 61, 1);
	g.addNonDirectedEdge(2, 5, 72, 1);

	g.addNonDirectedEdge(3, 4, 49, 2);
	g.addNonDirectedEdge(3, 5, 85, 2);

	g.addNonDirectedEdge(4, 5, 42, 1);

	g.initSP();

	int i = 5;

	while (i--)
	{
		g.addAnts();
		g.pushProbs(1, 1);
		antColonyAlgo(g);
	}

	showPath(std::get<0>(g.getShortestPath()));
	std::cout << "\nTotal path length: " << std::get<1>(g.getShortestPath()) << "\n\n";
}
