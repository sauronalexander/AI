//cpp for cs440

#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <cmath>




using namespace std;

//typedef vector<pair<Node*,int> > vct;


class Node
{
	public:
	int row;
	int col;
	char type;
	int dist;	//square of the straight line distance between node and goal state.

	Node(int row1, int col1, char type1)
	{
		row = row1;
		col = col1;
		type = type1;

	}
	~Node(){}
}; 


bool operator< (pair<Node*,int> & a,pair<Node*,int> & b)
{
	return a.second < b.second;
}

int BFS(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj,int & numExp)
{

	queue<Node*> q;
	q.push(maze[paci][pacj]);
	std::map<Node*, int> dist;
	std::map<Node*, Node*> parent;
	int marker[row][col];
	dist.insert(pair<Node*,int>(maze[paci][pacj],0));
	parent.insert(pair<Node*, Node*>(maze[paci][pacj], NULL));
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			marker[i][j] = 0;
		}
	}
	
	marker[paci][pacj] =1;
	while(!q.empty() && q.front()->type!='.')
	{
		Node * temp = q.front();
		q.pop();
		numExp++;	
		if(temp->col-1 >= 0 && marker[temp->row][temp->col-1]==0 && maze[temp->row][temp->col-1]->type!='%')
		{
			
			marker[temp->row][temp->col-1] =1;
			q.push(maze[temp->row][temp->col-1]);
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col-1],dist[temp]+1));
			parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col-1],temp));
		}	
		if(temp->row-1 >= 0 && marker[temp->row-1][temp->col]==0 && maze[temp->row-1][temp->col]->type!='%')
		{
			marker[temp->row-1][temp->col] =1;
			q.push(maze[temp->row-1][temp->col]);
			dist.insert(pair<Node*,int>(maze[temp->row-1][temp->col],dist[temp]+1));
			parent.insert(pair<Node*,Node*>(maze[temp->row-1][temp->col],temp));
		}

		if(temp->row + 1 <= row && marker[temp->row + 1][temp->col]==0 && maze[temp->row+1][temp->col]->type!='%')
		{
			
			marker[temp->row + 1][temp->col] =1;
			q.push(maze[temp->row + 1][temp->col]);
			dist.insert(pair<Node*,int>(maze[temp->row + 1][temp->col],dist[temp]+1));
				parent.insert(pair<Node*,Node*>(maze[temp->row + 1][temp->col],temp));
		}
		if(temp->col + 1 <= col && marker[temp->row][temp->col + 1]==0 && maze[temp->row][temp->col + 1]->type!='%')
		{
			marker[temp->row][temp->col + 1] =1;
			q.push(maze[temp->row][temp->col + 1]);
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col + 1],dist[temp]+1));
				parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col + 1],temp));
		}		
	}			// done BFS and proceed to path selection
	

	Node * temp1 = maze[endi][endj];

	while(parent[temp1]!= NULL)
	{
		temp1->type = '.';
		temp1 = parent[temp1];
	}
	return dist[maze[endi][endj]];	
}


int DFS(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj,int & numExp)
{

	stack<Node*> s;
	s.push(maze[paci][pacj]);
	std::map<Node*, int> dist;
	std::map<Node*, Node*> parent;
	int marker[row][col];
	dist.insert(pair<Node*,int>(maze[paci][pacj],0));
	parent.insert(pair<Node*, Node*>(maze[paci][pacj], NULL));
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			marker[i][j] = 0;
		}
	}
	
	marker[paci][pacj] =1;
	while(!s.empty() && s.top()->type!='.')
	{
		Node * temp = s.top();
		s.pop();
		numExp++;	
	
		if(temp->col + 1 <= col && marker[temp->row][temp->col + 1]==0 && maze[temp->row][temp->col + 1]->type!='%')
		{
			marker[temp->row][temp->col + 1] =1;
			s.push(maze[temp->row][temp->col + 1]);
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col + 1],dist[temp]+1));
				parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col + 1],temp));
		}		
		if(temp->row-1 >= 0 && marker[temp->row-1][temp->col]==0 && maze[temp->row-1][temp->col]->type!='%')
		{
			marker[temp->row-1][temp->col] =1;
			s.push(maze[temp->row-1][temp->col]);
			dist.insert(pair<Node*,int>(maze[temp->row-1][temp->col],dist[temp]+1));
			parent.insert(pair<Node*,Node*>(maze[temp->row-1][temp->col],temp));
		}

		if(temp->col-1 >= 0 && marker[temp->row][temp->col-1]==0 && maze[temp->row][temp->col-1]->type!='%')
		{
			
			marker[temp->row][temp->col-1] =1;
			s.push(maze[temp->row][temp->col-1]);
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col-1],dist[temp]+1));
			parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col-1],temp));
		}
		if(temp->row + 1 <= row && marker[temp->row + 1][temp->col]==0 && maze[temp->row+1][temp->col]->type!='%')
		{
			
			marker[temp->row + 1][temp->col] =1;
			s.push(maze[temp->row + 1][temp->col]);
			dist.insert(pair<Node*,int>(maze[temp->row + 1][temp->col],dist[temp]+1));
				parent.insert(pair<Node*,Node*>(maze[temp->row + 1][temp->col],temp));
		}
		
	}			// done DFS and proceed to path selection
	

	Node * temp1 = maze[endi][endj];

	while(parent[temp1]!= NULL)
	{
		temp1->type = '.';
		temp1 = parent[temp1];
	}
	return dist[maze[endi][endj]];	
}

