#include <iostream>
#include <cmath>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;
//chance for each action
/*double R = 0.05;
double D = 0.9;
double S_pizza = 0.1; //stay at the same state while carrying a pizza
double D_pizza = 0.8;
double R_pizza = 0.05;
double reheat = 0.3;
double S_pizza = 0.3; //stay at the same state while coming out of a pizza shop
double D_pizza = 0.6;
double R_pizza = 0.05;*/
int row = 9;
int col = 15;
int EXPLORATION = 150;

class state
{
	public:
	int i,j;
	double up[4],down[4],left[4],right[4];	// q-values for the four actions
	double utility;
	char action; // U-up, D-down, L-left, R-right, N-not clear
	int U[4], D[4], L[4], R[4];// action counts
	state(int ii, int jj, char a)
	{
		i = ii;
		j = jj;
		for(int m=0; m<4; m++)
		{
			up[m] = 0;
			down[m] = 0;
			left[m] = 0;
			right[m] = 0;
			U[m] = 0;
			D[m] = 0;
			L[m] = 0;
			R[m] = 0;
		}
		action = a;
		utility = 0;
	}
};


void environment(vector<vector<int> > map, vector<vector<double> > & mapVal, vector<vector<vector<state *> > > & policy, state* current, char action, state* & next, double & curReward, double & reward, int & pizza, int & ingredients);
void trial(vector<vector<int> >map, vector<vector<double> > & mapVal, vector<vector<vector<state *> > > & policy);
bool converge(vector<vector<vector<state*> > > &policy, vector<vector<vector<state*> > > &update);
int findMaxQvalues(state* & s, char & action, int curpolicy);

