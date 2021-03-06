/**
 * @file Maze.cpp
 * @brief 
 * @author Michael E. Rinos
 * @version 0
 * @date 2017-09-01
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "Maze.h"
#include "Edge.h"

using namespace std;

namespace MST{

    Maze::Maze(size_t size):
    count(0),
    pSize(),
    mSize()
    {
        vector< vector <int> > mat(size);
        for (size_t i = 0; i < size; i++){
            mat[i].resize(size);
        }
        matrix = mat;
            
    }

    Maze::~Maze(){
    //Shouldn't need anything else here
    }

    void Maze::generate(Maze & maze, int n, int seed, double p){
        maze.pSize = n;
        for ( int i = 0; i < n; i++){
            string str = to_string(i);
            Node * n = new Node(str);
            maze.graph.insert(pair<string, Node *>(str, n));
        }
        
        cout<<"Seed: " <<seed<<endl;
        srand(seed);

        while(maze.count != n){
            maze.count = 0;
            for (int i = 0; i < n; i++){
                for (int j = i; j < n; j++){
                    if (i == j) continue;
                    double r = (double) rand() / RAND_MAX;
                    if ( r <= p){
                        int weight = rand() % (n) + 1;
                        maze.matrix[i][j] = weight;
                        maze.matrix[j][i] = weight;

                        string str_i = to_string(i);
                        string str_j = to_string(j);
                        
                        Node * i = maze.graph.find(str_i)->second;
                        Node * j = maze.graph.find(str_j)->second;

                        i->putNeighbor(j, weight);
                        j->putNeighbor(i, weight);
                    }
                } 
            }
            Node * zero = maze.graph.find("0")->second;
            Node * invalid = new Node("-1");
            zero->setPred(*invalid);
            canReachDFS("0");
            //displayGraph();
        }
    }

    void Maze::canReachDFS(string sNode){
        Node * startNode = graph.find(sNode)->second;
        if (startNode->getMarked() == Node::Unknown){
            count++;
        }
        startNode->setMarked( Node::Discovered );
        vector<Node *> neighbors = startNode->getNeighbors();
        for ( auto& n : neighbors){
             if (n->getMarked() == Node::Unknown){
                 graph.find(n->getName())->second->setPred(*startNode);
                 canReachDFS(n->getName());
            }
        }
        startNode->setMarked( Node::Visited);

    }

    void Maze::displayGraph(){
        for (auto iter = graph.begin(); iter != graph.end(); iter++){
            cout<< "Key: " << iter->first << "\t Value: "<<*iter->second<<endl;
        }
    }

    int Maze::getCount(){
        return count;
    }

    vector < Edge > Maze::getMatrix(){
        vector < Edge > edges;
        for (int i = 0; i < count; i++){
            for (int j = 0; j < count; j++){
                if (i == j) continue;
                if (matrix[i][j] == 0) continue;
                else {
                    if (j > i){
                        Edge e(matrix[i][j], i, j);
                        edges.push_back(e);
                    }
                }
            }
        }
        return edges;
    }

    vector < Edge > Maze::getList(){
        vector < Edge > edges;
        for (int i = 0; i < count -1; i++){
            string str = to_string(i);
            Node * n = graph.find(str)->second;
            for (auto neighbor : n->getNeighbors()){
                if (neighbor->getName() > n->getName()){
                    Edge e(n->getWeight(neighbor->getName()), stoi(n->getName()), stoi(neighbor->getName()));
                    edges.push_back(e);
                }
            }
        }
        return edges;
    }

    vector < Node > Maze::getMatrixNodes(){
        vector< Node > nodes;
        for ( int i = 0; i < count; i++){
            string str = to_string(i);
            Node n(str);
            nodes.push_back(n);
        }
        for ( int i = 0; i < count; i++){
            for ( int j = 0; j < count; j++){
                if (i==j) continue;
                if (matrix[i][j] == 0) continue;
                else{
                    nodes[i].putNeighbor(&nodes[j], matrix[i][j]);
                    nodes[j].putNeighbor(&nodes[i], matrix[i][j]);
                }
            }
        }
        return nodes;
    }

    vector < Node > Maze::getListNodes(){
        vector< Node > nodes;
        for (int i = 0; i < count; i++){
            string str = to_string(i);

            nodes.push_back(*graph.find(str)->second);
        }
        return nodes;
    }

    string Maze::DFSInfo() const{
        ostringstream oss{};
        oss<<"Depth-First Search:\n";
        oss<<"Vertices:  \n";
        for ( int i = 0; i < count; i ++)
            oss<<" " << i;
        oss<<"\nPredecessors: \n";
        for ( int i = 0; i < count; i ++){
            oss<< graph.find(to_string(i))->second->getPredecessor()->getName() << " ";
        }
        oss<<"\n";
        return oss.str();
    }

    string Maze::listInfo() const {
        ostringstream oss{};
        oss<<"The graph as an adjacency list:\n";
        for (int i = 0; i < count; i++){
            oss<<*(graph.find(to_string(i))->second);
        }
        oss<<"\n";
        return oss.str();
    }

    string Maze::matrixInfo() const{
        ostringstream oss{};
        oss<<"The graph as an adjacency matrix:\n\n";
        for (int i = 0; i < count; i++){
            oss<<" ";
            for (int j = 0; j < count; j++){
                if ( j == 0)
                    oss<<matrix[i][j];
                else
                    oss<<"    "<<matrix[i][j];
            }
            oss<<"\n\n";
        }
        return oss.str();
    }
    ostream &operator<<(ostream & os, const Maze & maze){
        cout<<"Finished"<<endl<<endl;
        os<<maze.matrixInfo();
        os<<maze.listInfo();
        os<<maze.DFSInfo();
        return os;
    }

} /// Namespace

///Friend functions