int Greedy(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj,int & numExp)
{
	vector <pair<Node*,int> > pq;		//use vector to implement function of priority queue
	int marker[row][col];
	std::map<Node*, int> dist;
	std::map<Node*, Node*> parent;
	dist.insert(pair<Node*,int>(maze[paci][pacj],0));
	parent.insert(pair<Node*, Node*>(maze[paci][pacj], NULL));
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			int dist = abs((maze[i][j]->row - endi))+abs((maze[i][j]->col-endj));
			maze[i][j]->dist = dist;
			marker[i][j] = 0;
		}
	}
	
	pq.push_back(pair<Node*,int>(maze[paci][pacj],maze[paci][pacj]->dist));
	marker[paci][pacj] =1;
	while(!pq.empty())
	{
		std::vector <pair<Node*,int> >::iterator it1=pq.begin();		
		for(std::vector <pair<Node*,int> > ::iterator it2=pq.begin();it2 !=pq.end();++it2)
		{
			if(it1->second >it2->second)
			{
				it1=it2;
			}
		}
		int direction;
		Node * temp = it1->first;
		/*Node * par = parent[temp];
		if(par->row-temp->row == 1)
			direction = 1;//up
		else if(par->row-temp->row == -1)
			direction = 2;//down
		else if(par->col-temp->col)
			direction = 3;//left
		else
			direction = 4;//right*/

		if(temp->type == '.')
			break;
		//cout<< temp->dist <<" ,";
		pq.erase(it1);
		numExp++;	
	
		if(temp->col + 1 <= col && marker[temp->row][temp->col + 1]==0 && maze[temp->row][temp->col + 1]->type!='%')
		{
			marker[temp->row][temp->col + 1] =1;
			pq.push_back(pair<Node*,int>(maze[temp->row][temp->col + 1],maze[temp->row][temp->col + 1]->dist));
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col + 1],dist[temp]+1));
				parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col + 1],temp));
		}		
		if(temp->row-1 >= 0 && marker[temp->row-1][temp->col]==0 && maze[temp->row-1][temp->col]->type!='%')
		{
			marker[temp->row-1][temp->col] =1;
			pq.push_back(pair<Node*,int> (maze[temp->row-1][temp->col],maze[temp->row-1][temp->col]->dist));
			dist.insert(pair<Node*,int>(maze[temp->row-1][temp->col],dist[temp]+1));
			parent.insert(pair<Node*,Node*>(maze[temp->row-1][temp->col],temp));
		}

		if(temp->col-1 >= 0 && marker[temp->row][temp->col-1]==0 && maze[temp->row][temp->col-1]->type!='%')
		{
			
			marker[temp->row][temp->col-1] =1;
			pq.push_back(pair<Node*,int> (maze[temp->row][temp->col-1],maze[temp->row][temp->col-1]->dist));
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col-1],dist[temp]+1));
			parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col-1],temp));
		}
		if(temp->row + 1 <= row && marker[temp->row + 1][temp->col]==0 && maze[temp->row+1][temp->col]->type!='%')
		{
			
			marker[temp->row + 1][temp->col] =1;
			pq.push_back(pair<Node*,int>(maze[temp->row + 1][temp->col],maze[temp->row + 1][temp->col]->dist));
			dist.insert(pair<Node*,int>(maze[temp->row + 1][temp->col],dist[temp]+1));
				parent.insert(pair<Node*,Node*>(maze[temp->row + 1][temp->col],temp));
		}
		
	}			// done greedy and proceed to path selection
	
	Node * temp1 = maze[endi][endj];

	while(parent[temp1]!= NULL)
	{
		temp1->type = '.';
		temp1 = parent[temp1];
	}
	return dist[maze[endi][endj]];		
}

