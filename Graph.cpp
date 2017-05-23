#include "Graph.hpp"

Graph::Graph() {
    
}

Graph::Graph (const Graph& g):adjacencyList(g.adjacencyList) {

}

Graph& Graph::operator=(const Graph& g) {
    Graph temp(g);
    std::swap (adjacencyList,temp.adjacencyList);
    return *this;
}

void Graph::addEdge(const int& source, const int& dest, const int& weight ) {
    if ( adjacencyList.find(source) == adjacencyList.end() ) {
        std::unordered_map<int,int> edge;
        edge.insert(std::pair<int,int>(dest,weight));
        adjacencyList.insert(std::pair<int, std::unordered_map<int,int> >(source,edge));
    } else {
        adjacencyList[source].insert(std::pair<int,int>(dest,weight));
    }
}

void Graph::numberOfEdges() const {
    int size = 0;
    for ( auto& pair:adjacencyList ) {
        size += pair.second.size();
    }
    std::cout<<"number of edges are "<<size<<"\n";
}

Graph* Graph::createGraph ( const int& width, const int& height, const int& penalty, std::vector<int>& likelihoodArray ) {
    std::cout << "imagesegmentation::createGraph() begin" << std::endl;
    int imageSize = width*height;
    std::cout << "The image size passed is " << imageSize << std::endl;
    Graph* g = new Graph();

    for (int i=0; i<width*height; ++i ) {
        g->addEdge(0,i+1,likelihoodArray.at(i));
        g->addEdge(i+1,(width*height)+1,10-likelihoodArray.at(i));
    }

    for ( int i=1; i<= imageSize; ++i ) {
        int rownum = (i-1)/width;
        int colnum = (i-1)%width;
        //The above rownum and colnum work when we start with 0,0 as our indices;
        //since here our vertices start with 1,1
        rownum = rownum+1;
        colnum = colnum+1;
        // first row
        if ( rownum == 1 ) {
            // horizontal next pixel
            if ( i != width ) g->addEdge(i,i+1,penalty);
            // horizontal previous pixel
            if ( colnum != width ) g->addEdge(i,i-1,penalty);
            // vertical below pixel
            g->addEdge(i,i+width,penalty);
        } else if ( colnum == 1 ) {  // first column
            // we don't have to worry about the edge condition for pixel 1 since it will go to previous if loop;
            g->addEdge(i,i+1,penalty);
            g->addEdge(i,i-width,penalty);
            //last row
            if ( rownum != height) g->addEdge(i,i+width,penalty);
        } else if ( rownum == height ) {
            if ( colnum != width )
                g->addEdge(i,i+1,penalty);
            g->addEdge(i,i-width,penalty);
            // no check required here since colnum 1 will go to previous loop
            g->addEdge(i,i-1, penalty);
        } else if ( colnum == width ) {
            g->addEdge(i,i+width,penalty);
            g->addEdge(i,i-width,penalty);
            g->addEdge(i,i-1,penalty);
        } else {
            g->addEdge(i,i+width,penalty);
            g->addEdge(i,i-width,penalty);
            g->addEdge(i,i-1,penalty);
            g->addEdge(i,i+1,penalty);
        }
    }
    g->numberOfEdges();
    return g;
}