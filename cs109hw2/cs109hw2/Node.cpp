#include <iostream>
#include <cstring>
#include <vector>
#include <list>
#include <random>
#include <cstdlib>

using namespace std;
class Node{
public:
	static vector<Node> map;
	static int totalNodes;
	vector<Node> connections;	
	int NodeID;

	static Node NewNode(int ID, int weight){
		return Node(ID, weight);
	}

	 Node(int ID, int weight){
		NodeID = ID;
		CreateConnections(weight);
		totalNodes++;
		map.push_back(*this);
	}

	void CreateConnections(int weight){
		for(int i=0; i<totalNodes; i++){
			if(rand()%(weight-1)==0){
				this->connections.push_back(map.at(i));
			}
		}
	}

	vector<Node> GetMap(){
		return map;
	}

	void PrintMap(){
		for(unsigned int edge = 0; edge<map.size(); edge++){
			cout<<"edge: "<<edge;
			//for(int connections = 0; connections < map.at(edge).connections.size(); connections++)
				//cout<<"connections :"<<map.at(edge).connections->NodeID;
		}
	}


};