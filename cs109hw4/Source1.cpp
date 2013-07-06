/*

WRITE UP:
In this program I learned many things. I learned how the game of hex works, and how to implement it using a graph data type. 
I also learned to perfect my graph ADT. Previous programs using the Graph class were not as canonical as this one. 
This graph ADT adheres to the C++ style better. I also learned how to use preprocessor commands to check the runtime environment. 
This is very useful if I ever wanted to distribute source code for other to compile on various operating system.

I also learned how to set up a game in C++. By this, I mean I learned how to have a computer and person take turns, until there is a winner.
I learned a few ways to implement an AI as well. I try multiple ways before settling on the 'block the user' hex paradigm.
I also learned how hard it is to implement a successful AI. 
You have to account for many different cases, and even watch out for the user trying to find holes in your AI logic.

I also learned how to overload the left bitshift operator for user defined types. This comes in handy for debugging.


COMPILATION/GRADING INSTRUCTIONS:

When compiling this program on UNIX, try using this command: g++ [filename].cpp

This program also needs to detect your OS to try and clear the console after every move.
I have tested this on both Unix and Windows, and as long as MacOSX uses the 'clear' command, it should work for Mac too.

If the output is unreadable or messy on your operating system, please grade on UNIX or skip this submission. I tested this on Windows 64 bit and UNIX, and at all times the program should take no more than ~20 lines.

I put everything in one .cpp file to make it easier to grade. I could've used headers and .cpp files for each class, but that increases the chance that a grader will be unable to compile this. Please don't mark me down for this.








/*The following preprocessor commands are to determine the runtime environment for the PrintHex function.*/
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <stack>


using namespace std;

typedef class Node *NodeRef;
typedef class HexGraph *GraphRef;
enum hexColor {EMPTY, PLAYER, OPPONENT};
/*Main class*/
class HexGraph{
	vector<NodeRef> allNodes;
	vector<NodeRef> endNodes;
	const int size;
	enum hexColor Winner();
	bool CheckPath(enum hexColor user, NodeRef start, NodeRef end);
	void AddSentinelNode(int size, vector<NodeRef>);
	int PlayerGo();
	void AiGo();
	int BlockPath();
	void PieRule();
public:
	HexGraph(int);
	friend ostream& operator<<(ostream& os, const HexGraph& g);
	void InitNodes(Node& n);
	int GetSize(){return size;}
	void PrintHex();
	void GameStart();
	void PrintInstructions();
};

/*Graphs are made up of nodes*/
class Node{
	enum hexColor hexColor;
	int id;
	bool visited;
public:
	vector<NodeRef> nodeList;
	pair<int, int> location;
	Node(int);
	Node(int id, enum hexColor c);
	friend ostream& operator<<(ostream& os, const Node& n);
	int GetId(){return id;}
	bool Visited(){return visited;}
	void Visited(bool b){visited=b;}
	enum hexColor GetColor(){return hexColor;}
	void SetColor(enum hexColor c){hexColor = c;}
};

Node::Node(int id):id(id){
	hexColor = EMPTY;
	visited = false;
}

Node::Node(int id, enum hexColor c):id(id),hexColor(c){
	visited = false;
}

/*Initializes edgelist for each node*/
void HexGraph::InitNodes(Node& n){
	int i = n.location.first = n.GetId()/11;
	int j = n.location.second = n.GetId()%11;
		int botLeft = n.GetId()+size-1;
		int botRight = n.GetId()+size;
		int right= n.GetId()+1;
		int left = n.GetId()-1;
		int topLeft = n.GetId()-size;
		int topRight = n.GetId()-(size-1);

	/*corner cases*/
	if(i==0 && j==0){ /*top left*/
		n.nodeList.push_back(allNodes.at(1));		
		n.nodeList.push_back(allNodes.at(botRight));
	}

	else if(i==0 && j==size-1){ /*top right*/
		n.nodeList.push_back(allNodes.at(left));
		n.nodeList.push_back(allNodes.at(botLeft));
		n.nodeList.push_back(allNodes.at(botRight));
	}

	else if(i==size-1 && j==0){ /*bot left*/
		n.nodeList.push_back(allNodes.at(topLeft));		
		n.nodeList.push_back(allNodes.at(topRight));		
		n.nodeList.push_back(allNodes.at(right));		
	}

	else if(i==size-1 && j==size-1){ /*bot right*/
		n.nodeList.push_back(allNodes.at(topLeft));
		n.nodeList.push_back(allNodes.at(left));
	}

	/*edge cases*/
	else if(i==0){ /*top edge*/
		n.nodeList.push_back(allNodes.at(botLeft));
		n.nodeList.push_back(allNodes.at(botRight));
		n.nodeList.push_back(allNodes.at(right));
		n.nodeList.push_back(allNodes.at(left));
	}

	else if(j==0){ /*left edge*/
		n.nodeList.push_back(allNodes.at(topLeft));
		n.nodeList.push_back(allNodes.at(topRight));
		n.nodeList.push_back(allNodes.at(botRight));
		n.nodeList.push_back(allNodes.at(right));
	}

	else if(i==size-1){ /*bot edge*/
		n.nodeList.push_back(allNodes.at(left));
		n.nodeList.push_back(allNodes.at(topLeft));
		n.nodeList.push_back(allNodes.at(topRight));
		n.nodeList.push_back(allNodes.at(right));
	}

	else if(j==size-1){ /*right edge*/
		n.nodeList.push_back(allNodes.at(left));
		n.nodeList.push_back(allNodes.at(topLeft));
		n.nodeList.push_back(allNodes.at(botRight));
		n.nodeList.push_back(allNodes.at(botLeft));
	}

	/*not a corner or edge*/
	else{
		n.nodeList.push_back(allNodes.at(left));
		n.nodeList.push_back(allNodes.at(topLeft));
		n.nodeList.push_back(allNodes.at(topRight));
		n.nodeList.push_back(allNodes.at(right));
		n.nodeList.push_back(allNodes.at(botLeft));
		n.nodeList.push_back(allNodes.at(botRight));
	}
}

