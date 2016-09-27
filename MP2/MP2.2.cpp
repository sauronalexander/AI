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

//define the ratio between strength and player's current score(k1) and ratio between strength and number of cells player's posseses
int const ratio = 1;
//attrition fraction
float const fraction = 0.5;
//1 is battle, 2 is duel
int const war = 1;

class cell
{
	public:
	int row;
	int col;
	int value;
	int occupy; // 0 means empty, 1 means maxplayer, 2 means minplayer
	float strength;
	
	cell(int row, int col, int value, int occupy)
	{
		this->row = row;
		this->col = col;
		this->value = value;
		this->occupy = occupy;
		strength = 0;
	}

	~cell(){}
};

void num_occupies(vector<vector<cell*> >board, int & occupy1, int & occupy2);
void findAvailableMove(vector<vector<cell*> > & board, vector<cell*> & available);
void gameplay(vector<vector<cell*> > & board, int player1, int player2, int player1depth, int player2depth, int & sum1, int & sum2);
int alphabeta(vector<vector<cell*> > board, int depth, bool maxplayer, int depthlimit, cell* bestmove, cell*& move, int alpha, int beta, int & numExp, int & sum1, int & sum2);
void checkBlitz(cell* move,vector<vector<cell*> > & board, bool maxplayer,vector<cell*> & blitzedcell);
void uncheckBlitz(vector<cell*> & blitzedcell);
int evaluation(vector<vector<cell*> > & board,int eva_player1,int eva_player2);

//find units that are unoccupied and put them into vector "avaiable"
void findAvailableMove(vector<vector<cell*> > & board, vector<cell*> & available)
{
	for(int i=0; i<6; i++)
	for(int j=0; j<6; j++)
		if(board[i][j]->occupy == 0)
			available.push_back(board[i][j]);
}

//calculate the number of units that player1 and player2 occupy separately
void num_occupies(vector<vector<cell*> >board, int & occupy1, int & occupy2)
{
	for(int i=0; i<6; i++)
	for(int j=0; j<6; j++)
	{
		if(board[i][j]->occupy == 1)
			occupy1++;
		else if(board[i][j]->occupy == 2)
			occupy2++;
	}
}

/*---------------------------------------------------------------main----------------------------------------------------------------------*/
/*---------------------------------------------------------------main----------------------------------------------------------------------*/
int main()
{
	ifstream file("Smolensk.txt");
	vector<string> lines;
	string line;
	
	while(getline(file,line))
	{
		lines.push_back(line);
		line.clear();
	}

	//read in the board configuration
	//and store the values into board
	vector<vector<cell*> > board;
	string temp;
	for(int i=0; i<6; i++)
	{	
		vector <cell*> row;
		int j=0, k=0;
		while(j<6)
		{
			temp = temp + lines[i][k];
			if(lines[i][k]>'9' ||lines[i][k]<'0')
			{
				int value;
				if(temp.length() == 3)
					value = ((int)temp[0]-48)*10 + ((int)temp[1]-48);
				else
					value =  ((int)temp[0]-48);
				cell* temp1 = new cell(i, j, value, 0);
				row.push_back(temp1);
				temp.clear();
				j++;
			}
			k++;
		}
		board.push_back(row);
	}

	//begin game play for alpha-beta vs alpha-beta
	//input is following:
	//board, player1 depth, player2 depth, score of player1, score of player2
	int sum1=0, sum2=0;
	gameplay(board, 3, 5, sum1, sum2);

	//print out final result
	for(int i =0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(board[i][j]->occupy == 1)
			{
					if(board[i][j]->value > 9)
						cout<<"\033[42m" << board[i][j]->value << "\33[0m"<<" "; 
					else
						cout<<"\033[42m" << board[i][j]->value<<" " << "\33[0m"<<" "; 
			}
			else if (board[i][j]->occupy == 2)
			{
					
					if(board[i][j]->value > 9)
						cout<<"\033[44m" << board[i][j]->value << "\33[0m"<<" "; 
					else
						cout<<"\033[44m" << board[i][j]->value<<" " << "\33[0m"<<" "; 
			}
			else
			{					
					if(board[i][j]->value > 9)
						cout<<board[i][j]->value <<" "; 
					else
						cout<<board[i][j]->value <<"  "; 
			}
		}
		cout<<endl;
	}

	//calculate the scores for each player
	/*for(int i =0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(board[i][j]->occupy == 1)
				sum1 = sum1 + board[i][j]->value;
			else
				sum2 = sum2 + board[i][j]->value;
		}
	}*/
	cout<<"player 1 score = "<<sum1<<endl;
	cout<<"player 2 score = "<<sum2<<endl;
	if(sum1 > sum2)
		cout<<"player 1 wins"<<endl;
	else
		cout<<"player 2 wins"<<endl;

	return 0; 
}
/*---------------------------------------------------------------end of main---------------------------------------------------------------*/
/*---------------------------------------------------------------end of main---------------------------------------------------------------*/

