/*NOTE: This program contains deprecated methods and data members, used to keep compatibility with Dijkstra's algorithm
if you see a method is not used in Kruskal, it was probably used in Dijkstra*/

/*To make it easier on the graders, instead of including a readme I will put it here in a block comment:
This program was written in Visual Studio, but I have tested it on the Unix environment as well. To compile,
use the command "g++ std=gnu++0x [filename.cpp]". The program will not compile if you do not specify the 2011 standard
with -std=gnu++0x.

The file this program looks for is Mstdata.txt. If you want to change the filename, just edit the string filename under
the declarations and globals section.

The result format is: ColorOfEdge FirstNode SecondNode CostofEdge
*/

/*What I Learned:
In this program, I was able to familiarize myself with C++ I/O and operator overloading. I learned how to use 
iostream and fstream. Using the ifstream is much different than traditional c, and much more reminiscent of object
oriented languages. I like having a stream object because of all the member methods you can call on it.

Not only did I learn how to access filestreams and gather data from them, I also learned how to parse the input
into usable data. I did this mostly by using a string tokenizer with space as a delimiter. I then would create an edge
with the values I recorded from the input stream.

I learned how to overload the << operator for enumerated types, and learned more about enumerated types in C++
in general. They make code much more readable

I also solidified my knowledge on the graph data structure, and how it works. I modified my graph data structure from
the last assignment, because I didn't have as firm of a grasp on the data structure as I do now.

Finally, I cemented my understanding of Kruskal's algorithm.

I also learned that one use for Kruskal's algorithm is maze generation.

*/
#include <cstring>
#include <string>
#include <iostream>
#include <istream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <cassert>
#include <fstream>
#include <climits>
#include <set>

/*Declarations and Globals*/
using namespace std;
class Compare;
typedef class Node *NodeRef;
typedef class Graph *GraphRef;
typedef class Edge *EdgeRef;
GraphRef g;
enum color {RED, GREEN, BLUE, NONE};
const string filename = "Mstdata.txt";
const int STREAM_SIZE = 10000;

/*Prints enums as strings instead of ints*/
ostream& operator<<(ostream& os, const color& color){
	string s;
	switch (color)
	{
	case RED: s="RED";
		break;
	case GREEN: s="GREEN";
		break;
	case BLUE: s="BLUE";
		break;
	case 3: s="NONE";
		break;
	default:
		break;
	}
	os<<s;
	return os;
}

/*An edge is a container which holds 2 nodes, a cost, and a color*/
/*Almost all of Kruskals uses edges instead of the node data type*/
/*Edges basically abstract nodes away from the user*/
class Edge{
public:
	bool used;
	NodeRef a;
	NodeRef b;
	int cost;
	enum color color;
	Edge(NodeRef a, NodeRef b, int cost, enum color color):a(a), b(b), cost(cost), color(color){}
	Edge(){}
	bool operator==(EdgeRef e){
		return ((this->a == e->a || this->b == e->a) && (this->b==e->b || this->a==e->b));

	}


};

/*Nodes make up a graph
/*Associated costs and colors are now vectors instead of their previous types
/*However, I have both types to ensure compatability with my older Dijkstra function
/*Nonvector color and cost are not used by Kruskal's
*/
class Node{
	int nodeid;
	enum color color;
	int cost;
	bool visited;
	int j;

public:
	vector<NodeRef> links;
	vector<EdgeRef> edgeList;
	NodeRef prev;											//ptr to prev node for cost calculation
	Node(NodeRef Node){
		*this = Node;
	}

	Node(int id, int j, int cost, enum color color){
		nodeid=id;
		this->cost = cost;
		this->color = color;
		prev=NULL;
		visited = false;
	}
	Node(int id){
		cost = 0;
		nodeid = id;
		visited = false;
		prev = NULL;
	}
	void Visited(bool b){
		visited = b;
	}
	bool Visited(){
		return visited;
	}
	int GetId(){
		return nodeid;
	}
	vector<NodeRef> GetLinks(){
		return links;
	}
	int GetCost(){
		return cost;
	}
	enum color GetColor(){
		return color;
	}
	void SetCost(int c){
		cost = c;
	}
	void SetColor(enum color c){
		color = c;
	}
	NodeRef GetPrev(){
		return prev;
	}
	void SetPrev(NodeRef n){
		prev=n;
	}
	void CreateEdge(NodeRef a, NodeRef b, int cost, enum color color){
		EdgeRef e = new Edge(a,b,cost,color);
		edgeList.push_back(e);
	}

};


/*A comparator needed for the priority queue*/
class Compare{						
public:
bool operator() (NodeRef a, NodeRef b){
		return a->GetCost()<b->GetCost();
	}
	bool operator() (EdgeRef a, EdgeRef b){
		return b->cost<a->cost;
	}
};