/*Adds special nodes to see for path detection*/
void HexGraph::AddSentinelNode(int size, vector<NodeRef> allNodes){
	NodeRef top = new Node(-1);
	NodeRef bot = new Node(-2);
	NodeRef left = new Node(-3);
	NodeRef right = new Node(-4);
	top->SetColor(PLAYER);
	bot->SetColor(PLAYER);
	left->SetColor(OPPONENT);
	right->SetColor(OPPONENT);
	endNodes.push_back(top);
	endNodes.push_back(bot);
	endNodes.push_back(left);
	endNodes.push_back(right);
	for(int i=0; i<size; i++){
		allNodes.at(i)->nodeList.push_back(top);
		top->nodeList.push_back(allNodes.at(i));
		allNodes.at(i*size)->nodeList.push_back(left);
		left->nodeList.push_back(allNodes.at(i*size));
		allNodes.at(i*size + size-1)->nodeList.push_back(right);
		right->nodeList.push_back(allNodes.at(i*size + size-1));
		allNodes.at(size*(size-1) + i)->nodeList.push_back(bot);
		bot->nodeList.push_back(allNodes.at(size*(size-1)+1));
	}


}

/*Create all nodes, connect them, and stick them in graph*/
HexGraph::HexGraph(int size):size(size){
	for(int i=0; i<size*size; i++){
		NodeRef n = new Node(i);
		allNodes.push_back(n);
	}
	for(int i=0; i<allNodes.size(); i++){
		InitNodes(*allNodes.at(i));
	}
	AddSentinelNode(size, allNodes);

}

/*Overloads cout for Nodes*/
ostream& operator<<(ostream& os, const Node& n){
	os<<n.id<<" ("<<n.location.first<<", "<<n.location.second<<") "<<n.hexColor<<endl;
	return os;
}

/*Overloads cout for Graphs*/
ostream& operator<<(ostream& os, const HexGraph& g){
	for(int i=0; i<g.allNodes.size(); i++){
		os<<g.allNodes.at(i);
	}
	return os;
}

/*Check if the path reaches the special node*/
bool HexGraph::CheckPath(enum hexColor user, NodeRef start, NodeRef end){
		stack<NodeRef> stack;
		stack.push(start);
	while(!stack.empty()){
		NodeRef current = stack.top();
		stack.pop();
		if(current->GetId()==end->GetId()){
			return true;
		}
		current->Visited(true);
		for(int edge = 0; edge<current->nodeList.size(); edge++){
			if(!current->nodeList.at(edge)->Visited() &&  current->nodeList.at(edge)->GetColor() == user){
				current->nodeList.at(edge)->Visited(true);
				stack.push(current->nodeList.at(edge));
			}
		}
	}
	for(int i=0; i<allNodes.size(); i++)
		allNodes.at(i)->Visited(false);
	for(int i=0; i<endNodes.size(); i++)
		endNodes.at(i)->Visited(false);
	return false;
}

/*AI move path finder*/
int HexGraph::BlockPath(){
	vector<NodeRef> v;
	vector<NodeRef> possibleBlocks;
	int result = -1;
	for(int i=0; i<allNodes.size(); i++){
		if(allNodes.at(i)->GetColor()==PLAYER){
			v.push_back(allNodes.at(i));
		}
	}
	int random = rand()%v.size();
	int randomId = v.at(random)->GetColor();
	NodeRef block = v.at(random);
	int blockIndex = block->GetId();
	if(block->GetId()+size < size*size && allNodes.at(blockIndex+size)->GetColor()==EMPTY)
		result= blockIndex+size;
	else if(block->GetId()-size >= 0 && allNodes.at(blockIndex-size)->GetColor()==EMPTY)
		result= blockIndex-size;
	return result;

}