int A_star(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj,int & numExp, int forward, int turn, int heruistic)
{
	vector <pair<Node*,int> > pq;		//use vector to implement function of priority queue
	int direction;
	int marker[row][col];
	std::map<Node*, int> dist;
	std::map<Node*, Node*> parent;
	dist.insert(pair<Node*,int>(maze[paci][pacj],0));
	parent.insert(pair<Node*, Node*>(maze[paci][pacj], NULL));

	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			int dist;
			if(heruistic == 2)
				dist = (maze[i][j]->row - endi)*(maze[i][j]->row - endi) + (maze[i][j]->col-endj)*(maze[i][j]->col-endj);
			else
				dist = abs((maze[i][j]->row - endi))+abs((maze[i][j]->col-endj));
			maze[i][j]->dist = dist;
			marker[i][j] = 0;
		}
	}
	
	pq.push_back(pair<Node*,int>(maze[paci][pacj],maze[paci][pacj]->dist));
	marker[paci][pacj] =1;
	while(!pq.empty())
	{


		std::vector <pair<Node*,int> >::iterator it1=pq.begin();		
		for(std::vector <pair<Node*,int> > ::iterator it2=pq.begin();it2 !=pq.end();++it2)
		{
			if(it1->second > it2->second)
			{
				it1=it2;
			}else if(it1->second == it2->second)
			{
				if(dist[it1->first]>dist[it2->first])
				it1 = it2;
			}				
		}

		//for MP1.2
		Node * temp = it1->first;
		Node * par = parent[temp];
		if(temp->type != 'P')
		{
			if(par->row-temp->row == 1)
				direction = 1;//up
			else if(par->row-temp->row == -1)
				direction = 2;//down
			else if(par->col-temp->col)
				direction = 3;//left
			else
				direction = 4;//right
		}


		if(temp->type == '.')
			break;
		pq.erase(it1);
		numExp++;	
		if(temp->col + 1 <= col && marker[temp->row][temp->col + 1]==0 && maze[temp->row][temp->col + 1]->type!='%') // going right
		{
			marker[temp->row][temp->col + 1] =1;

			dist.insert(pair<Node*,int>(maze[temp->row][temp->col + 1],direction==4?dist[temp]+forward:dist[temp]+turn));
			pq.push_back(pair<Node*,int>(maze[temp->row][temp->col + 1],maze[temp->row][temp->col + 1]->dist+ dist[maze[temp->row][temp->col + 1]]));
			
				parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col + 1],temp));
		}		
		if(temp->row-1 >= 0 && marker[temp->row-1][temp->col]==0 && maze[temp->row-1][temp->col]->type!='%')//up
		{
			marker[temp->row-1][temp->col] =1;
			dist.insert(pair<Node*,int>(maze[temp->row-1][temp->col],direction==1?dist[temp]+forward:dist[temp]+turn));
			pq.push_back(pair<Node*,int> (maze[temp->row-1][temp->col],maze[temp->row-1][temp->col]->dist+dist[maze[temp->row-1][temp->col]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row-1][temp->col],temp));
		}

		if(temp->col-1 >= 0 && marker[temp->row][temp->col-1]==0 && maze[temp->row][temp->col-1]->type!='%')//left
		{
			
			marker[temp->row][temp->col-1] =1;
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col-1],direction==3?dist[temp]+forward:dist[temp]+turn));
			pq.push_back(pair<Node*,int> (maze[temp->row][temp->col-1],maze[temp->row][temp->col-1]->dist+dist[maze[temp->row][temp->col-1]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col-1],temp));
		}
		if(temp->row + 1 <= row && marker[temp->row + 1][temp->col]==0 && maze[temp->row+1][temp->col]->type!='%')//down
		{
			
			marker[temp->row + 1][temp->col] =1;
			dist.insert(pair<Node*,int>(maze[temp->row + 1][temp->col],direction==2?dist[temp]+forward:dist[temp]+turn));
			pq.push_back(pair<Node*,int>(maze[temp->row + 1][temp->col],maze[temp->row + 1][temp->col]->dist+dist[maze[temp->row + 1][temp->col]]));
			
				parent.insert(pair<Node*,Node*>(maze[temp->row + 1][temp->col],temp));
		}
		
	}			// done greedy and proceed to path selection
	
	Node * temp1 = maze[endi][endj];

	while(parent[temp1]!= NULL)
	{
		temp1->type = '.';
		temp1 = parent[temp1];
	}
	return dist[maze[endi][endj]];		
}


