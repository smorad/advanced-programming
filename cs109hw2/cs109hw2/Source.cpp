/*In this program, I was able to work with C++ and able to try and understand it better. I learned about how classes within C++ work, and how they are different from classes in Java. 
I learned about some parts of the standard library, most notably vectors and priority queues. Using a priority queue allowed me to cut the time of one function from N to NlogN which greatly sped up and simplified my code.

I also learned about how Dijkstra’s algorithm, and how it computes the fastest path to a specified place. It is a very useful and powerful algorithm.

I also got familiar with Visual Studio and its built in debugger. This is big for me, because learning to use your IDE correctly greatly enhances your ability and speed at which you code.

Overall, I think the most important thing I learned was how to use the C++ standard libraries. 
They are much more fleshed out than C, and therefore much easier to work with. Since the syntax is very similar to C, once I can learn all the standard libraries, I should have a good understanding of C++.*/


#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <cassert>


using namespace std;

const double WEIGHT = 0.2;			
const int TOTAL_NODES = 50;
const int MAX_COST = 10;
class Node;
typedef Node* NodeRef;
class Compare;				//used for node comparator function
int numLinks = 0;			//total number of links
vector<NodeRef> edgeList;
enum color{RED, BLACK};

int nodeCount = 0;
class Node{
	int nodeid;


public:
	color color;
	vector<NodeRef> links;
	int cost;
	NodeRef prev;											//ptr to prev node for cost calculation
	bool visited;

	Node(){
		if(rand()%2==1) color=BLACK; else if(rand()%2==0) color=RED;
		nodeid=nodeCount;
		cost=rand()%MAX_COST+1;
		++nodeCount;
		prev=NULL;
		visited = false;
	}
	void Visited(bool b){visited = b;}
	int GetId(){return nodeid;}
	vector<NodeRef> GetLinks(){return links;}
	int GetCost(){return cost;}
	enum color GetColor(){return color;}
	void SetCost(int c){cost = c;}
	void SetColor(enum color c){color = c;}
};

class Compare{						//comparator for priority queue
public:
	bool operator() (NodeRef a, NodeRef b){
		return a->GetCost()<b->GetCost();
	}

};

inline float Prob(){
	return (static_cast<float>(rand())/RAND_MAX);
}

void PrintList(){
	for(unsigned iter=0; iter<edgeList.size(); iter++){
		cout<<(*edgeList.at(iter)).GetId()<<":   ";		//print current node
		for(unsigned link=0; link<(*edgeList.at(iter)).links.size(); link++){
			cout<<(*edgeList.at(iter)).links.at(link)->GetId()<<" ";
		}
		cout<<endl;
	}
}

int CalculateCost(NodeRef n){			//run backwards up the list to calculate total cost//works
	int cost = 0;
	while(n->prev!=NULL){
		cost = cost+  n->GetCost();
		n = n->prev;
	}
	return cost;
}

bool Find(vector<NodeRef> v, NodeRef n){
	for(int i = 0; i<v.size(); i++){
		if(v.at(i)==n)
			return true;
		cout<<"find";
	}
	return false;

}


int Dijkstra(NodeRef start, int want, color color){	//to find distance, create node.prev which contains the prev node.you can go backwards and add up all nodes. 
	priority_queue<NodeRef, vector<NodeRef>, Compare> openSet;
	vector<NodeRef> closedSet;
	openSet.push(start);											//add initial node to open set
	while(openSet.size()!=0){
		NodeRef current = openSet.top();								//node with least cost
		closedSet.push_back(current);
		current -> visited = true;
		if(current->GetId() == want){									//if wanted node is found
			return CalculateCost(edgeList.at(want));		
		}
		openSet.pop();												//remove current node from open set
		if(current->GetCost()==INT_MAX){                               //current node has no connections
			cout<<"No connections found, break"<<endl;
			break;	
		}	 
		for(int link=0; link<(current)->links.size(); link++){
			if((!current->links.at(link)->visited) && current->links.at(link)->color == color){		//if unvisited and correct color
				current->links.at(link)->Visited(true);
				current->links.at(link)->prev = current;											
				int alt = CalculateCost(current) + (CalculateCost(current->links.at(link)) - CalculateCost(current));		//relaxing the edge
				if(alt<CalculateCost(current)){									//if found a faster way
					(current->links.at(link))->cost = alt;				
			}	
				openSet.push(current->links.at(link));							
			}

		}
	}
	return INT_MAX;										//failed to find node
}


void LinkRandom(){
	int index = rand()%edgeList.size();
	int index2 = rand()%edgeList.size();
	if(index!=index2){
		(*edgeList.at(index)).links.push_back(edgeList.at(index2));
		(*edgeList.at(index2)).links.push_back(edgeList.at(index));
	}
}

void ReinitNodes(){
	for(int i=0; i<edgeList.size(); i++){
		edgeList.at(i)->visited=false;
	}

}


void GetAverageCost(int iterMax, color color){
	long avg = 0;
	for(int iter = 0; iter<iterMax; ++iter){
		ReinitNodes();
		edgeList.at(0)->color = color;
		edgeList.at(iter)-> color = color;						//make sure node we want is correct color
		int tmp = Dijkstra(edgeList.at(0), iter, color);
		if(tmp!=INT_MAX)
			avg = avg+ tmp;
	}
	string s;
	if(color==RED) s = "RED"; else s = "BLACK";
	cout<<"Average total cost for "<<s<<" : "<<avg/iterMax<<endl;
}

inline bool CalculateDensity(){
	return numLinks<WEIGHT*((edgeList.size()*(edgeList.size()-1)));
}

void CreateGraph(){
	cout<<"populating edgelist..."<<endl;
		for(int i=0; i<TOTAL_NODES; i++){
		NodeRef ptr = new Node();
		edgeList.push_back(ptr);		//populate edgelist with nodes
	}
	cout<<"edgelist populated"<<endl;
	cout<<"linking nodes..."<<endl;
	while(CalculateDensity()){
		LinkRandom();
		numLinks++;
	}

	cout<<"nodes linked"<<endl;
	PrintList();

}


int main(){
	//freopen("output.txt","w",stdout);			//uncomment to output to file
	srand(static_cast<unsigned>(time(NULL)));
	CreateGraph();
	GetAverageCost(TOTAL_NODES, RED);
	GetAverageCost(TOTAL_NODES, BLACK);

}

