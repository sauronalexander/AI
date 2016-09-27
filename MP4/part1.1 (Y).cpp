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
#include <algorithm>

using namespace std;
double DISCOUNT = 0.99;
int TERMINAL = 1;


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
              //  cout<< u<<endl;
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

double evaluation_function(double a, int b)
{
    if (b < 3)
        return 3;
    else
        return a;
}

void tdqlearning(vector<vector<double> > & mapVal,vector<vector<double> > & tdqu, int trials, vector<vector<char> > & tdqpolicy)
{
    double q[6][6][4]={0};
    int alpha_factor = 200;

    for(int i=0; i<6;i++)
        for(int j=0; j<6;j++)
        {

            if (mapVal[i][j] == 0)
                continue;

            int t = 0;
            int dp_table[6][6][4]={0};//similar to dynamic programming filling table
            for(int k = 0; k < trials; k++ )// can modify parameters
            {
                double alpha = (double)alpha_factor/(double)(alpha_factor+t);//alpha function
                t++;
                int goup =  evaluation_function(q[i][j][0], dp_table[i][j][0]);
                int godown =  evaluation_function(q[i][j][1], dp_table[i][j][1]);
                int goleft =  evaluation_function(q[i][j][2], dp_table[i][j][2]);
                int goright =  evaluation_function(q[i][j][3], dp_table[i][j][3]);

                int temp = goup;
                int action = 0;

                    if(temp <= godown)
                    {
                        action = 0;
                        tdqpolicy[i][j]= 'U';
                    }
                	if(temp < godown)
                    {
                        action = 1;
                        tdqpolicy[i][j]= 'D';
                    }
                    if(temp < goleft)
                    {
                        action = 2;
                        tdqpolicy[i][j]= 'L';
                    }
                    if(temp < goright)
                    {
                        action = 3;
                        tdqpolicy[i][j]= 'R';
                    }

                //int action = max({goup,godown,goleft,goright});
                dp_table[i][j][action]++;            
                int row,col;//successsor state

                if(action == 0 && i-1 >= 0 && i-1 <=5)
                {
                        if(mapVal[i-1][j]!=0)
                        {
                            row = i-1;
                            col = j;
                        }
                }

                else if(action == 1)
                {
                    if(i+1 >= 0 && i+1 <=5)
                    {
                        if(mapVal[i+1][j]!=0)
                        {
                            row = i+1;
                            col = j;
                        }
                    }
                }

                else if(action == 2)
                {
                    if(j-1 >= 0 && j-1 <=5)
                    {
                        if(mapVal[i][j-1]!=0)
                        {
                            row = i;
                            col = j-1;
                        }
                    }
                }

                else if(action == 3)
                {
                    if(j+1 >= 0 && j+1 <=5)
                    {
                        if(mapVal[i][j+1]!=0)
                        {
                            row = i;
                            col = j-1;
                        }
                    }
                }    
                else
                {
                    row=i;
                    col=j;
                }


                //float successsormax= max(q[row][col][0], q[row][col][1], q[row][col][2], q[row][col][3]);
                    double successsormax = q[row][col][0];
                	if(successsormax < q[row][col][1])
                        successsormax = q[row][col][1];
                    if(successsormax < q[row][col][2])
                        successsormax = q[row][col][2];
                    if(successsormax < q[row][col][3])
                        successsormax = q[row][col][3];

                q[i][j][action] = q[i][j][action] + alpha * (mapVal[row][col] + DISCOUNT * successsormax - q[i][j][action]);
               // cout << "q[i][j][action]"<<q[i][j][action] << endl;
            }
        }

    for (int i=0;i<6;i++)
        for(int j=0;j<6;j++)
        {
            //tdqu[i][j]= max(q[i][j][0], q[i][j][1], q[i][j][2], q[i][j][3]);
            tdqu[i][j]= q[i][j][0];
                if(tdqu[i][j] < q[i][j][1])
                    tdqu[i][j] = q[i][j][1];
                if(tdqu[i][j] < q[i][j][2])
                    tdqu[i][j] = q[i][j][2];
                if(tdqu[i][j] < q[i][j][3])
                    tdqu[i][j] = q[i][j][3];
        }

}


int main()
{
    // initialize setting.
    // map is map
    // policy is policy
    vector<vector<double> > mapVal;
    vector<vector<int> >map;
    vector<vector<state *> > policy;
    vector<vector<double> > tdq_utility;
    vector<vector<vector<double> > > tdq_u;
    vector<vector<char> > tdqpolicy;

    for(int i=0; i<6;i++)
    {
        vector<double> temp1;
        vector<state *> temp2;
        vector<int> temp3;
        vector<double> temp4;
        vector<char> temp5;
        for(int j=0; j<6;j++)
        {
            temp1.push_back(-0.04);
            state* s = new state(0, 'N');
            temp2.push_back(s);
            temp3.push_back(0);
            temp4.push_back(0);
            temp5.push_back('-');
        }
        mapVal.push_back(temp1);
        policy.push_back(temp2);
        map.push_back(temp3);
        tdq_utility.push_back(temp4);
        tdqpolicy.push_back(temp5);
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
cout << "here1" << endl;
    for (int trials = 1000; trials<=6000; trials+=1000)
    {//as a function of the number of trials (you will probably need several thousand)
        tdqlearning(mapVal,tdq_utility,trials,tdqpolicy);
        tdq_u.push_back(tdq_utility);
    }
cout << "here2" << endl;
double sum=0;
    for(int i =0; i< 5; i++)
    {
        sum=0;
        for(int j=0;j<6;j++)
        {
            for(int k=0;k<6;k++)
                if(mapVal[j][k]!= 0)
                {
                    sum += (tdq_u[i][j][k]- policy[j][k]->utility)*(tdq_u[i][j][k]-policy[j][k]->utility);
                    cout << "tdq_u[i][j][k]" << tdq_u[i][j][k] << endl;
                    //cout << "policy[j][k]->utility" << policy[j][k]->utility << endl;
                }
        }

    }
            sum /= 6*6;
            cout << sqrt(sum) << endl;

    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            cout<<tdqpolicy[i][j]<<"   ";
        }
        cout<<endl;
    }
    cout<<endl;

    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            cout<<tdq_u[5][i][j]<<"   ";
        }
        cout<<endl;
    }
    cout<<endl;

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