//play the game with corresponding player strategy and current sum
void gameplay(vector<vector<cell*> > & board, int player1depth, int player2depth, int & sum1, int & sum2)
{
	cell* bestmove; //bestmove for current player
	cell* tempmove; //
	vector<cell*> p1; //cells that player1 occupies
	vector<cell*> p2; //cells that player2 occupies

	int utility; //
	int alpha = -9999;
	int beta = 9999;
	int nodes1 =0;
	int nodes2 =0;
	int count1=0,count2=0;
	clock_t t11,t12,t21,t22;
	float second1=0,second2=0;

	//there will be 36 turns in total
	//player strategy: alpha-beta vs alpha-beta
	for(int i=0; i<36; i+=2)
	{
		//first get the number of points each player occupies
		int occupy1 = 0, occupy2 = 0;
		num_occupies(board, occupy1, occupy2);

		//player1 moves first
		++count1;
		occupy1++;
		t21=clock();
		//alpha_beta for player1
		//board, depth, whether it is maxplayer, depth limit, tempmove, final move, alpha, beta, number of nodes expanded, score of player1, score of player2, ratio
		alphabeta(board, 0, true, player1depth, tempmove, bestmove, alpha, beta, nodes1, sum1, sum2);
		t22=clock();
		second1 += ((float)t22-(float)t21) / CLOCKS_PER_SEC;
		cout.precision(6);
		cout << "This step takes: " << ((float)t22-(float)t21) / CLOCKS_PER_SEC<<"seconds"<<endl;
		cout << "Player1 current time spent: " << second2<<" seconds"<<endl;
		cout << "Player1 takes " << count2 << " steps"<<endl;

		//check whether a war will begin and who will win the war
		bestmove->occupy = 1;
		sum1 += bestmove->value;
		bestmove->strength = bestmove->value*ratio*sum1/occupy1;
		checkBlitz(bestmove,board,true,blitzedcell);
		blitzedcell.clear();

		//player2 moves next
		++count2;
		t21=clock();
		//alpha_beta for player1
		//board, depth, whether it is maxplayer, depth limit, tempmove, final move, alpha, beta, number of nodes expanded, score of player1, score of player2, ratio
		alphabeta(board,0, false, player2depth, tempmove, bestmove, alpha, beta,nodes2, sum1, sum2);
		t22=clock();
		second2+= ((float)t22-(float)t21) / CLOCKS_PER_SEC;
		cout.precision(6);
		cout << "This step takes: " << ((float)t22-(float)t21) / CLOCKS_PER_SEC<<"seconds"<<endl;
		cout << "Player2 current time spent: " << second2 <<" seconds"<<endl;
		cout << "Player2 takes " << count2 << " steps"<<endl;

		//check whether a war will begin and who will win the war
		bestmove->occupy = 2;
		sum2 += bestmove->value;
		bestmove->strength = bestmove->value*ratio*sum2/occupy2;
		checkBlitz(bestmove,board,false,blitzedcell);
		blitzedcell.clear();

		//print out result for each turn
		cout<<i<<endl;
		for(int j=0; j<6; j++)
		{
			for(int k=0; k<6; k++)
			{
				if(board[j][k]->occupy == 1)
				{
					if(board[j][k]->value > 9)
						cout<<"\033[42m" << board[j][k]->value << "\33[0m"<<" "; 
					else
						cout<<"\033[42m" << board[j][k]->value<<" " << "\33[0m"<<" "; 
				}
				else if (board[j][k]->occupy == 2)
				{	
					if(board[j][k]->value > 9)
						cout<<"\033[44m" << board[j][k]->value << "\33[0m"<<" "; 
					else
						cout<<"\033[44m" << board[j][k]->value<<" " << "\33[0m"<<" "; 
				}
				else
				{					
					if(board[j][k]->value > 9)
						cout<<board[j][k]->value <<" "; 
					else
						cout<<board[j][k]->value <<"  "; 
				}
			}
			cout<<endl;
		}


		//update the scores for each unit strength for attrition
		for(int j=0; j<6; j++)
		for(int k=0; k<6; k++)
			board[j][k]->strength /= fraction;

		//print out info
		cout<<endl;
		cout << "steps " << count1 << " " <<count2<<endl;
		cout<<" nodes   "<<nodes1<<"  "<<nodes2<<endl;
		cout << "Average number of nodes of Player 1 expanded per move: "<< (float)nodes1/count1 <<endl;
		cout << "Average number of nodes of Player 2 expanded per move: "<< (float)nodes2/count2 <<endl;
		cout << "Average amount of time of Player 1 to make a move: "<< (float)second1/count1 <<endl;
		cout << "Average amount of time of Player 2 to make a move: "<< (float)second2/count2 <<endl;
	}
}