/*Basically just a container for nodes
And their associated functions*/
class Graph{
	
	int nodeCount;				//incremented node counter
	int graphSize;				//max nodes in graph
	int numLinks;				//used for density calculations
	float density;
public:
	vector<NodeRef> vertexList;	//list of all edge nodes
	vector<EdgeRef> edgeList;

/*Probablility calculator for random edges used in Dijkstra*/
inline float Prob(){
	return (static_cast<float>(rand())/RAND_MAX);
}

/*Prints out each edge for Dijkstra*/
void PrintList(){
		for(unsigned i=0; i<edgeList.size(); i++)
			cout<<edgeList.at(i)->a->GetId() << " "<<edgeList.at(i)->b->GetId()<<endl;
	cout<<"\n\n\n";
}

/*Uses stack to run backwards and calculate cost from start to this node. Used for Dijkstra.*/
int CalculateCost(NodeRef n){		
	int cost = 0;
	while(n->prev!=NULL){
		cost = cost+  n->GetCost();
		n=n->prev;
	}
	return cost;
}


int Dijkstra(NodeRef start, int want, color color){	//to find distance, create node.prev which contains the prev node.you can go backwards and add up all nodes. 
	priority_queue<NodeRef, vector<NodeRef>, Compare> openSet;
	vector<NodeRef> closedSet;
	openSet.push(start);											//add initial node to open set
	while(openSet.size()!=0){
		NodeRef current = openSet.top();								//node with least cost
		cout<<"Current: "<<current->GetId()<<endl;
		closedSet.push_back(current);
		current -> Visited(true);
		if(current->GetId() == want){									//if wanted node is found
			return CalculateCost(vertexList.at(want));		
		}
		openSet.pop();												//remove current node from open set
		if(current->GetCost()==INT_MAX){                               //current node has no connections
			cout<<"No connections found, break"<<endl;
			break;	
		}	 
		for(int link=0; link<(current)->links.size(); link++){
			if((!current->links.at(link)->Visited())){		//if unvisited and correct color
				current->links.at(link)->Visited(true);
				current->links.at(link)->SetPrev(current);	//FIX									
				int alt = CalculateCost(current) + (CalculateCost(current->links.at(link)) - CalculateCost(current));		//relaxing the edge
				if(alt<CalculateCost(current)){									//if found a faster way
					(current->links.at(link))->SetCost(alt);	//FIX		
			}	
				openSet.push(current->links.at(link));	
			}

		}
	}
	cout<<"Failed to find node"<<endl;
	return INT_MAX;										//failed to find node
}

/*Kruskal's algorithm that returns the MST as a vector using wikipedia definition:
create a forest F (a set of trees), where each vertex in the graph is a separate tree
create a set S containing all the edges in the graph
while S is nonempty and F is not yet spanning
remove an edge with minimum weight from S
if that edge connects two different trees, then add it to the forest, combining two trees into a single tree
otherwise discard that edge.
*/
vector<EdgeRef> Kruskal(color color, enum color color2=NONE, enum color color3=NONE){
	vector<int> cycles;
	int cost=0;
	int i=0;
	int cycleInt = 0;
	vector<EdgeRef> mst;
	//mst.clear();
	priority_queue<EdgeRef, vector<EdgeRef>, Compare> edgeSet;
	for(int iter = 0; iter<edgeList.size(); iter++){	//add all edges to open set
		edgeSet.push(edgeList.at(iter));
	}
	for(i=0; i<g->graphSize; i++) //initialize cycle checker
		cycles.push_back(i);
	while((mst.size() < graphSize-1) && edgeSet.size()){
		EdgeRef current = edgeSet.top();
		if(current->color == color || current->color == color2 || current->color == color3){
			int edgeA = current->a->GetId();
			int edgeB = current->b->GetId();
			if(cycles[edgeA] != cycles[edgeB]){ //Check to make sure graph is not cyclic
				cycleInt = cycles[edgeB];
				for(i=0; i<graphSize;i++){
					if(cycles[i] == cycleInt){
						cycles[i]=cycles[edgeA];
					}

				}
				mst.push_back(current);
			}
		}
		edgeSet.pop();
	}
	return mst;
}



/*Links nodes until at certain density used for Dijkstra*/
void LinkRandom(){
	int index = rand()%vertexList.size();
	int index2 = rand()%vertexList.size();
	if(index!=index2){
		(*vertexList.at(index)).links.push_back(vertexList.at(index2));
		(*vertexList.at(index2)).links.push_back(vertexList.at(index));
	}
}

/*Reset graph */
void ReinitNodes(){
	for(int i=0; i<vertexList.size(); i++){
		vertexList.at(i)->Visited(false);
	}
	for(int i=0; i<edgeList.size(); i++)
		edgeList.at(i)->used = false;

}

/*Gets the cost of all 50 nodes for Dijkstra's algorithm*/
void GetAverageCost(int iterMax, color color){
	long avg = 0;
	for(int iter = 0; iter<iterMax; ++iter){
		ReinitNodes();
		vertexList.at(0)->SetColor(color);
		vertexList.at(iter)->SetColor(color);						//make sure node we want is correct color
		int tmp = Dijkstra(vertexList.at(0), iter, color);
		if(tmp!=INT_MAX)
			avg = avg+ tmp;
	}
	string s;
	if(color==RED) s = "RED"; else s = "BLACK";
	cout<<"Average total cost for "<<s<<" : "<<avg/iterMax<<endl;
}

/*Calculates if graph is at specified density used for Dijkstra*/
inline bool CalculateDensity(){
	return numLinks<density*((vertexList.size()*(vertexList.size()-1)));
}

Graph(int graphSize, float density = 0.4){
	this->graphSize = graphSize;
	this->density = density;
	this->numLinks = 0;
	this->nodeCount = 0;
	for(int i=0; i<graphSize; i++)
		AddNode(i);
}
/*Add edge to graph*/
void AddEdge(NodeRef a, NodeRef b, int cost, enum color color){
	edgeList.push_back( new Edge(a,b,cost,color));
	a->links.push_back(b);
}

/*Adds node to graph*/
void AddNode(int id){
	NodeRef n = new Node(id);
	vertexList.push_back(n);
}
void AddNode(int i, int j, int cost, enum color color){
	if(vertexList[i]==NULL){
		NodeRef n = new Node(i, j, cost, color);
		vertexList.push_back(n);
	}
	else if (vertexList[j] == NULL){
		NodeRef n = new Node(j);
		vertexList.at(j)->links.push_back(n);
	}
}
};

