
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <cmath>
#include <time.h>

using namespace std;

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

class Goal
{
	public:
	int row;
	int col;
	int current_cost;

	Goal(int row1, int col1, int current_cost1)
	{
		row = row1;
		col = col1;
		current_cost = current_cost1;

	}
	~Goal(){}
};

bool operator< (pair<Node*,int> & a,pair<Node*,int> & b)
{
	return a.second < b.second;
}

int A_star(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj, int & numExp, map<Node*, Node*> & parent)
{
	//cout<<"pac"<<paci<<","<<pacj<<";"<<"end"<<endi<<","<<endj;
	vector <pair<Node*,int> > pq;		//use vector to implement function of priority queue
	int marker[row][col];
	std::map<Node*, int> dist;
//	std::map<Node*, Node*> parent;
	dist.insert(pair<Node*,int>(maze[paci][pacj],0));
	parent.insert(pair<Node*, Node*>(maze[paci][pacj], NULL));
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			//int dist = abs((maze[i][j]->row - endi))+abs((maze[i][j]->col-endj));
			int dist = (maze[i][j]->row - endi)+ (maze[i][j]->col-endj);
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
			}
			else if(it1->second == it2->second)
			{
				if(dist[it1->first]>dist[it2->first])
				it1 = it2;
			}				
		}
		Node * temp = it1->first;
		pq.erase(it1);
		numExp++;	
		if(temp->col + 1 <= col && marker[temp->row][temp->col + 1]==0 && maze[temp->row][temp->col + 1]->type!='%')
		{
			marker[temp->row][temp->col + 1] =1;
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col + 1],dist[temp]+1));
			pq.push_back(pair<Node*,int>(maze[temp->row][temp->col + 1],maze[temp->row][temp->col + 1]->dist+ dist[maze[temp->row][temp->col + 1]]));
			
				parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col + 1],temp));
		}		
		if(temp->row-1 >= 0 && marker[temp->row-1][temp->col]==0 && maze[temp->row-1][temp->col]->type!='%')
		{
			marker[temp->row-1][temp->col] =1;
			dist.insert(pair<Node*,int>(maze[temp->row-1][temp->col],dist[temp]+1));
			pq.push_back(pair<Node*,int> (maze[temp->row-1][temp->col],maze[temp->row-1][temp->col]->dist+dist[maze[temp->row-1][temp->col]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row-1][temp->col],temp));
		}

		if(temp->col-1 >= 0 && marker[temp->row][temp->col-1]==0 && maze[temp->row][temp->col-1]->type!='%')
		{
			
			marker[temp->row][temp->col-1] =1;
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col-1],dist[temp]+1));
			pq.push_back(pair<Node*,int> (maze[temp->row][temp->col-1],maze[temp->row][temp->col-1]->dist+dist[maze[temp->row][temp->col-1]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col-1],temp));
		}
		if(temp->row + 1 <= row && marker[temp->row + 1][temp->col]==0 && maze[temp->row+1][temp->col]->type!='%')
		{
			
			marker[temp->row + 1][temp->col] =1;
			dist.insert(pair<Node*,int>(maze[temp->row + 1][temp->col],dist[temp]+1));
			pq.push_back(pair<Node*,int>(maze[temp->row + 1][temp->col],maze[temp->row + 1][temp->col]->dist+dist[maze[temp->row + 1][temp->col]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row + 1][temp->col],temp));
		}
		
	}			// done greedy and proceed to path selection
	
	/*Node * temp1 = maze[endi][endj];
	while(parent[temp1]!= NULL)
	{
		temp1->type = '.';
		temp1 = parent[temp1];
	}*/
	return dist[maze[endi][endj]];		
}

int multiple(vector<vector<Node*> > & maze, vector<Goal*> & goal, int row, int col, int paci,int pacj, int & numExp)
{
	vector <pair<Goal*,int> > pq;		//use vector to implement function of priority queue
	vector <map<Node*, Node*> > path;
	int num_goal;
	map<Node*, Node*> temp_path;
	vector <Node* > goallocation;
	int total_cost = 0;

	while(!goal.empty())
	{
		num_goal = goal.size();
		int temp=0;
		for(int i=0; i<num_goal; i++)
		{
			int cost = A_star(maze, row, col, paci, pacj, goal[i]->row, goal[i]->col, numExp, temp_path);
			//cout<<cost<<",";
			goal[i]->current_cost = cost;
			if(cost < goal[temp]->current_cost)
			{
				temp = i;
			}
		}
		//cout<<goal[temp]->row<<","<<goal[temp]->col<<endl;
		total_cost += goal[temp]->current_cost;
		A_star(maze, row, col, paci, pacj, goal[temp]->row, goal[temp]->col, numExp, temp_path);
		path.push_back(temp_path);
		goallocation.push_back(maze[goal[temp]->row][goal[temp]->col]);
		paci = goal[temp]->row;
		pacj = goal[temp]->col;
		goal.erase(goal.begin()+temp);
	}
	while(!path.empty())
	{
		Node * temp1 = goallocation.back();
		map<Node*, Node*> parent = path.back();

		while(parent[temp1]!= NULL)
		{
			temp1->type = '?';
			temp1 = parent[temp1];
		}
		path.pop_back();
		goallocation.pop_back();
	}

	return total_cost;
}


int main()
{
	clock_t t1,t2;
 

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
	//int forward, turn;




	while(getline(myfile, line))
	{
		lines.push_back(line);
		line.clear();
	}
	
	row = lines.size();
	col = lines[0].length();
	vector<vector<Node*> > maze;
	vector<Goal*> goal;
		
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
				//cout<<i<<","<<j<<endl;
				Goal* temp2 = new Goal(i,j,0);
				goal.push_back(temp2);
			}
			Node* temp1 = new Node(i,j,lines[i][j]);
			temp.push_back(temp1);
		}
		maze.push_back(temp);
	} 
	int numExp = 0;
	t1=clock();
	int pathCost = multiple(maze, goal, row, col, paci, pacj, numExp);
	t2=clock();
	
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


    float diff ((float)t2-(float)t1);
 
	float seconds = diff / CLOCKS_PER_SEC;

    cout<<seconds<<endl;
    return 0;

}
