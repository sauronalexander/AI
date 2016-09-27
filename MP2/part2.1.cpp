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
class cell
{
	public:
	int row;
	int col;
	int value;
	int occupy; // 0 means empty, 1 means maxplayer, 2 means minplayer

	
	cell(int row1, int col1, int value1, int occupy1)
	{
		row = row1;
		col = col1;
		value = value1;
		occupy = occupy1;

	}
};


int minimax(vector<vector<cell*> > board, int depth, bool maxplayer, int depthlimit, cell* bestmove, cell* & move, int & numExp);
void checkBlitz(cell* move,vector<vector<cell*> > & board, bool maxplayer, vector<cell*> & blitzedcell);
void uncheckBlitz(vector<cell*> & blitzedcell);
void findAvailableMove(vector<vector<cell*> > & board, vector<cell*> & available);
int evaluation(vector<vector<cell*> > & board,int eva_player1,int eva_player2);
void gameplay(vector<vector<cell*> > & board, int player1, int player2, int player1depth, int player2depth);
int alphabeta(vector<vector<cell*> > board, int depth, bool maxplayer, int depthlimit, cell* bestmove, cell*& move, int alpha, int beta,int & numExp );

int minimax(vector<vector<cell*> > board, int depth, bool maxplayer, int depthlimit, cell* bestmove, cell*& move, int & numExp)
{
	vector<cell*> available;
	findAvailableMove(board, available);
	if(depth == depthlimit || available.size()==0)
	{
		numExp++;
		return evaluation(board);	
	}

	if(maxplayer)
	{
		int highest = -9999; // highest utility
		numExp++;
		for(int i=0; i<available.size(); i++)
		{
			
			available[i]-> occupy = 1;
			vector<cell*> blitzcells;
			checkBlitz(available[i], board, maxplayer,blitzcells);
			int temp = minimax(board, depth+1, false,depthlimit,bestmove,move, numExp);
			if(highest < temp)
			{
				bestmove = available[i];
				highest = temp;
			}

			available[i]->occupy = 0;
			uncheckBlitz(blitzcells);
		}
		move = bestmove;
		return highest;	
	}
	else
	{
		int lowest = 9999;
		numExp++;
		for(int i=0; i<available.size(); i++)
		{
			available[i]-> occupy = 2;
			vector<cell*> blitzcells;
			checkBlitz(available[i], board, maxplayer,blitzcells);
			int temp = minimax(board, depth+1, true,depthlimit,bestmove,move, numExp);
			
			if(lowest > temp)
			{
				bestmove = available[i];
				lowest = temp;
			}		

			available[i]->occupy = 0;
			uncheckBlitz(blitzcells);
		}
		move = bestmove;
		return lowest;	
	}
}