int alphabeta(vector<vector<cell*> > board, int depth, bool maxplayer, int depthlimit, cell* bestmove, cell*& move, int alpha, int beta,int & numExp, int &sum1, int &sum2)
{
	//first find the avaiable units in the board
	vector<cell*> available;
	findAvailableMove(board, available);

	//if the depth reaches depth limit or all the units in the board is occupied, no move is needed
	if(depth == depthlimit || available.size()==0)
	{
		numExp++;
		return evaluation(board);	
	}
		
	//find the number of units that each player occupies
	int occupy1 = 0, occupy2 = 0;
	num_occupies(board, occupy1, occupy2);

	//calculate the best move for player1
	if(maxplayer)
	{
		int highest = -9999; // highest utility
		numExp++;
		occupy1++;

		while(!available.empty())
		{
			int i=0;
			int j=0;
			int jval;

			//record captured cells
			vector<cell*> blitzcells;

			//initialize ival(max evalution value)
			available[i]->occupy = 1;
			sum1 += avaiable[i]->value;
			avaiable[i]->strength = avaiable[i]->value*ratio*sum1/occupy1;
			//
			checkBlitz(available[i], board, maxplayer, blitzcells);
			int ival = evaluation(board);
			//clear current settings
			available[i]->occupy = 0;
			sum1 -= available[i]->value;
			avaiable[i]->strength = 0;
			uncheckBlitz(blitzcells);

			//find the max evalutaion value
			for(j = 0; j < available.size();j++)
			{
				available[j]->occupy = 1;
				sum1 += available[j]->value;
				avaiable[j]->strength = avaiable[j]->value*ratio*sum1/occupy1;
				checkBlitz(available[j], board, maxplayer,blitzcells);
				jval = evaluation(board);
				available[j]->occupy = 0;
				sum1 -= available[j]->value;
				avaiable[j]->strength = 0;
				uncheckBlitz(blitzcells);
				if(jval > ival)
				{
					i = j;
					ival = jval;
				}
			}
			cell* next = available[i];
			//erase the point that is occupied and set that point to the current setting
			available.erase(available.begin()+i);
			next-> occupy = 1;			
			sum1 += next->value;
			next->strength = next->value*ratio*sum1/occupy1;
			
			checkBlitz(next, board, maxplayer,blitzcells);
			int temp = alphabeta(board, depth+1, false, depthlimit, bestmove, move, alpha, beta, numExp, sum1, sum2);
			
			if(highest < temp)
			{
				bestmove = next;
				highest = temp;
			}
			next->occupy = 0;
			sum1 -= next->value;
			next->strength = 0;
			uncheckBlitz(blitzcells);

			if(beta <= highest)
				return highest;
			if(alpha < highest)
				alpha = highest;
		}
		move = bestmove;
		return highest;	
	}
	else
	{
		int lowest = 9999;
		numExp++;
		occupy2++;

		while(!available.empty())
		{
			int i=0;
			int j=0;
			int jval;

			//record captured cells
			vector<cell*> blitzcells;

			//initialize ival(min evalution value)
			available[i]->occupy = 2;
			sum2 += avaiable[i]->value;
			avaiable[i]->strength = avaiable[i]->value*ratio*sum2/occupy2;
			//
			checkBlitz(available[i], board, maxplayer,blitzcells);
			int ival = evaluation(board);
			//clear current settings
			available[i]->occupy = 0;
			sum2 -= available[i]->value;
			avaiable[i]->strength = 0;
			uncheckBlitz(blitzcells);

			//find the min evalutaion value
			for(j = 0; j < available.size();j++)
			{
				available[j]->occupy = 2;
				sum2 += available[j]->value;
				avaiable[j]->strength = avaiable[j]->value*ratio*sum2/occupy2;
				checkBlitz(available[j], board, maxplayer,blitzcells);
				jval = evaluation(board);
				available[j]->occupy = 0;
				sum2 -= available[j]->value;
				avaiable[j]->strength = 0;
				uncheckBlitz(blitzcells);
				if(jval < ival)
				{
					i = j;
					ival = jval;
				}
			}
			//erase the point that is occupied and set that point to the current setting
			cell* next = available[i];
			available.erase(available.begin()+i);

			next-> occupy = 2;
			sum2 += next->value;
			next->strength = next->value*ratio*sum2/occupy2;

			checkBlitz(next, board, maxplayer,blitzcells);
			int temp = alphabeta(board, depth+1, true,depthlimit,bestmove,move,alpha,beta, numExp);
			
			if(lowest > temp)
			{
				bestmove = next;
				lowest = temp;
			}		
			next->occupy = 0;
			sum2 -= next->value;
			sum2->strength = 0
			uncheckBlitz(blitzcells);
			if(lowest <= alpha)
				return lowest;
			if(beta > lowest)
				beta = lowest;
		}
		move = bestmove;
		return lowest;	
	}
}