void AssignValues(int i, int j, int cost, color c){
			g->AddEdge(g->vertexList.at(i), g->vertexList.at(j), cost, c);
}

/*Prints out the MST in a readable format*/
void PrintVector(vector<EdgeRef> v, string s){
	int cost=0;
	cout<<s<<":"<<endl;
	cout<<"Color\tNode\tNode\tCost\t"<<endl;
	cout<<"-------------------------------------------"<<endl;
for(int i=0; i<v.size(); i++){
		cout<<v.at(i)->color<<"\t"<<v.at(i)->a->GetId()<<"\t"<<v.at(i)->b->GetId()<<"\t"<<v.at(i)->cost<<endl;
		cost+=v.at(i)->cost;
	}
	//cout<<cost<<"\n\n\n"<<endl;
	cout<<"\n\nTotal cost for "<<s<<" is: \t\t"<<cost<<endl<<endl;
}

void TestKruskal(GraphRef g){
	vector<EdgeRef> tmp;
	tmp = g->Kruskal(RED);
	PrintVector(tmp, "Red");
	tmp = g->Kruskal(GREEN);
	PrintVector(tmp, "GREEN");
	tmp = g->Kruskal(BLUE);
	PrintVector(tmp, "BLUE");
	tmp = g->Kruskal(RED, GREEN);
	PrintVector(tmp, "REDGREEN");
	tmp = g->Kruskal(BLUE, GREEN);
	PrintVector(tmp, "BLUEGREEN");
	tmp = g->Kruskal(RED, GREEN, BLUE);
	PrintVector(tmp, "ALL COLORS");
}

/*Reads input and initializes graph with input
/*Strings are parsed at the newline character by getline
/*The resulting line is then parsed by the string tokenizer
/*The values are converted to their correct format
*/
int main(){
	srand(time(NULL));		//seed random
	ifstream input;
	input.open(filename);
	if(input==NULL){
		cout<<"Mstdata.txt not found, terminating"<<endl;
		exit(1);
	}
	int lineNum= 0;
	char in[256];
	string a;
	while(input.good()){
		input.getline(in, STREAM_SIZE); 
		if(lineNum==0) {
			g= new Graph(atoi(in));
			lineNum++;
		}
		else{
			/*Parse with " " as delim*/
			int i = atoi(strtok(in, " "));
			int j = atoi(strtok(NULL, " "));
			int cost = atoi(strtok(NULL, " "));
			int color = atoi(strtok(NULL, " "));
			enum color c = (enum color)color;
			AssignValues(i, j, cost, c);

		}
	}
	//g->PrintList(); //Uncomment me to print all edges
	cout<<endl<<endl; //formatting
	TestKruskal(g);
	//cout<<g->Prim(BLUE,RED,GREEN)<<endl;



}