/*Check paths to see if there is a winner*/
enum hexColor HexGraph::Winner(){
	if(CheckPath(PLAYER, endNodes.at(0), endNodes.at(1)))
		return PLAYER;
	if(CheckPath(OPPONENT, endNodes.at(2), endNodes.at(3)))
		return OPPONENT;
	return EMPTY;
}

/*Clears screen on various OSes to offer a better playing experience*/
void ClearScreen(){
	#ifdef OS_WIN
		system("CLS");		//blanks console screen
	#else
		system("clear");	//type clear if on UNIX
	#endif
}

/* Reads input and puts a piece in the specified hexagon. Returns the index of piece placed*/
int HexGraph::PlayerGo(){
	int i, j;
	cout<<"Your turn, enter your i coordinate and hit enter."<<endl;
	cin>>i;
	cout<<"Enter your j coordinate and hit enter."<<endl;
	cin>>j;
	if(i<0 || j<0 || i>GetSize()-1 || j>GetSize()-1){
		cout<<"("<<i<<" ,"<<j<<") is invalid. Try again."<<endl;
		cin.clear();		//clear and ignore flush input buffer. 
		cin.ignore();		//this way bad inputs such as ~ don't crash the program
		return PlayerGo();
	}
	int nodeLoc = i*(GetSize())+j;
	if(allNodes.at(nodeLoc)->GetColor()!=EMPTY){
		cout<<"Space is already occupied, please try again."<<endl;
		cin.clear();
		cin.ignore();
		return PlayerGo();
	}
	else{
		allNodes.at(nodeLoc)->SetColor(PLAYER);
	}
	return nodeLoc;
}

/*AI puts a piece in specified location*/
void HexGraph::AiGo(){
	int random = rand()%(size*size);
	int nodeLoc = BlockPath();
	int i = nodeLoc / size;
	int j = nodeLoc % size;
	if(nodeLoc == -1){
		if(allNodes.at(random)->GetColor()==EMPTY){
			allNodes.at(random)->SetColor(OPPONENT);
			 i = nodeLoc / size;
			 j = nodeLoc % size;
		}
		else{
			AiGo();
			return;
		}
	}
	else{
		allNodes.at(nodeLoc)->SetColor(OPPONENT);
		PrintHex();
		cout<<"Your opponent places a piece at ("<<i<<", "<<j<<")."<<endl;
	}
}

/*Indents used for PrintHex()*/
void Indent(int spaces){
	for(int i=0; i<spaces; i++)
		cout<<" ";
}



/*Prints the board*/
void HexGraph::PrintHex(){
	ClearScreen();
	PrintInstructions();
	cout<<endl;
	for(int i=0; i<GetSize(); i++){
		if(i!=GetSize()-1)
			cout<<i<<" ";
		else
			cout<<i;
		Indent(i);
		for(int j=0; j<GetSize(); j++){
			enum hexColor c = allNodes.at(i*(size)+j)->GetColor();
			if(c==EMPTY)
				cout<<" *";
			else if(c==PLAYER)
				cout<<" P";
			else
				cout<<" O";
		}
		cout<<endl;
	}
}

/*Lets the AI apply the pie rule*/
void HexGraph::PieRule(){
	cout<<"As the player, you get the first move. "<<endl<<"However, the opponenent may swap your first move with his."<<endl<<"This is called the pie rule."<<endl<<endl<<endl;;
	int index = PlayerGo();
	PrintHex();
	if(rand()%2 || index==size*size/2){
		allNodes.at(index)->SetColor(OPPONENT);
		PrintHex();
		cout<<"Your opponent has used the pie rule to take your spot."<<endl;
	}
	else{
		PrintHex();
		cout<<"Your opponent chose not to apply the pie rule."<<endl;
		AiGo();
	}
	//PrintHex();

}

/*Prints instructions*/
void HexGraph::PrintInstructions(){
	cout<<"Welcome to the game of hex player!"<<endl;
	cout<<"This board is "<<size<<" by "<<size<<", so the board is numbered from 0 to "<<size-1<<"."<<endl;
	cout<<"To win, make a path from the top to the bottom, or the bottom to the top."<<endl;
}

/*Main game method*/
void HexGraph::GameStart(){
	PrintInstructions();
	enum hexColor winner = EMPTY;
	PieRule();
	while(winner==EMPTY){
		PlayerGo();
		winner = Winner();
		if(winner!= EMPTY) break;
		PrintHex();
		AiGo();
		if(winner!=EMPTY) break;
		PrintHex();
		winner = Winner();
	}
	if(winner==PLAYER){
		PrintHex();
		cout<<"Game over, the winner is the player!"<<endl;
	}
	if(winner==OPPONENT){
		PrintHex();
		cout<<"Game over, the winner is the opponent!"<<endl;
	}
}

/*Entry point*/
int main(){
	srand(NULL);	//for pie rule
	GraphRef h = new HexGraph(11);
	h->GameStart();
}