int alphabeta(vector<vector<cell*> > board, int depth, bool maxplayer, int depthlimit, cell* bestmove, cell*& move, int alpha, int beta,int & numExp)
{
	vector<cell*> available;
	findAvailableMove(board, available);
	if(depth == depthlimit || available.size()==0)
	{
		numExp++;
		return evaluation(board);	
	}

	if(maxplayer)
	{
		int highest = -9999; // highest utility
		numExp++;
		
		while(!available.empty())
		{
			int i=0;
			int j=0;
			int jval;
			vector<cell*> blitzcells;			
				
			available[i]->occupy = 1;
			checkBlitz(available[i], board, maxplayer,blitzcells);
			int ival = evaluation(board);
			available[i]->occupy = 0;
			uncheckBlitz(blitzcells);
			for(j = 0; j < available.size();j++)
			{
				available[j]->occupy = 1;
				checkBlitz(available[j], board, maxplayer,blitzcells);
				jval = evaluation(board);
				available[j]->occupy = 0;
				uncheckBlitz(blitzcells);
				if(jval > ival)
				{
					i = j;
					ival = jval;
				}
			}
			cell* next = available[i];
			available.erase(available.begin()+i);

			
			next-> occupy = 1;
			
			checkBlitz(next, board, maxplayer,blitzcells);
			int temp = alphabeta(board, depth+1, false,depthlimit,bestmove,move,alpha,beta, numExp);
			
			if(highest < temp)
			{
				bestmove = next;
				highest = temp;
			}
			next->occupy = 0;
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

		while(!available.empty())
		{
			int i=0;
			int j=0;
			int jval;
			vector<cell*> blitzcells;			
				
			available[i]->occupy = 2;
			checkBlitz(available[i], board, maxplayer,blitzcells);
			int ival = evaluation(board);
			available[i]->occupy = 0;
			uncheckBlitz(blitzcells);
			for(j = 0; j < available.size();j++)
			{
				available[j]->occupy = 2;
				checkBlitz(available[j], board, maxplayer,blitzcells);
				jval = evaluation(board);
				available[j]->occupy = 0;
				uncheckBlitz(blitzcells);
				if(jval < ival)
				{
					i = j;
					ival = jval;
				}
			}
			cell* next = available[i];
			available.erase(available.begin()+i);

			next-> occupy = 2;
			checkBlitz(next, board, maxplayer,blitzcells);
			int temp = alphabeta(board, depth+1, true,depthlimit,bestmove,move,alpha,beta, numExp);
			
			if(lowest > temp)
			{
				bestmove = next;
				lowest = temp;
			}		
			next->occupy = 0;
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


void checkBlitz(cell* move,vector<vector<cell*> > & board, bool maxplayer,vector<cell*> & blitzedcell)
{
	int i= move->row;
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

	}	
}
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

void findAvailableMove(vector<vector<cell*> > & board, vector<cell*> & available)
{
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(board[i][j]->occupy == 0)
				available.push_back(board[i][j]);
		}
	}
}

int evaluation(vector<vector<cell*> > & board,int eva_player1,int eva_player2)
{
	int sum=0;//The node that is 2 unit distance away(mangattan) from current enemy node should not to be placed
	//sum is for player 1, MAX player
	//occupy: 0 means empty, 1 means maxplayer, 2 means minplayer
	for(int i=0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(board[i][j]->occupy == 1 && eva_player==1)
			{
				sum += board[i][j]->value;

				if(i-2>=0 && board[i-2][j]->occupy == 0)
				{
					sum -= board[i-2][j]->value;
					if(board[i-1][j]->occupy == 0)
						sum -= board[i-1][j]->value;
				}
				if(i+2<6&& board[i+2][j]->occupy == 0)
				{
					sum-= board[i+2][j]->value;
					if(board[i+1][j]->occupy == 0)
						sum-= board[i+1][j]->value;
				}
				if(j-2>=0&& board[i][j-2]->occupy == 0)
				{
					sum-= board[i][j-2]->value;
					if(board[i][j-1]->occupy == 0)
						sum-= board[i][j-1]->value;
				}
				if(j+2<6&& board[i][j+2]->occupy == 0)
				{
					sum-= board[i][j+2]->value;
					if(board[i][j+1]->occupy == 0)
						sum-= board[i][j+1]->value;
				}
				if(i-1>=0 && j-1>=0&& board[i-1][j-1]->occupy == 0)
				{
					sum-= board[i-1][j-1]->value;
					if(board[i-1][j]->occupy == 0||board[i][j-1]->occupy != 0)
						sum-= board[i-1][j]->value;
					else if(board[i][j-1]->occupy == 0||board[i-1][j]->occupy != 0)	
						sum-= board[i][j-1]-> value;
					else if (board[i][j-1]->occupy == 0||board[i-1][j]->occupy == 0)
					{
						if(board[i][j-1]->value >= board[i-1][j]->value)
						{
							sum-=board[i][j-1]->value;
						//	sum+= board[i-1][j]->value;
						}
						else
						{
						//	sum+=board[i][j-1]->value;
							sum-= board[i-1][j]->value;
						}

					}
					else{}

				}
				if(i-1>=0 && j+1<6&& board[i-1][j+1]->occupy == 0)
				{
					sum-= board[i-1][j+1]->value;
					if(board[i-1][j]->occupy == 0||board[i][j+1]->occupy != 0)
						sum-= board[i-1][j]->value;
					else if(board[i][j+1]->occupy == 0||board[i-1][j]->occupy != 0)	
						sum-= board[i][j+1]-> value;
					else if (board[i][j+1]->occupy == 0||board[i-1][j]->occupy == 0)
					{
						if(board[i][j+1]->value >= board[i-1][j]->value)
						{
							sum-=board[i][j+1]->value;
						//	sum+= board[i-1][j]->value;
						}
						else
						{
						//	sum+=board[i][j+1]->value;
							sum-= board[i-1][j]->value;
						}

					}
					else{}
					
				}
				if(i+1<6 && j-1>=0&& board[i+1][j-1]->occupy == 0)
				{
					sum-= board[i+1][j-1]->value;
					if(board[i+1][j]->occupy == 0||board[i][j-1]->occupy != 0)
						sum-= board[i+1][j]->value;
					else if(board[i][j-1]->occupy == 0||board[i+1][j]->occupy != 0)	
						sum-= board[i][j-1]-> value;
					else if (board[i][j-1]->occupy == 0||board[i+1][j]->occupy == 0)
					{
						if(board[i][j-1]->value >= board[i+1][j]->value)
						{
							sum-=board[i][j-1]->value;
						//	sum+= board[i+1][j]->value;
						}
						else
						{
						//	sum+=board[i][j-1]->value;
							sum-= board[i+1][j]->value;
						}

					}
					else{}
					
				}
				if(i+1<6 && j+1<6&& board[i+1][j+1]->occupy == 0)
				{
					sum-= board[i+1][j+1]->value;

					if(board[i+1][j]->occupy == 0||board[i][j+1]->occupy != 0)
						sum-= board[i+1][j]->value;
					else if(board[i][j+1]->occupy == 0||board[i+1][j]->occupy != 0)	
						sum-= board[i][j+1]-> value;
					else if (board[i][j+1]->occupy == 0||board[i+1][j]->occupy == 0)
					{
						if(board[i][j+1]->value >= board[i+1][j]->value)
						{
							sum-=board[i][j+1]->value;
						//	sum+= board[i+1][j]->value;
						}
						else
						{
						//	sum+=board[i][j+1]->value;
							sum-= board[i+1][j]->value;
						}

					}
					else{}
				}
			}
			if(board[i][j]->occupy == 2)
			{
				
				sum -= board[i][j]->value;
				
				if(i-2>=0 && board[i-2][j]->occupy == 0)
				{
					sum -= board[i-2][j]->value;
					if(board[i-1][j]->occupy == 0)
						sum -= board[i-1][j]->value;
				}
				if(i+2<6&& board[i+2][j]->occupy == 0)
				{
					sum-= board[i+2][j]->value;
					if(board[i+1][j]->occupy == 0)
						sum-= board[i+1][j]->value;
				}
				if(j-2>=0&& board[i][j-2]->occupy == 0)
				{
					sum-= board[i][j-2]->value;
					if(board[i][j-1]->occupy == 0)
						sum-= board[i][j-1]->value;
				}
				if(j+2<6&& board[i][j+2]->occupy == 0)
				{
					sum-= board[i][j+2]->value;
					if(board[i][j+1]->occupy == 0)
						sum-= board[i][j+1]->value;
				}
				if(i-1>=0 && j-1>=0&& board[i-1][j-1]->occupy == 0)
				{
					sum-= board[i-1][j-1]->value;
					if(board[i-1][j]->occupy == 0||board[i][j-1]->occupy != 0)
						sum-= board[i-1][j]->value;
					else if(board[i][j-1]->occupy == 0||board[i-1][j]->occupy != 0)	
						sum-= board[i][j-1]-> value;
					else if (board[i][j-1]->occupy == 0||board[i-1][j]->occupy == 0)
					{
						if(board[i][j-1]->value >= board[i-1][j]->value)
						{
							sum-=board[i][j-1]->value;
						//	sum+= board[i-1][j]->value;
						}
						else
						{
						//	sum+=board[i][j-1]->value;
							sum-= board[i-1][j]->value;
						}

					}
					else{}

				}
				if(i-1>=0 && j+1<6&& board[i-1][j+1]->occupy == 0)
				{
					sum-= board[i-1][j+1]->value;
					if(board[i-1][j]->occupy == 0||board[i][j+1]->occupy != 0)
						sum-= board[i-1][j]->value;
					else if(board[i][j+1]->occupy == 0||board[i-1][j]->occupy != 0)	
						sum-= board[i][j+1]-> value;
					else if (board[i][j+1]->occupy == 0||board[i-1][j]->occupy == 0)
					{
						if(board[i][j+1]->value >= board[i-1][j]->value)
						{
							sum-=board[i][j+1]->value;
						//	sum+= board[i-1][j]->value;
						}
						else
						{
						//	sum+=board[i][j+1]->value;
							sum-= board[i-1][j]->value;
						}

					}
					else{}
					
				}
				if(i+1<6 && j-1>=0&& board[i+1][j-1]->occupy == 0)
				{
					sum-= board[i+1][j-1]->value;
					if(board[i+1][j]->occupy == 0||board[i][j-1]->occupy != 0)
						sum-= board[i+1][j]->value;
					else if(board[i][j-1]->occupy == 0||board[i+1][j]->occupy != 0)	
						sum-= board[i][j-1]-> value;
					else if (board[i][j-1]->occupy == 0||board[i+1][j]->occupy == 0)
					{
						if(board[i][j-1]->value >= board[i+1][j]->value)
						{
							sum-=board[i][j-1]->value;
						//	sum+= board[i+1][j]->value;
						}
						else
						{
						//	sum+=board[i][j-1]->value;
							sum-= board[i+1][j]->value;
						}

					}
					else{}
					
				}
				if(i+1<6 && j+1<6&& board[i+1][j+1]->occupy == 0)
				{
					sum-= board[i+1][j+1]->value;

					if(board[i+1][j]->occupy == 0||board[i][j+1]->occupy != 0)
						sum-= board[i+1][j]->value;
					else if(board[i][j+1]->occupy == 0||board[i+1][j]->occupy != 0)	
						sum-= board[i][j+1]-> value;
					else if (board[i][j+1]->occupy == 0||board[i+1][j]->occupy == 0)
					{
						if(board[i][j+1]->value >= board[i+1][j]->value)
						{
							sum-=board[i][j+1]->value;
						//	sum+= board[i+1][j]->value;
						}
						else
						{
						//	sum+=board[i][j+1]->value;
							sum-= board[i+1][j]->value;
						}

					}
					else{}
				}

			}
		}
	}
	return sum;
}

