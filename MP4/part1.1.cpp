//
//  part1.cpp
//  
//
//  Created by WANZHICHUN on 11/29/15.
//
//

#include <iostream>
#include <cmath>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;
double DISCOUNT = 0.99;
int TERMINAL = 0;


class state
{
	public:
	double utility;
	char action; // U-up, D-down, L-left, R-right, N-not clear
	state(double u, char a)
	{
		utility = u;
		action = a;
	}
};
void iterate(vector<vector<int> >map, vector<vector<double> > & mapVal, vector<vector<state *> > & policy);
double maxUtility(vector<vector<int> >map,vector<vector<double> > & mapVal, vector<vector<state *> > & policy, int i, int j, char & action);
bool converge(vector<vector<state *> > & policy,vector<vector<state *> > & update);


void iterate(vector<vector<int> >map, vector<vector<double> > & mapVal, vector<vector<state *> > & policy)
{
	
	int count = 0;
	vector<vector<state *> > update;
	for(int i=0; i<6; i++)
	{
		vector<state *> temp;
		for(int j=0; j<6; j++)
		{
			state * s = new state(0,'N');
			temp.push_back(s);
		}
		update.push_back(temp);
	}
	
	do
	{
		count++;
		for(int i=0; i<6; i++)
		{
			for(int j=0; j<6; j++)
			{
				policy[i][j]->utility = update[i][j]->utility;
				policy[i][j]->action = update[i][j]->action;
			}
		}

		for(int i=0; i<6; i++)
		{
			for(int j=0; j<6; j++)
			{
				char act;
				double u = maxUtility(map,mapVal, policy, i,j, act);
				//cout<< u<<endl;
				update[i][j]->utility = u*DISCOUNT + mapVal[i][j]; //equation
				update[i][j]->action = act;
			}
		}
	/*   for(int i=0; i<6; i++)
		{
			for(int j=0; j<6; j++)
			{
				cout<<update[i][j]->utility<<"   ";
			}
			cout<<endl;
		}
		cout<<endl;*/
	}while(!converge(policy, update));

	cout<< count<<endl;
	
	
}

double maxUtility(vector<vector<int> >map,vector<vector<double> > & mapVal, vector<vector<state *> > & policy, int i, int j, char & action)
{
	//map 0 is normal state, 1 is terminal state, 2 is wall
	if(TERMINAL == 1 && map[i][j] == 1)	//terminal state case
	{
		action = 'N';
		return 0;
	}
	if(map[i][j] == 2)		// if the state is a wall
	{
		action ='N';
		return 0;
	}
	double up1,down1,left1,right1,up,down,left,right;	//utilities in 4 directions
	//transition model, 0.8 chance going correctly, 0.1 chance going side way
	if(i==0 || map[i-1][j]==2)
		up1 = policy[i][j]->utility;
	else
		up1 = policy[i-1][j]->utility;
	
	if(i == 5 ||map[i+1][j] == 2)
		down1 = policy[i][j]->utility;
	else 
		down1 = policy[i+1][j]->utility;

	if(j == 0 || map[i][j-1] ==2)
		left1 = policy[i][j]->utility;
	else
		left1 = policy[i][j-1]->utility;
	
	if(j == 5 || map[i][j+1] ==2)
		right1 = policy[i][j]->utility;
	else
		right1 = policy[i][j+1]->utility;

	// get utilities in four actions
	
	up = 0.8* up1 + 0.1 *left1+ 0.1 * right1;
	down = 0.8 * down1 + 0.1 *left1+0.1* right1;
	left = 0.8 * left1 +0.1 * up1+ 0.1 * down1;
	right = 0.8 * right1 + 0.1* up1 +0.1 * down1;

	
	// choose maximum utilities;
	double temp = up;
	action = 'U';
	if(temp < down)
	{
		temp = down;
		action = 'D';
	}
	if(temp < left)
	{
		temp = left;
		action = 'L';
	}
	if(temp < right)
	{
		temp = right;
		action = 'R';
	}

	return temp;
}

bool converge(vector<vector<state *> > & policy,vector<vector<state *> > & update)
{
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(abs(policy[i][j]->utility - update[i][j]->utility) > 0.001)
				return false;
		}
	}
	return true;
}


int main()
{
	// initialize setting.
	// map is map
	// policy is policy
	vector<vector<double> > mapVal;
	vector<vector<int> >map;
	vector<vector<state *> > policy;
	for(int i=0; i<6;i++)
	{
		vector<double> temp1;
		vector<state *> temp2;
		vector<int> temp3;
		for(int j=0; j<6;j++)
		{
			temp1.push_back(-0.04);
			state* s = new state(0, 'N');
			temp2.push_back(s);
			temp3.push_back(0);
		}
		mapVal.push_back(temp1);
		policy.push_back(temp2);
		map.push_back(temp3);
	}
	mapVal[0][1] = -1;
	mapVal[1][3] = 0;
	mapVal[1][4] = -1;
	mapVal[2][3] = 0;
	mapVal[2][5] = 3;
	mapVal[3][3] = 0;
	mapVal[5][0] = 1;
	mapVal[5][1] = -1;
	mapVal[5][3] = 0;
	mapVal[5][4] = -1;
	mapVal[5][5] = -1;
	
	map[0][1] = 1;  // 1- terminal state  2- wall
	map[1][3] = 2;
	map[1][4] = 1;
	map[2][3] = 2;
	map[2][5] = 1;
	map[3][3] = 2;
	map[5][0] = 1;
	map[5][1] = 1;
	map[5][3] = 2;
	map[5][4] = 1;
	map[5][5] = 1;
	/*********************************done initializing map ***************/
/*  char action;
	double u = maxUtility(map,mapVal, policy, 0,0, action);
	cout << u <<endl;
	cout<<action <<endl;*/
	iterate(map,mapVal,policy);
	
	/****************************output************************************/
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			cout<<policy[i][j]->utility<<"   ";
		}
		cout<<endl;
	}
	cout<<endl;
	 
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			cout<<policy[i][j]->action<<"   ";
		}
		cout<<endl;
	}
	
	return 0;
}
