//
//  main.cpp
//  Ford_Fulkerson
//
//  Created by Abhyudaya on 20/11/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Graph.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <unordered_map>
#include <map>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <chrono>
#include <memory>
#include <memory>

int breadthFirstSearch(Graph& residualGraph, int source, int sink, std::vector<int>& parent, std::vector<bool>& reached) {
    size_t V = residualGraph.adjacencyList.size();
    std::queue <int> queue;
    queue.push(source);
    reached[source] = true;
    parent[source] = -1;
    
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        std::unordered_map<int, int>& neighbors = residualGraph.adjacencyList[u];
        for(auto& edgeWeightPair:neighbors ) {
            if (reached[edgeWeightPair.first]==false && edgeWeightPair.second > 0) {
                queue.push(edgeWeightPair.first);
                parent[edgeWeightPair.first] = u;
                reached[edgeWeightPair.first] = true;
            }
        }
    }
    return reached[sink];
}

cv::Mat buildSegmentedImage (const Graph& graph, const std::vector<bool>& reached, const int& width, const int& height) {
    int foregroundCount = 0;
    int backgroundCount = 0;
    cv::Mat binaryImage = cv::Mat(height,width,CV_8UC1);
    for (int i=0; i< graph.adjacencyList.size()-1; ++i) {
        int rowVal = i/width;
        int colVal  = i%width;
        if ( reached[i+1] ) {
            binaryImage.at<uchar>(rowVal,colVal) = 0;
            foregroundCount++;
        }
        else {
            binaryImage.at<uchar>(rowVal,colVal) = 255;
            backgroundCount++;
        }
    }
    return binaryImage;
}

// method returns binary image after max flow using ford_fulkerson with breadth first search
cv::Mat Ford_Fulkerson (Graph& graph, int source, int sink, int width, int height ) {
    // create a copy for segmentation
    Graph prevGraph(graph);
    
    std::vector<bool> reached(graph.adjacencyList.size(),false);
    int u, v, maxFlow=0;
    
    std::vector<int> parent(graph.adjacencyList.size());
    // check if there is a path from source to sink
    while (breadthFirstSearch(graph, source, sink, parent,reached)) {
        // Find the directed edge with minimum capacity in the s-t path
        int minCapacityEdge = std::numeric_limits<int>::max(); // set some high value in the beginning;
        for (v=sink; v!=source; v=parent[v]) {
            u = parent[v];
            minCapacityEdge = std::min(minCapacityEdge, graph.adjacencyList[u][v]);
        }
        // push flow equal to the minCapacityEdge in the s-t path
        // update the graph, increasing flow decreasing the capacity on forward direction
        // and increasing capacity on the revese directions
        for (v=sink; v != source; v=parent[v]) {
            u = parent[v];
            graph.adjacencyList[u][v] -= minCapacityEdge;
            graph.adjacencyList[v][u] += minCapacityEdge;
        }
        
        // Update the flow with amount pushed.
        maxFlow += minCapacityEdge;
        std::fill(reached.begin(),reached.end(),false);
    }  
    return buildSegmentedImage(prevGraph, reached, width,height); 
}

int main(int argc, const char * argv[]) {
    std::string imagename;
    int gmmornot = 0;  // default is gmm
    int numberofClusters = 2; // default is 2 cluster
    std::string filename(" ");
    std::cout << "Ford Fulkerson implementation begin() " << std::endl;
    
    switch (argc) {
        case 2: imagename = std::string(argv[1]);
                break;
        case 3:
                imagename = std::string(argv[1]);
                gmmornot = std::atoi(argv[2]);
                break;
        case 4:
                imagename = std::string(argv[1]);
                gmmornot = std::atoi(argv[2]);
                numberofClusters = std::atoi(argv[3]);
                break;
        default:
                std::cout << "Invalid number of arguments" << std::endl;
                std::cout << "Please provide image name and an integer (0 or 1! 0 for GMM, 1 for K-Means ) and number of segments " << std::endl;
                return 0;
    }

    std::string baselocation = ("./");
    std::string imageFile = baselocation + std::string("image/") + std::string(imagename);
    
    if (gmmornot == 0 )
        filename = baselocation + "gmm_clustering.py ";
    else
        filename = baselocation + "k-likelihood.py ";

    std::string command = "python ";
    command += filename;
    command += imageFile + std::string(" ") + std::to_string(numberofClusters);
    system(command.c_str());
    std::cout << "The command used to call the python file is " << command << std::endl;

    cv::Mat image;
    image = cv::imread(imageFile);
    
    if (!image.data) {
        std::cout << "Unable to read image file " << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // read the kmeans output and store it as a vector
    std::string file = baselocation + std::string("output.txt");
    std::ifstream label(file.c_str());
    std::vector<int> likelihoodArray;
    if ( label.is_open() ) {
        std::string line;
        while (std::getline(label, line) ) {
            likelihoodArray.push_back(std::stoi(line));
        }
        label.close();
    }else {
        std::cout << "Cannot read data file file" << std::endl;
    }
    
    std::cout << "The size of likelihood array is " << likelihoodArray.size() << std::endl;
    Graph* inputGraph = Graph::createGraph(image.cols,image.rows,4,likelihoodArray);
    inputGraph->addEdge(inputGraph->adjacencyList.size(),inputGraph->adjacencyList.size(),0);
    std::cout << "The size of the graph is " << inputGraph->adjacencyList.size() << std::endl;
    cv::Mat binaryImage= cv::Mat(image.cols,image.rows,CV_8UC1);
    auto start = std::chrono::system_clock::now();
    binaryImage=Ford_Fulkerson(*inputGraph,0,inputGraph->adjacencyList.size()-1,image.cols, image.rows);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout<<elapsed.count();
    std::cout<<"\n";
    
    cv::imshow("Segmented Image",binaryImage);
    cv::waitKey(0);
    delete inputGraph;
    return 0;
    
}