void gameplay(vector<vector<cell*> > & board, int player1, int player2, int player1depth, int player2depth) // player -- 0: minimax, 1:alphabeta, 2:human
{
	int i=0; 
	cell* bestmove;
	cell* tempmove;
	vector<cell*> blitzedcell;
	int utility;
	int alpha = -9999;
	int beta = 9999;
	int count1=0,count2=0;
	clock_t t11,t12,t21,t22;
 	float second1=0,second2=0;

	while(i<36)
	{
		if(player1 == 0)
		{
			++count1;
			t11=clock();
			minimax(board,0, true, player1depth,tempmove, bestmove,nodes1);
			t12=clock();
			second1+= ((float)t12-(float)t11) / CLOCKS_PER_SEC;
			cout.precision(6);
			cout << "This step takes: " << ((float)t12-(float)t11) / CLOCKS_PER_SEC<<" seconds"<<endl;
			cout << "Player1 current time spent: " << second1<<" seconds"<<endl;
			cout << "Player1 takes " << count1 << " steps"<<endl;

		}
		else if(player1 == 1)
		{
			++count1;
			t21=clock();
			alphabeta(board,0, true, player1depth, tempmove, bestmove, alpha, beta,nodes1);
			t22=clock();
			second1+= ((float)t22-(float)t21) / CLOCKS_PER_SEC;
			cout.precision(6);
			cout << "This step takes: " << ((float)t22-(float)t21) / CLOCKS_PER_SEC<<"seconds"<<endl;
			cout << "Player1 current time spent: " << second2<<" seconds"<<endl;
			cout << "Player1 takes " << count2 << " steps"<<endl;
		}
		else
		{
			cout<<" enter your coordinates"<<endl;
			string temp;
			cin >> temp;
			bestmove = board[(int)temp[0]-49][(int)temp[1]-49];	
			bestmove->occupy = 1;
			checkBlitz(bestmove,board,true,blitzedcell);
			blitzedcell.clear();		
		}
		
		bestmove->occupy = 1;
		checkBlitz(bestmove,board,true,blitzedcell);
		blitzedcell.clear();

		if(player2 == 0)
		{
			++count2;
			t21=clock();
			utility = minimax(board,0, false, player2depth, tempmove,bestmove,nodes2);
			t22=clock();
			second2+= ((float)t22-(float)t21) / CLOCKS_PER_SEC;
			cout.precision(6);
			cout << "This step takes: " << ((float)t22-(float)t21) / CLOCKS_PER_SEC<<" seconds"<<endl;
			cout << "Player2 current time spent: " << second1<<" seconds"<<endl;
			cout << "Player2 takes " << count1 << " steps"<<endl;
			
		}
		else if(player2 == 1)
		{
			++count2;
			t21=clock();
			alphabeta(board,0, false, player2depth, tempmove, bestmove, alpha, beta,nodes2);
			t22=clock();
			second2+= ((float)t22-(float)t21) / CLOCKS_PER_SEC;
			cout.precision(6);
			cout << "This step takes: " << ((float)t22-(float)t21) / CLOCKS_PER_SEC<<"seconds"<<endl;
			cout << "Player2 current time spent: " << second2 <<" seconds"<<endl;
			cout << "Player2 takes " << count2 << " steps"<<endl;	
		}

		bestmove->occupy = 2;
		checkBlitz(bestmove,board,false,blitzedcell);
		blitzedcell.clear();
		
		cout<<i<<endl;		
		for(int a =0; a<6; a++)
		{
			for(int b=0; b<6; b++)
			{
					if(board[a][b]->occupy == 1)
			{
					if(board[a][b]->value > 9)
						cout<<"\033[42m" << board[a][b]->value << "\33[0m"<<" "; 
					else
						cout<<"\033[42m" << board[a][b]->value<<" " << "\33[0m"<<" "; 
			}
			else if (board[a][b]->occupy == 2)
			{
					
					if(board[a][b]->value > 9)
						cout<<"\033[44m" << board[a][b]->value << "\33[0m"<<" "; 
					else
						cout<<"\033[44m" << board[a][b]->value<<" " << "\33[0m"<<" "; 
			}
			else
			{					
					if(board[a][b]->value > 9)
						cout<<board[a][b]->value <<" "; 
					else
						cout<<board[a][b]->value <<"  "; 
			}
			}
			cout<<endl;
		}

		cout<<endl;
		cout << "steps " << count1 << " " <<count2<<endl;
		cout<<" nodes   "<<nodes1<<"  "<<nodes2<<endl;
		cout << "Average number of nodes of Player 1 expanded per move: "<< (float)nodes1/count1 <<endl;
		cout << "Average number of nodes of Player 2 expanded per move: "<< (float)nodes2/count2 <<endl;
		cout << "Average amount of time of Player 1 to make a move: "<< (float)second1/count1 <<endl;
		cout << "Average amount of time of Player 2 to make a move: "<< (float)second2/count2 <<endl;
		i +=2;			
	}
//	cout<<"nodes expanded 1:"<<nodes1<<endl;
//	cout<<"nodes expanded 2:"<<nodes2<<endl;
}

/****************************************************/
int main()
{
	ifstream file("Keren.txt");
	vector<string> lines;
	string line;
	
	while(getline(file,line))
	{
		lines.push_back(line);
		line.clear();
	}

	//read in the board configuration	
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


	gameplay( board,1,0,6,3);		// board,(player1 strategy),(player strategy),(player1 depth),(player2 depth)-------- 0:minimax, 1:alphabeta, 2:human3

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


	
	int sum1=0, sum2 =0;
	for(int i =0; i<6; i++)
	{
		for(int j=0; j<6; j++)
		{
			if(board[i][j]->occupy == 1)
				sum1 = sum1 + board[i][j]->value;
			else
				sum2 = sum2 + board[i][j]->value;
		}
	}
	cout<<"player 1 score = "<<sum1<<endl;
	cout<<"player 2 score = "<<sum2<<endl;
	if(sum1 > sum2)
		cout<<"player 1 wins"<<endl;
	else
		cout<<"player 2 wins"<<endl;

	return 0; 
}