int main()
{

	int i=0 ,j=0;
	int paci, pacj;
	int endi,endj;
	char row, col;
	int choice_case=-1, choice_algorism=0, heruistic=0;
    string mazename;
    
    cout<<"input file name: ";
    cin >> mazename;
    ifstream myfile(mazename.c_str());

	

	vector <string> lines;
	if(!myfile)
	{
	     cout << "File does not exist." << endl;
	}

	string line;
	int forward, turn;

	do
	{
		cout<<"(0) forward movement has cost 1 and any turn has cost 1"<<endl;
		cout<<"(1) forward movement has cost 2 and any turn has cost 1"<<endl;
		cout<<"(2) forward movement has cost 1 and any turn has cost 2"<<endl;
		cin>>choice_case;
		if(choice_case==0)
		{
			forward = 1;
			turn = 1;
		}
		else if(choice_case==1)
		{
			forward = 2;
			turn = 1;
		}
		else if(choice_case==2)
		{
			forward = 1;
			turn = 2;
		}
		else
			cout<<"invalid choice"<<endl;
	}while(choice_case != 1 && choice_case != 2 && choice_case != 0);

	do
	{
		cout<<"(1) Manhattan distance"<<endl;
		cout<<"(2) Euclidean distance"<<endl;
		cin>>heruistic;
		if(heruistic != 1 && heruistic != 2)
			cout<<"invalid choice"<<endl;
	}while(heruistic != 1 && heruistic != 2);


	while(getline(myfile, line))
	{
		lines.push_back(line);
		line.clear();
	}
	
	row = lines.size();
	col = lines[0].length();
	vector<vector<Node*> > maze;
		
	for(int i=0; i<row; i++)
	{
		vector<Node*> temp;
		for(int j=0; j<col; j++)
		{
			if(lines[i][j] == 'P')
			{
				paci = i;
				pacj = j;
			} 
			if(lines[i][j] == '.')
			{
				endi = i;
				endj = j;
			}
			Node* temp1 = new Node(i,j,lines[i][j]);
			temp.push_back(temp1);
		}
		maze.push_back(temp);
	} 
	int numExp = 0;
	int pathCost = 0;

	do
	{
		cout<<"(1) BFS"<<endl;
		cout<<"(2) DFS"<<endl;
		cout<<"(3) Greedy"<<endl;
		cout<<"(4) A_star"<<endl;		
		cin>>choice_algorism;
		switch(choice_algorism)
		{		
			case(1):
				pathCost = BFS(maze, row, col, paci, pacj, endi, endj, numExp);
				break;

			case(2):
				pathCost = DFS(maze, row, col, paci, pacj, endi, endj, numExp);
				break;
			
			case(3):
				pathCost = Greedy(maze, row, col, paci, pacj, endi, endj, numExp);
				break;

			case(4):
				pathCost = A_star(maze, row, col, paci, pacj, endi, endj, numExp, forward, turn, heruistic);
				break;

			default:
				cout<<"invalid choice"<<endl;
				break;
		}

	}while(choice_algorism!=1 && choice_algorism!=2 && choice_algorism!=3 && choice_algorism!=4);
	

	
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col;j++)
		{
			cout<<maze[i][j]->type;
		}
		cout <<endl;	
	}
	
	cout<<"pathCost = "<<pathCost<<endl;
	cout<<"Nodes expanded =" << numExp<<endl;

	return 0;

}