void environment(vector<vector<int> > map, vector<vector<double> > & mapVal, vector<vector<vector<state *> > > & policy, state* current, char action, state* & next, double & curReward, double & reward, int & pizza, int & ingredients)
{
	int r = current->i;
	int c = current->j;
	int next_r = 0;
	int next_c = 0;
	srand(time(0));
	int chance = rand()%200;
	//0 is avaiable position, 1 is wall, 2 is grocery shop, 3 is pizza shop, 4 is student house
	curReward = mapVal[r][c];
	if (pizza == 1 && ingredients == 1)
	{
		if(map[r][c] == 0 || map[r][c] == 2)
		{
			if(action == 'U')
			{
				if(chance < 160) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4) //if it reaches a student
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'D')
			{
				if(chance < 160) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'L')
			{
				if(chance < 160)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'R')
			{
				if(chance < 160)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];
				}
			}
		}
		else if(map[r][c] == 3)
		{
			if(chance < 60) //reheat the pizza
			{
				next_r = r;
				next_c = c;
				reward = mapVal[r][c];
			}
			else
			{
				if(action == 'U')
				{
					if(chance < 144) //go up
					{
						if(r == 0 || map[r-1][c] == 1) //hit the wall
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];
						}
						else
						{
							next_r = r-1;
							next_c = c;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4) //if it reaches a student
								pizza = 0;
						}
					}
					else if(chance < 151) //going right
					{
						if(c == (col-1) || map[r][c+1] == 1)
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];						
						}
						else
						{
							next_r = r;
							next_c = c+1;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 193) //stay in the same state
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];	
					}
				}
				if(action == 'D')
				{
					if(chance < 144) //going down
					{
						if(r == (row-1) || map[r+1][c] == 1) //hit the wall
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];
						}
						else
						{
							next_r = r+1;
							next_c = c;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 151) //going right
					{
						if(c == 0 || map[r][c-1] == 1)
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];						
						}
						else
						{
							next_r = r;
							next_c = c-1;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 193) //stay in the same state
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];	
					}
				}
				if(action == 'L')
				{
					if(chance < 144)
					{
						if(c == 0 || map[r][c-1] == 1)
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];
						}
						else
						{
							next_r = r;
							next_c = c-1;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 151) //going right
					{
						if(r==0 || map[r-1][c] == 1)
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];						
						}
						else
						{
							next_r = r-1;
							next_c = c;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 193) //stay in the same state
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];	
					}
				}
				if(action == 'R')
				{
					if(chance < 144)
					{
						if(c == (col-1) || map[r][c+1] == 1)
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];
						}
						else
						{
							next_r = r;
							next_c = c-1;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 151) //going right
					{
						if(r == row-1 || map[r+1][c] == 1)
						{
							next_r = r;
							next_c = c;
							reward = mapVal[r][c];						
						}
						else
						{
							next_r = r+1;
							next_c = c;
							reward = mapVal[next_r][next_c];
							if(map[next_r][next_c] == 4)
								pizza = 0;
						}
					}
					else if(chance < 193) //stay in the same state
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
				}
			}
		}
		if(map[r][c] == 4)
		{
			pizza = 0;
			if(action == 'U')
			{
				if(chance < 180) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
					}
				}
				if(chance < 190) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
					}
				
				}
			}
			if(action == 'D')
			{
				if(chance < 180) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
					}
				}
				else if(chance < 190) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
					}
				}
			}
			if(action == 'L')
			{
				if(chance < 180)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
					}
				}
				else if(chance < 190) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
					}
				}
			}
			if(action == 'R')
			{
				if(chance < 180)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
					}
				}
				else if(chance < 190) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
					}
				}
			}
		}
	}
	else if(pizza == 0 && ingredients == 1)
	{
		if(map[r][c] == 0 || map[r][c] == 4 || map[r][c] == 2)
		{
			if(action == 'U')
			{
				if(chance < 180) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'D')
			{
				if(chance < 180) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'L')
			{
				if(chance < 180)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'R')
			{
				if(chance < 180)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
		}
		else if(map[r][c] == 3)
		{
			ingredients = 0;
			pizza = 1;
			if(action == 'U')
			{
				if(chance < 120) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4) //if it reaches a student
							reward = -0.1;
						}
					}
				else if(chance < 130) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 170) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'D')
			{
				if(chance < 120) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.01;
					}
				}
				else if(chance < 130) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.01;
					}
				}
				else if(chance < 170) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'L')
			{
				if(chance < 120)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.01;
					}
				}
				else if(chance < 130) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.01;
					}
				}
				else if(chance < 170) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'R')
			{
				if(chance < 120)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.01;
					}
				}
				else if(chance < 130) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.01;
					}
				}
				else if(chance < 170) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];
				}
			}
		}
	}
	else if(pizza == 1 && ingredients == 0)
	{
		if(map[r][c] == 0 || map[r][c] == 3)
		{
			if(action == 'U')
			{
				if(chance < 160) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4) //if it reaches a student
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'D')
			{
				if(chance < 160) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'L')
			{
				if(chance < 160)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'R')
			{
				if(chance < 160)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];
				}
			}
		}
		else if(map[r][c] == 2)
		{
			ingredients = 1;
			if(action == 'U')
			{
				if(chance < 160) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4) //if it reaches a student
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'D')
			{
				if(chance < 160) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'L')
			{
				if(chance < 160)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'R')
			{
				if(chance < 160)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 170) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];
				}
			}
		}
		else if(map[r][c] == 4)
		{
			pizza = 0;
			if(action == 'U')
			{
				if(chance < 160) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4) //if it reaches a student
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'D')
			{
				if(chance < 160) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'L')
			{
				if(chance < 160)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];	
				}
			}
			if(action == 'R')
			{
				if(chance < 160)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 170) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							pizza = 0;
						else if(map[next_r][next_c] == 2)
							ingredients = 1;
					}
				}
				else if(chance < 190) //stay in the same state
				{
					next_r = r;
					next_c = c;
					reward = mapVal[r][c];
				}
			}
		}
	}
	else if(pizza == 0 && ingredients == 0)
	{
		if(map[r][c] == 0 || map[r][c] == 4 || map[r][c] == 3)
		{
			if(action == 'U')
			{
				if(chance < 180) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}				
				}
				else if(chance < 190) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'D')
			{
				if(chance < 180) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'L')
			{
				if(chance < 180)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'R')
			{
				if(chance < 180)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];				
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
		}
		else if(map[r][c] == 2)
		{
			ingredients = 1;
			if(action == 'U')
			{
				if(chance < 180) //go up
				{
					if(r == 0 || map[r-1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}				
				}
				else if(chance < 190) //going right
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c+1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'D')
			{
				if(chance < 180) //going down
				{
					if(r == (row-1) || map[r+1][c] == 1) //hit the wall
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'L')
			{
				if(chance < 180)
				{
					if(c == 0 || map[r][c-1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(r==0 || map[r-1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];						
					}
					else
					{
						next_r = r-1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
			if(action == 'R')
			{
				if(chance < 180)
				{
					if(c == (col-1) || map[r][c+1] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];
					}
					else
					{
						next_r = r;
						next_c = c-1;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
				else if(chance < 190) //going right
				{
					if(r == row-1 || map[r+1][c] == 1)
					{
						next_r = r;
						next_c = c;
						reward = mapVal[r][c];				
					}
					else
					{
						next_r = r+1;
						next_c = c;
						reward = mapVal[next_r][next_c];
						if(map[next_r][next_c] == 4)
							reward = -0.1;
					}
				}
			}
		}
	}
	int curpolicy = -1;
	if(pizza == 0 && ingredients == 0)
		curpolicy = 0;
	else if(pizza == 1 && ingredients == 0)
		curpolicy = 1;
	else if(pizza == 0 && ingredients == 1)
		curpolicy = 2;
	else if(pizza == 1 && ingredients == 1)

	next = policy[curpolicy][next_r][next_c];
}

void trial(vector<vector<int> >map, vector<vector<double> > & mapVal, vector<vector<vector<state *> > > & policy)
{
	int count = 0;
	state* current;
	state* next;
	double reward;
	double nextreward;
	int pizza;
	int ingredients;
	vector<vector<vector<state*> > > update;
	for(int i=0; i<4; i++)
	{
		vector<vector<state*> > temp1;
		for(int j=0; j<row; j++)
		{
			vector<state*> temp2;
			for(int k=0; k<col; k++)
			{
				state* s = new state(j, k, 'N');
				temp2.push_back(s);
			}
			temp1.push_back(temp2);
		}
		update.push_back(temp1);
	}

	state* start = update[0][6][2];
	ingredients = 1;
	pizza = 0;

	do
	{
		int curpolicy = -1;
		if(pizza == 0 && ingredients == 0)
			curpolicy = 0;
		else if(pizza == 1 && ingredients == 0)
			curpolicy = 1;
		else if(pizza == 0 && ingredients == 1)
			curpolicy = 2;
		else if(pizza == 1 && ingredients == 1)

		current = start;
		next = start;
		count++;
		for(int l=0; l<4; l++)
		for(int k=0; k<4; k++)
		for(int i=0; i<row; i++)
		for(int j=0; j<col; j++)
		{
			policy[l][i][j]->up[k] = update[l][i][j]->up[k];
			policy[l][i][j]->down[k] = update[l][i][j]->down[k];
			policy[l][i][j]->left[k] = update[l][i][j]->left[k];
			policy[l][i][j]->right[k] = update[l][i][j]->right[k];
			policy[l][i][j]->U[k] = update[l][i][j]->U[k];
			policy[l][i][j]->D[k]= update[l][i][j]->D[k];
			policy[l][i][j]->L[k] = update[l][i][j]->L[k];
			policy[l][i][j]->R[k] = update[l][i][j]->R[k];
			policy[l][i][j]->action = update[l][i][j]->action;
			policy[l][i][j]->utility = update[l][i][j]->utility;
		}
		for(int terminal = 0; terminal<1000; terminal++)
		{
			if(pizza == 0 && ingredients == 0)
				curpolicy = 0;
			else if(pizza == 1 && ingredients == 0)
				curpolicy = 1;
			else if(pizza == 0 && ingredients == 1)
				curpolicy = 2;
			else if(pizza == 1 && ingredients == 1)
				current = next;

			char act;
			int actionTaken = findMaxQvalues(current, act, curpolicy);
			if(actionTaken < EXPLORATION)
			{
				int d = rand()%4;
				if(d == 0)
					act = 'U';
				else if(d == 1)
					act = 'D';
				else if(d == 2)
					act = 'L';
				else
					act = 'R';
			}
			current->action = act;
			environment(map, mapVal, policy, current, act, next, reward, nextreward, pizza, ingredients);
			double alpha = (double) 1000/(1000+count);
			if(pizza == 0 && ingredients == 0)
				curpolicy = 0;
			else if(pizza == 1 && ingredients == 0)
				curpolicy = 1;
			else if(pizza == 0 && ingredients == 1)
				curpolicy = 2;
			else if(pizza == 1 && ingredients == 1)		

			if(act == 'U')
			{
				current->up[curpolicy] = current->up[curpolicy] + alpha * (reward + next->utility - current->up[curpolicy]); // TD update equation
				current->U[curpolicy]++;
			}
			else if(act == 'D')
			{
				current->down[curpolicy] = current->down[curpolicy] + alpha*(reward + next->utility - current->down[curpolicy]); // TD update equation
				current->D[curpolicy]++;
			}
			else if(act =='L')
			{
				current->left[curpolicy] = current->left[curpolicy] + alpha*(reward + next->utility - current->left[curpolicy]); // TD update equation
				current->L[curpolicy]++;
			}
			else
			{
				current->right[curpolicy] = current->right[curpolicy] + alpha*(reward + next->utility - current->right[curpolicy]); // TD update equation
				current->R[curpolicy]++;
			}
		}
		double temp;
		start = update[0][6][2];
		ingredients = 1;
		pizza = 0;
	}while(!converge(policy, update));
	cout<<"count = "<<count<<endl;
	for(int i=0; i<4; i++)
	for(int j=0; j<row; j++)
	for(int k=0; k<col; k++)
		delete update[i][j][k];
}

bool converge(vector<vector<vector<state*> > > &policy, vector<vector<vector<state*> > > &update)
{
	for(int l=0; l<4; l++)
	for(int k=0; k<4; k++)
	for(int i=0; i<row; i++)
	for(int j=0; j<col; j++)
		if(abs(policy[k][i][j]->up[l] - update[k][i][j]->up[l]) > 0.0002 || abs(policy[k][i][j]->down[l] - update[k][i][j]->down[l]) > 0.0002 || abs(policy[k][i][j]->left[l] - update[k][i][j]->left[l]) > 0.0002 || abs(policy[k][i][j]->right[l] - update[k][i][j]->right[l]) > 0.0002)
			return false;
	return true;
}

int findMaxQvalues(state* & s, char & action, int curpolicy)
{
	double temp = s->up[curpolicy];
	int number = s->U[curpolicy];
	char act = 'U';
	if(temp < s->down[curpolicy])
	{
		temp = s->down[curpolicy];
		act = 'D';
		number = s->D[curpolicy];
	}
	if(temp < s->left[curpolicy])
	{
		temp = s->left[curpolicy];
		act = 'L';
		number = s->L[curpolicy];
	}
	if(temp < s->right[curpolicy])
	{
		temp = s->right[curpolicy];
		act = 'R';
		number = s->R[curpolicy];
	}
	action = act;
	s->utility = temp;
	return number;
}

int main()
{
	// initialize setting.
	// map is map
	// policy is policy
	vector<vector<double> > mapVal;
	vector<vector<int> > map;
	vector<vector<vector<state *> > > policy;
	for(int i=0; i<row;i++)
	{
		vector<double> temp1;
		vector<int> temp3;
		for(int j=0; j<col;j++)
		{	   
			temp1.push_back(-0.1);
			temp3.push_back(0);
		}
		mapVal.push_back(temp1);
		map.push_back(temp3);
	}
	for(int k=0; k<4; k++)
	{
		vector<vector<state*> > temp1;
		for(int i=0; i<row; i++)
		{
			vector<state*> temp2;
			for(int j=0; j<col; j++)
			{
				state* temp_s = new state(i, j, 'N');
				temp2.push_back(temp_s);
			}
			temp1.push_back(temp2);
		}
		policy.push_back(temp1);
	}
	//0 is avaiable position, 1 is wall, 2 is grocery shop, 3 is pizza shop, 4 is student house
	//set up the map
	for(int i=0; i<col; i++)
	{
		map[0][i] = 1;
		map[8][i] = 1;
		if(i<9)
		{
			map[i][0] = 1;
			map[i][14] = 1;
		}
	}
	map[4][1] = 1;
	map[4][2] = 1;
	map[4][3] = 1;
	map[4][4] = 1;
	map[3][4] = 1;
		
	map[4][10] = 1;
	map[4][11] = 1;
	map[4][12] = 1;
	map[4][13] = 1;
	map[3][10] = 1;
	map[5][10] = 1;

	map[6][2] = 2;
	map[6][12] = 2;
	
	map[1][9] = 3;
	map[7][6] = 3;
	map[7][7] = 3;

	map[2][2] = 4;
	map[2][12] = 4;

	mapVal[2][2] = 5;
	mapVal[2][12] = 5;
/**********************done map setting up************************************************/
	trial(map, mapVal, policy);
	for(int k=0; k<4; k++)
	{
		cout<<"case: "<<k<<endl;
		for(int i=0; i<row; i++)
		{
			for(int j=0; j<col; j++)
			{
				cout<<policy[k][i][j]->utility<<"   ";
			}
			cout<<endl;
		}
		cout<<endl;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<15; j++)
			{
				cout<<policy[k][i][j]->action<<"   ";
			}
			cout<<endl;
		}
	}

	for(int k=0; k<4; k++)
	for(int i=0; i<row; i++)
	for(int j=0; j<col; j++)
		delete policy[k][i][j];
	return 0;
}