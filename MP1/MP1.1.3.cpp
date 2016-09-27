
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

bool operator< (pair<Node*,int> & a,pair<Node*,int> & b)
{
	return a.second < b.second;
}

int A_star_detour(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj, int & numExp, map<Node*, Node*> & parent)
{
	//cout<<"pac"<<paci<<","<<pacj<<";"<<"end"<<endi<<","<<endj;
	vector <pair<Node*,int> > pq;		//use vector to implement function of priority queue
	int marker[row][col];
	std::map<Node*, int> dist;
	//std::map<Node*, Node*> parent;
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
		if(temp->col + 1 <= col && marker[temp->row][temp->col + 1]==0 && maze[temp->row][temp->col + 1]->type!='%' && maze[temp->row][temp->col + 1]->type!='g' && maze[temp->row][temp->col + 1]->type!='G')
		{
			marker[temp->row][temp->col + 1] =1;
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col + 1],dist[temp]+1));
			pq.push_back(pair<Node*,int>(maze[temp->row][temp->col + 1],maze[temp->row][temp->col + 1]->dist+ dist[maze[temp->row][temp->col + 1]]));
			
				parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col + 1],temp));
		}		
		if(temp->row-1 >= 0 && marker[temp->row-1][temp->col]==0 && maze[temp->row-1][temp->col]->type!='%' && maze[temp->row-1][temp->col]->type!='g' && maze[temp->row-1][temp->col]->type!='G')
		{
			marker[temp->row-1][temp->col] =1;
			dist.insert(pair<Node*,int>(maze[temp->row-1][temp->col],dist[temp]+1));
			pq.push_back(pair<Node*,int> (maze[temp->row-1][temp->col],maze[temp->row-1][temp->col]->dist+dist[maze[temp->row-1][temp->col]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row-1][temp->col],temp));
		}

		if(temp->col-1 >= 0 && marker[temp->row][temp->col-1]==0 && maze[temp->row][temp->col-1]->type!='%' && maze[temp->row][temp->col-1]->type!='g' && maze[temp->row][temp->col-1]->type!='G')
		{
			
			marker[temp->row][temp->col-1] =1;
			dist.insert(pair<Node*,int>(maze[temp->row][temp->col-1],dist[temp]+1));
			pq.push_back(pair<Node*,int> (maze[temp->row][temp->col-1],maze[temp->row][temp->col-1]->dist+dist[maze[temp->row][temp->col-1]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row][temp->col-1],temp));
		}
		if(temp->row + 1 <= row && marker[temp->row + 1][temp->col]==0 && maze[temp->row+1][temp->col]->type!='%' && maze[temp->row+1][temp->col]->type!='g' && maze[temp->row+1][temp->col]->type!='G')
		{
			
			marker[temp->row + 1][temp->col] =1;
			dist.insert(pair<Node*,int>(maze[temp->row + 1][temp->col],dist[temp]+1));
			pq.push_back(pair<Node*,int>(maze[temp->row + 1][temp->col],maze[temp->row + 1][temp->col]->dist+dist[maze[temp->row + 1][temp->col]]));
			
			parent.insert(pair<Node*,Node*>(maze[temp->row + 1][temp->col],temp));
		}
		
	}			// done greedy and proceed to path selection
	
	return dist[maze[endi][endj]];		
}

int A_star(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj, int & numExp, map<Node*, Node*> & parent, int & tempi, int & tempj, int & tempei, int & tempej, int & overlap, int & cost_0, bool flag)
{

	vector <pair<Node*,int> > pq;		//use vector to implement function of priority queue
	int marker[row][col];
	std::map<Node*, int> dist;
	dist.insert(pair<Node*,int>(maze[paci][pacj],0));
	parent.insert(pair<Node*, Node*>(maze[paci][pacj], NULL));
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			int dist = (maze[i][j]->row - endi) + (maze[i][j]->col-endj);
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
	
	Node * temp1 = maze[endi][endj];
	if(flag)
	{
		while(parent[temp1]->type !='g' && parent[temp1]->type != 'G')
		{
			temp1 = parent[temp1];
		}
		tempei = temp1->row;
		tempej = temp1->col;
	
		
		while(parent[temp1]->type !=' ')
		{
			temp1 = parent[temp1];
			overlap++;
		}
		tempi = parent[temp1]->row;
		tempj = parent[temp1]->col;
		cost_0 = dist[maze[tempi][tempj]];
		while(parent[temp1] != NULL)
		{
			temp1 = parent[temp1];
		}
	
	}
	return dist[maze[endi][endj]];		
}

