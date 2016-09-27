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
#include <time.h>
#include <cstdlib>
#include<ctime>

using namespace std;
double DISCOUNT = 0.99;
int TERMINAL = 0;
int EXPLORATION = 150;



class state
{
    public:
    int i,j;
    double up,down,left,right,utility;	// q-values for the four actions
    char action; // U-up, D-down, L-left, R-right, N-not clear
    int U,D,L,R;// action counts
    state(int ii, int jj, double u,double d,double l,double r, char a)
    {
        i = ii;
        j = jj;
        up = u;
		down = d;
		left = l;
		right = r;
        action = a;
        U=0;
        D=0;
        L=0;
        R=0;
        utility = 0;
    }
};

void trial(vector<vector<int> >map, vector<vector<double> > & mapVal, vector<vector<state *> > & policy);
double maxUtility(vector<vector<int> >map,vector<vector<double> > & mapVal, vector<vector<state *> > & policy, int i, int j, char & action);
bool converge(vector<vector<state *> > & policy,vector<vector<state *> > & update);
bool environment(vector<vector<int> >map, vector<vector<double> > & mapVal,vector<vector<state *> > & policy,state* current, char action, state* & next, double & curReward, double & reward, int & call);
int findMaxQvalues(state* & s,char & action);


bool environment(vector<vector<int> >map, vector<vector<double> > & mapVal,vector<vector<state *> > & policy,state* current,char action, state* & next, double & curReward, double & reward, int & call)
{
    int i = current->i;
    int j = current->j;
    int chance = rand()%100;
	
    bool terminal = false;
    curReward = mapVal[i][j];
    if(action == 'U')
    {
        if(chance < 80) // going staight
        {
            if(current-> i == 0 || map[i-1][j] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i-1][j];
                reward = mapVal[i-1][j];
                if(map[i-1][j] == 1)
                    terminal = true;
            }
            return terminal;
        }else if(chance >=80 && chance <90) // going left
        {
            
            if(current-> j == 0 || map[i][j-1] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i][j-1];
                reward = mapVal[i][j-1];
                if(map[i][j-1] == 1)
                    terminal = true;
            }
            return terminal;
        }else   //going right
        {
            if(current-> j == 5 || map[i][j+1] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i][j+1];
                reward = mapVal[i][j+1];
                if(map[i][j+1] == 1)
                    terminal = true;
            }
            return terminal;
        }
    }else if(action =='D')
    {
        if(chance < 80) // going staight
        {
            if(current-> i == 5 || map[i+1][j] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i+1][j];
                reward = mapVal[i+1][j];
                if(map[i+1][j] == 1)
                    terminal = true;
            }
            return terminal;
        }else if(chance >=80 && chance <90) // going left
        {
            
            if(current-> j == 0 || map[i][j-1] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i][j-1];
                reward = mapVal[i][j-1];
                if(map[i][j-1] == 1)
                    terminal = true;
            }
            return terminal;
        }else   //going right
        {
            if(current-> j == 5 || map[i][j+1] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i][j+1];
                reward = mapVal[i][j+1];
                if(map[i][j+1] == 1)
                    terminal = true;
            }
            return terminal;
        }
    }else if(action =='L')
    {
        if(chance < 80) // going staight
        {
            if(current-> j == 0 || map[i][j-1] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i][j-1];
                reward = mapVal[i][j-1];
                if(map[i][j-1] == 1)
                    terminal = true;
            }
            return terminal;
        }else if(chance >=80 && chance <90) // going left
        {
            
            if(current-> i == 0 || map[i-1][j] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i-1][j];
                reward = mapVal[i-1][j];
                if(map[i-1][j] == 1)
                    terminal = true;
            }
            return terminal;
        }else   //going right
        {
            if(current-> i == 5 || map[i+1][j] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i+1][j];
                reward = mapVal[i+1][j];
                if(map[i+1][j] == 1)
                    terminal = true;
            }
            return terminal;
        }
    }else
    {
        if(chance < 80) // going staight
        {
            if(current-> j == 5 || map[i][j+1] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i][j+1];
                reward = mapVal[i][j+1];
                if(map[i][j+1] == 1)
                    terminal = true;
            }
            return terminal;
        }else if(chance >=80 && chance <90) // going left
        {
            
            if(current-> i == 0 || map[i-1][j] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i-1][j];
                reward = mapVal[i-1][j];
                if(map[i-1][j] == 1)
                    terminal = true;
            }
            return terminal;
        }else   //going right
        {
            if(current-> i == 5 || map[i+1][j] == 2)
            {
                next = policy[i][j];
                reward = mapVal[i][j];
            }
            else
            {
                next = policy[i+1][j];
                reward = mapVal[i+1][j];
                if(map[i+1][j] == 1)
                    terminal = true;
            }
            return terminal;
        }
    }
}