//war
void checkBlitz(cell* move,vector<vector<cell*> > & board, bool maxplayer,vector<cell*> & blitzedcell)
{
	//battle
	if(war == 1)
	{
		int i = move->row;
		int j = move->col;
		if()
	}
	/*int i= move->row;
	int j = move->col;
	if(maxplayer)
	{
		if((i>0&&board[i-1][j]->occupy == 1) ||(j>0&&board[i][j-1]->occupy == 1)||(i<5&&board[i+1][j]->occupy == 1)||(j<5&&board[i][j+1]->occupy == 1))
		{
			if(i>0 && board[i-1][j]->occupy == 2)
			{
				board[i-1][j]->occupy = 1;
				blitzedcell.push_back(board[i-1][j]);
			}
			if(j>0 && board[i][j-1]->occupy == 2)
			{
				board[i][j-1]->occupy = 1;
				blitzedcell.push_back(board[i][j-1]);
			}
			if(i<5 && board[i+1][j]->occupy == 2)
			{
				board[i+1][j]->occupy = 1;
				blitzedcell.push_back(board[i+1][j]);
			}
			if(j<5 && board[i][j+1]->occupy == 2)
			{
				board[i][j+1]->occupy = 1;
				blitzedcell.push_back(board[i][j+1]);
			}
		}
	}
	else
	{
		if((i>0&&board[i-1][j]->occupy == 2) ||(j>0&&board[i][j-1]->occupy == 2)||(i<5&&board[i+1][j]->occupy == 2)||(j<5&&board[i][j+1]->occupy == 2))
		{
			if(i>0 &&board[i-1][j]->occupy == 1)
			{
				board[i-1][j]->occupy = 2;
				blitzedcell.push_back(board[i-1][j]);
			}
			if(j>0 && board[i][j-1]->occupy == 1)
			{
				board[i][j-1]->occupy = 2;
				blitzedcell.push_back(board[i][j-1]);
			}
			if(i<5 && board[i+1][j]->occupy == 1)
			{
				board[i+1][j]->occupy = 2;
				blitzedcell.push_back(board[i+1][j]);
			}
			if(j<5 && board[i][j+1]->occupy == 1)
			{
				board[i][j+1]->occupy = 2;
				blitzedcell.push_back(board[i][j+1]);
			}
		}
	}*/
}

//restore the version(max min) before checkBlitz function
void uncheckBlitz(vector<cell*> & blitzedcell)
{
	while(!blitzedcell.empty())
	{
		cell* temp = blitzedcell.back();
		if(temp->occupy == 1)
			temp->occupy = 2;
		else
			temp->occupy =1;
		blitzedcell.pop_back();
	}
}

//evaluation function
int evaluation(vector<vector<cell*> > & board,int eva_player1,int eva_player2)
{
	return 0;
}