bool pass_horizontal(vector<vector<Node*> > & maze, int tempi, int tempj, int cost, int trajectory, int iGi, int iGj, int igi, int igj, int direction_p, int overlap)
{

	int offset = iGj-igj;
	int position_i = igi, position_j = 0;
	int residue = (cost + offset)%((trajectory-1)*2);
	int direction_g = 0; //1 is right, -1 is left
	int endj = igi+trajectory-1;
	//cout<<"igi"<<iGi;
	if(residue >= (trajectory-1))
	{
		position_j = igj + (trajectory-1)*2 - residue;
		direction_g = -1;
	}
	else
	{
		position_j = igj + residue;
		direction_g = 1;
	}
	//cout<<"data"<<endl;
	while(overlap > 0)
	{
		//cout<<tempj<<","<<position_j<<";";
		if(position_j == tempj)
			return false;
		if(direction_p == 1 && direction_g == -1)
		{
			if(position_j == (tempj+1))
				return false;
		}
		else if(direction_p == -1 && direction_g == 1)
		{
			if(position_j == (tempj-1))
				return false;
		}

		overlap--;
		tempj += direction_p;

		//cout<<direction_g<<";";
		if(position_j == igj)
			direction_g =  1;
		else if(position_j == endj)
			direction_g = -1;
		//cout<<direction_g<<";";


		position_j += direction_g;
		//cout<<tempj<<","<<position_j<<endl;
	}
	return true;
}

bool pass_vertical(vector<vector<Node*> > & maze, int tempi, int tempj, int cost, int trajectory, int iGi, int iGj, int igi, int igj, int direction_p, int overlap)
{

	int offset = iGi-igi;
	int position_i = 0, position_j = igj;
	int residue = (cost + offset)%((trajectory-1)*2);
	int direction_g = 0; //1 is right, -1 is left
	int endi = igj+trajectory-1;
	//cout<<"igi"<<iGi;
	if(residue >= (trajectory-1))
	{
		position_i = igi + (trajectory-1)*2 - residue;
		direction_g = -1;
	}
	else
	{
		position_i = igi + residue;
		direction_g = 1;
	}
	//cout<<"data"<<endl;
	while(overlap > 0)
	{
		//cout<<tempj<<","<<position_j<<";";
		if(position_i == tempi)
			return false;
		if(direction_p == 1 && direction_g == -1)
		{
			if(position_i == (tempi+1))
				return false;
		}
		else if(direction_p == -1 && direction_g == 1)
		{
			if(position_i == (tempi-1))
				return false;
		}

		overlap--;
		tempi += direction_p;

		//cout<<direction_g<<";";
		if(position_i == igi)
			direction_g =  1;
		else if(position_i == endi)
			direction_g = -1;
		//cout<<direction_g<<";";


		position_i += direction_g;
		//cout<<tempj<<","<<position_j<<endl;
	}
	return true;
}