int findMaxQvalues(state* & s, char & action)
{
    double temp = s->up;
    int number =s->U;
    char act = 'U';
    if(temp < s->down)
    {
        temp = s->down;
        act = 'D';
        number = s->D;
    }
    if(temp <s->left)
    {
        temp = s->left;
        act = 'L';
        number = s->L;
    }
    if(temp <s->right)
    {
        temp = s->right;
        act = 'R';
        number = s->R;
    }
    action = act;
    s->utility = temp;
//    if(s->i == 3 && s->j == 1)
//       cout<< s->up<<" "<<s->down<<" "<<s->left<<" "<<s->right<<" "<<s->U<<" "<<s->D<<" "<<s->L<<" "<<s->R<<endl;
    return number;
}

void trial(vector<vector<int> >map, vector<vector<double> > & mapVal, vector<vector<state *> > & policy)
{

	double trueUtility[6][6]={{1.6643805096654937791811335046077147126197814941406,-1,1.8121276469407541220846269425237551331520080566406,1.8358641736049907677852388587780296802520751953125,  1.9095493273886601226507764295092783868312835693359,2.3478585151401980546381764725083485245704650878906},   
{2.0698916194143750502121292811352759599685668945312,2.1397385130775412598325146973365917801856994628906,2.2096238007982544893081922054989263415336608886719,0,-1,2.4827968923418426427929261990357190370559692382812},   
{2.1384232725349536785586224141297861933708190917969,2.2176269448233045800122908985940739512443542480469,2.2969637787087910929528788983589038252830505371094,0,2.7439060479321368823946158954640850424766540527344,3 },  
{2.1964151562415423768470645882189273834228515625,2.2903387152362890688550578488502651453018188476562,2.3864767496455567830082600266905501484870910644531,0,2.79704499145231011425494216382503509521484375,   2.9000082337939590004793899424839764833450317382812 },  
{2.1309736495677200629472736181924119591712951660156,   2.2301935558607306475664699974004179239273071289062,   2.47916571123354145811390480957925319671630859375,   2.6293449377894915208742077084025368094444274902344,   2.7130498679744685652792668406618759036064147949219,   2.8028838130159017261178178159752860665321350097656},   
{1,   -1,   2.0249585626878521260607612930471077561378479003906,   0,   -1,   -1}};  // utility obtained from 1.1

	ofstream data("data.txt", ofstream::out);

	int call = 1;
    int count = 0;
    state* current;
    state* next;
    double reward;
    double nextreward;
    vector<vector<state *> > update;
    for(int i=0; i<6; i++)
    {
        vector<state *> temp;
        for(int j=0; j<6; j++)
        {
           	state* s = new state(i,j,0,0,0,0,'N');
            temp.push_back(s);
        }
        update.push_back(temp);
    }
    state * start = update[3][1]; // set starting state
    
    do
	{
        current = start;
        next = start;
		count++; // number of trials
        // at begin of each trail, update policy
        for(int i=0; i<6; i++)
        {
            for(int j=0; j<6; j++)
            {
                policy[i][j]->up = update[i][j]->up;
                policy[i][j]->down = update[i][j]->down;
                policy[i][j]->left = update[i][j]->left;
                policy[i][j]->right = update[i][j]->right;
                policy[i][j]->U = update[i][j]->U;
                policy[i][j]->D = update[i][j]->D;
                policy[i][j]->L = update[i][j]->L;
                policy[i][j]->R = update[i][j]->R;
                policy[i][j]->action = update[i][j]->action;
                policy[i][j]->utility = update[i][j]->utility;
            }
        }
        // in one trial
        bool terminal = false;


        while(!terminal)
        {
            current = next;
            // generate action
            // exploration function
            char act;
            int actionTaken = findMaxQvalues(current,act);
			
            if(actionTaken < EXPLORATION)
            {
               int d = rand()%4;
                if(d == 0)
                    act = 'U';
                else if (d ==1)
                    act = 'D';
                else if(d == 2)
                    act = 'L';
                else
                    act = 'R';
/*                int temp = current->U;
                act ='U';
                if(temp >current->D)
                {
                    temp = current->D;
                    act = 'D';
                }
                if(temp >current->L)
                {
                    temp = current->L;
                    act = 'L';
                }
                if(temp >current->R)
                {
                    temp = current->R;
                    act = 'R';
                }*/
            } // after this, get the action to use in this state
//			cout<<act<<endl;
            current->action = act;
            /*********************************/
            terminal = environment(map, mapVal,update,current,act, next, reward,nextreward,call);
			call++;

           // cout<<next->i <<" "<<next->j<<" "<<nextreward<<" "<<reward<<endl;
//            char nonuse;
//            findMaxQvalues(next,nonuse);
            // update q values of the state
            double alpha = (double)60/(59 + count); // learning rate
			
            if(act == 'U')
            {
                current ->up = current->up + alpha*(reward+DISCOUNT*next->utility - current->up); // TD update equation
                current->U++;
            }
            else if(act == 'D')
            {
                current ->down = current->down + alpha*(reward+DISCOUNT*next->utility - current->down); // TD update equation
                current->D++;
            }
            else if(act =='L')
            {
                current ->left = current->left + alpha*(reward+DISCOUNT*next->utility - current->left); // TD update equation
                current->L++;
            }
            else
            {
                current ->right = current->right + alpha*(reward+DISCOUNT*next->utility - current->right); // TD update equation
                current->R++;
            }
            if(terminal)
            {
                next->utility = nextreward;
                next->up = nextreward;
                next->down = nextreward;
                next->left = nextreward;
                next->right = nextreward;
            }
/*			cout<<"action = "<<act<<"   ends at "<<next->i<<" "<<next->j<<endl;
			 for(int i=0; i<6; i++)
        {
            for(int j=0; j<6; j++)
            {
                cout<<update[i][j]->up<<"&"<<update[i][j]->down<<"&"<<update[i][j]->left<<"&"<<update[i][j]->right<<"      " ;
            }
            cout<<endl;
        }
        cout<<endl;	*/
        }
        
   /*     for(int i=0; i<6; i++)
        {
            for(int j=0; j<6; j++)
            {
      //          cout<<update[i][j]->up<<"&"<<update[i][j]->down<<"&"<<update[i][j]->left<<"&"<<update[i][j]->right ;
		//		cout<<update[i][j]->action <<update[i][j]->U<<"/"<<update[i][j]->D<<"/"<<update[i][j]->L<<"/"<<update[i][j]->R<<"   ";
		//		cout<<update[i][j]->utility<<update[i][j]->action<<"    ";
            }
            cout<<endl;
        }
        cout<<endl;*/


		double temp;	
		for(int i=0; i<6; i++)
			for(int j=0; j<6; j++)
			{
				temp += (update[i][j]->utility-trueUtility[i][j])*(update[i][j]->utility-trueUtility[i][j]);
			}
		temp = temp /36;
		temp = sqrt(temp);
		data<<temp<<endl;
		cout<<temp<<endl;


		// generate a random starting point after reaching terminal state
		int ii = rand()%6;
		int jj = rand()%6;
		while(map[ii][jj] != 0)
		{
			 ii = rand()%6;
			 jj = rand()%6;
		}
		start = update[ii][jj];

   }while(!converge(policy, update));
	cout<<"count = "<<count<<endl;
    
}

double maxUtility(vector<vector<int> >map,vector<vector<double> > & mapVal, vector<vector<state *> > & policy, int i, int j, char & action)
{
    return 0;
}

bool converge(vector<vector<state *> > & policy,vector<vector<state *> > & update)
{
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            if(abs(policy[i][j]->up - update[i][j]->up)>0.0002||abs(policy[i][j]->down != update[i][j]->down)>0.0002||abs(policy[i][j]->left != update[i][j]->left)>0.0002||abs(policy[i][j]->right != update[i][j]->right)>0.0002)
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
	srand(time(NULL));
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
            state* s = new state(i,j,0,0,0,0,'N');
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
//    state* current = policy[3][1];
//   state* next;
//    double reward,nextreward;
//    bool terminal = environment(map, mapVal,policy,current,'D', next,reward,nextreward);
//    cout<< reward<<endl;
//    cout<<terminal<<endl;
//    cout<<next->i<<" "<<next->j<<endl;
//    iterate(map,mapVal,policy);
    
    trial(map, mapVal, policy);
	

    
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
