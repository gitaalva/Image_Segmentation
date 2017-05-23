#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

class Graph {
	Graph();
public:
	Graph(const Graph &g);
	Graph& operator=(const Graph &g);
    
    void addEdge(const int& source, const int& dest, const int& weight );
    void numberOfEdges() const;
    static Graph* createGraph ( const int& width, const int& height, const int& penalty, std::vector<int>& likelihoodArray);

    std::unordered_map<int,std::unordered_map<int,int> > adjacencyList;
};

#endif // GRAPH_HPP