int Ghost(vector<vector<Node*> > & maze, int row, int col, int paci,int pacj, int endi,int endj, int & numExp, map<Node*, Node*> & parent, int trajectory, int iGi, int iGj, int igi, int igj, int cost_detour)
{
	std::map<Node*, Node*> parent_temp;
	int overlap = 0, tempi = 0, tempj = 0, cost_0 = 0, cost = 0, tempei, tempej;
	A_star(maze, row, col, paci, pacj,  endi, endj, numExp, parent, tempi, tempj, tempei, tempej, overlap, cost_0, true);
	cout<<tempi<<','<<tempj<<';'<<tempei<<','<<tempej<<';'<<overlap<<','<<trajectory<<";"<<cost_0<<endl;

	if(maze[igi][igj+1]->type == 'g' || maze[igi][igj+1]->type == 'G') // it is horizontal
	{
		cout<<"horizontal"<<endl;
		int direction_p = 0;	
		if(tempej<tempj)
			direction_p = -1;
		else
			direction_p = 1;
		
		//cout<<cost<<endl;
		for(cost = (cost_0+1); !pass_horizontal(maze, tempi, tempj, cost, trajectory, iGi, iGj, igi, igj, direction_p, overlap); cost += 2)
		{
			//cout<<cost<<endl;
			if(cost>(cost_detour==0? 100:cost_detour))
				return 0;
		}
	}
	else if(maze[igi+1][igj]->type == 'g' || maze[igi+1][igj]->type == 'G')
	{
		cout<<"vertical"<<endl;
		int direction_p = 0;	
		if(tempei<tempi)
			direction_p = -1;
		else
			direction_p = 1;
		
		//cout<<cost<<endl;
		for(cost = (cost_0+1); !pass_vertical(maze, tempi, tempj, cost, trajectory, iGi, iGj, igi, igj, direction_p, overlap); cost += 2)
		{
			//cout<<cost<<endl;
			if(cost>(cost_detour==0? 100:cost_detour))
				return 0;
		}
	}
	//cout<<cost<<endl;
	cost += overlap;	
	cost += A_star(maze, row, col, tempei, tempej, endi, endj, numExp, parent_temp, tempi, tempj, tempei, tempej, overlap, cost_0, false);
	return cost;
}

int main()
{

	int i=0 ,j=0;
	int paci, pacj;
	int endi,endj;
	char row, col;
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

	while(getline(myfile, line))
	{
		lines.push_back(line);
		line.clear();
	}
	
	row = lines.size();
	col = lines[0].length();
	vector<vector<Node*> > maze;
	int trajectory = 0;
	int iGi = 0, iGj = 0, igi = 0, igj = 0;
	bool flag = true;

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
			if(lines[i][j] == 'g' || lines[i][j] == 'G')
			{	
				trajectory++;
				if(flag)
				{
					igi = i;
					igj = j;
					flag = false;
				}
			}
			if(lines[i][j] == 'G')
			{
				iGi = i;
				iGj = j;
			}
			Node* temp1 = new Node(i,j,lines[i][j]);
			temp.push_back(temp1);
		}
		maze.push_back(temp);
	} 

	cout<<paci<<','<<pacj<<endl;
	int numExp1 = 0, numExp2 = 0;
	std::map<Node*, Node*> parent_detour;
	std::map<Node*, Node*> parent_ghost;
	int pathCost_detour = A_star_detour(maze, row, col, paci, pacj, endi, endj, numExp1, parent_detour);
	int pathCost_ghost = Ghost(maze, row, col, paci, pacj, endi, endj, numExp2, parent_ghost, trajectory, iGi, iGj, igi, igj, pathCost_detour);
	
	Node * temp1 = maze[endi][endj];
	if((pathCost_detour>pathCost_ghost && pathCost_ghost != 0) || pathCost_detour == 0)
	{
		while(parent_ghost[temp1]!= NULL)
		{
			temp1->type = '.';
			temp1 = parent_ghost[temp1];
		}
	}
	else
	{
		while(parent_detour[temp1]!= NULL)
		{
			temp1->type = '.';
			temp1 = parent_detour[temp1];
		}		
	}
	
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col;j++)
		{
			cout<<maze[i][j]->type;
		}
		cout <<endl;	
	}
	
	if((pathCost_detour>pathCost_ghost && pathCost_ghost != 0) || pathCost_detour == 0)
		cout<<"pathCost = "<<pathCost_ghost<<endl;
	else
		cout<<"pathCost(detour) = "<<pathCost_detour<<endl;
	
	cout<<"Nodes expanded =" << numExp1 + numExp2<<endl;

	return 0;

}
