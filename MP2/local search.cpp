//MP1.2

#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <cmath>
#include <algorithm>
#include <time.h>


using namespace std;

class tuple
{
	public:
	int x;
	int y;
	bool visited;
	int color;
	int neighbor;

	tuple(int x, int y)
	{
		this->x = x;
		this->y = y;
		visited = false;
		color = 0;
		neighbor = 0;
	}
	~tuple(){}
}; 

bool check_intersection(tuple * start, tuple * end, vector<pair<tuple*, tuple*> > neighbors);
bool more_connections(int N, vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors);
void clear_visited(vector<tuple*> points, bool flag);
int num_legal_colors(tuple* current, vector<pair<tuple*, tuple*> > neighbors);
bool all_unvisited(vector<tuple*> points);
bool all_visited(vector<tuple*> points);
bool backtrack_without_forwardchecking(tuple* current, vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors);
bool current_visited(vector<tuple*> connected, vector<tuple*> & unvisited);
bool backtrack_with_forwardchecking(tuple* current, vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors);
bool forwardchecking(vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors);
bool local_search(vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors, int N);
int num_conflicts(vector<pair<tuple*, tuple*> > neighbors);
bool conflicted(tuple* current, vector<pair<tuple*, tuple*> > neighbors);

//check intersection with code from http://paulbourke.net/geometry/pointlineplane/pdb.c
//point1->start, point2->end, point3->neighbors[i].first, point4->neighbors[i].second
bool check_intersection(tuple * start, tuple * end, vector<pair<tuple*, tuple*> > neighbors) 
{
	for(int i = 0; i < neighbors.size(); i++)
	{
		double mua, mub;
		double denominator, numeratora, numeratorb;
		denominator = (neighbors[i].second->y-neighbors[i].first->y) * (end->x-start->x) - (neighbors[i].second->x-neighbors[i].first->x) * (end->y-start->y);
		numeratora = (neighbors[i].second->x-neighbors[i].first->x) * (start->y-neighbors[i].first->y) - (neighbors[i].second->y-neighbors[i].first->y) * (start->x-neighbors[i].first->x);
		numeratorb = (end->x-start->x) * (start->y-neighbors[i].first->y) - (end->y-start->y) * (start->x-neighbors[i].first->x);
		
		mua = numeratora / denominator;
		mub = numeratorb / denominator;

		if(denominator == 0.0f && numeratora == 0.0f && numeratorb == 0.0f) //coinsident
		{
			if(max(start->x+start->y , end->x+end->y) > min(neighbors[i].first->x+neighbors[i].first->y , neighbors[i].second->x+neighbors[i].second->y) && max(start->x+start->y , end->x+end->y) <= max(neighbors[i].first->x+neighbors[i].first->y , neighbors[i].second->x+neighbors[i].second->y))
				return false;
			else if(min(start->x+start->y , end->x+end->y) >= min(neighbors[i].first->x+neighbors[i].first->y , neighbors[i].second->x+neighbors[i].second->y) && min(start->x+start->y , end->x+end->y) < max(neighbors[i].first->x+neighbors[i].first->y , neighbors[i].second->x+neighbors[i].second->y))
				return false;
		}
		else if(mua>0 && mua<1 && mub>0 && mub<1) //two lines intersect
			return false;
	}
	return true;
}

//check whether there is more line segments posible.
//needs to be modified with a better algorithm
bool more_connections(int N, vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors)
{
	vector<pair<tuple*, tuple*> > it;
	for(int i = 0; i<N; i++)
	for(int j = 0; j<N; j++)
	{
		if(i == j)
			continue;
		else if(std::find(neighbors.begin(), neighbors.end(), pair<tuple*, tuple*> (points[i], points[j])) != neighbors.end()) //check if the line already exists
			continue;
		else if(std::find(neighbors.begin(), neighbors.end(), pair<tuple*, tuple*> (points[j], points[i])) != neighbors.end())
			continue;
		else if(check_intersection(points[i], points[j], neighbors))  //if a line can be added
		{
			//cout<<i<<","<<j<<endl;
			//cout<<points[i]->x<<","<<points[i]->y<<";"<<points[j]->x<<","<<points[j]->y<<endl;
			return true;
		}
	}
	return false;
}

//clear current data
//if flag is true, clear the color
void clear_visited(vector<tuple*> points, bool flag)
{
	for(int i=0; i<points.size(); i++)
	{
		if(flag)
			points[i]->color = 0;
		points[i]->visited = false;
	}
}

//return the number of legal stats for the point is want to test
int num_legal_colors(tuple* current, vector<pair<tuple*, tuple*> > neighbors)
{
	//initialize legal_colors(fill the legal_vectors with 1,2,3,4) for next point
	vector<int> legal_colors;
	vector<tuple*> connected;
	for(int i=1; i<=4; i ++)
		legal_colors.push_back(i);
	//find all the nabors and eliminate all the legal colors
	for(int i=0; i<neighbors.size();i++)
	{
		if(neighbors[i].first->x == current->x && neighbors[i].first->y == current->y)
		{
			connected.push_back(neighbors[i].second);
			vector<int>::iterator it = std::find(legal_colors.begin(), legal_colors.end(), neighbors[i].second->color); //elminate the illegal states
			if(it != legal_colors.end())
				legal_colors.erase(it);	
		}
		else if(neighbors[i].second->x == current->x && neighbors[i].second->y == current->y)
		{
			connected.push_back(neighbors[i].first);
			vector<int>::iterator it = std::find(legal_colors.begin(), legal_colors.end(), neighbors[i].second->color); //elminate the illegal states
			if(it != legal_colors.end())
				legal_colors.erase(it);	
		}		
	}
	return legal_colors.size();
}

//check whether all points have not been colored yet
bool all_unvisited(vector<tuple*> points)
{
     for(int i=0; i<points.size(); i++)
         if(points[i]->visited)
             return false;
     return true;
}

//check whether all points has been visited yet
bool all_visited(vector<tuple*> points)
{
	for(int i=0; i<points.size(); i++)
	{
         if(points[i]->visited)
             continue;
         else
         	return points[i]->visited;
	}
     return true;
}

/*---------------------------------------------------------------main----------------------------------------------------------------------*/
/*---------------------------------------------------------------main----------------------------------------------------------------------*/
int main()
{
	while(true)
	{
		int N, count;
		cout<<"input number of points: ";
		cin>>N;
		vector<tuple*> points;
		vector<pair<tuple*, tuple*> > neighbors;
		map<int, vector<tuple*> > distance;
	
		//scatter N points in to N*N plan(N*N*unit plan)
		for(count = N; count > 0;)
		{
			int index = rand() % (N*N) + 1; //generate a random number from 1 to (N-1)*(N-1)
			int y = (index % N)? index/N + 1 : index/N; //generate y coordinate from 1 to N  
			int x = (index % N)? index % N : N;    //generate x coordinate from 1 to N
			bool flag = true;
			for(int j=0; j<points.size(); j++)
			{
				if (index == (points[j]->x + (points[j]->y - 1)*N)) //point exists
				{
					flag = false;
					break;
				}
			}
			if(flag)
			{
				cout<<"("<<x<<","<<y<<")"<<";";	
				tuple * temp = new tuple(x,y);
				points.push_back(temp);
				count--;
			}
		}
		cout<<endl<<"connect all points"<<endl;
		clock_t time1, time2;
		time1 = clock();
		//connect all points
		while(more_connections(N, points, neighbors))
		{
	
			int temp = rand() % N;
			tuple * start = points[temp];
			//cout<<"checking points: ("<<start->x<<","<<start->y<<")"<<endl;
			//calculate all the distance form current point
			for(int j=0; j<points.size(); j++)
			{
				int distance_temp = (points[j]->x - points[temp]->x)*(points[j]->x - points[temp]->x) + (points[j]->y - points[temp]->y)*(points[j]->y - points[temp]->y);
				distance[distance_temp].push_back(points[j]);
				//cout<<distance_temp<<":"<<points[j]->x<<","<<points[j]->y<<endl;
			}
			//find the nearest neighbors
			for(int radius = 1; radius<N*N; radius++)
			{
				bool flag = true;
				if(distance[radius].empty())
					continue;
				else
				{
					vector<tuple*>::iterator it_end = distance[radius].begin();
					while(it_end != distance[radius].end())
					{
						//it_end and start is not connected;
						if(std::find(neighbors.begin(), neighbors.end(), pair<tuple*, tuple*> (start, *it_end)) != neighbors.end()) //found
						{
							it_end++;
							continue;
						}
						else if(std::find(neighbors.begin(), neighbors.end(), pair<tuple*, tuple*> (*it_end, start)) != neighbors.end()) //found
						{
							it_end++;
							continue;
						}
						//check intersection
						else if(check_intersection(start, *it_end, neighbors))   //does not intersect with all current line segments
						{
							neighbors.push_back(pair<tuple*, tuple*> (start, *it_end));
							start->neighbor++;
							(*it_end)->neighbor++;
							flag = false;
							cout<<"points connected:"<<"("<<start->x<<","<<start->y<<")"<<"-"<<"("<<(*it_end)->x<<","<<(*it_end)->y<<")"<<";"<<endl;
							break;
						}
						else
							it_end++;
					}
					if(!flag)
						break;
				}
			} //connect nearest neighbor finished 
			distance.clear();
		}
		cout<<"number of constraints:"<<neighbors.size()<<endl;
		time2 = clock();
		float different ((float)time1 - (float)time2);
		cout<<"time cost for connecting points"<<different/CLOCKS_PER_SEC<<endl;
		int size = neighbors.size();
		clock_t t1, t2, t3, t4, t5, t6;
	
		//begin coloring of the map generated above with different methods
		//the map is colored with 4 colors, 1,2,3,4;
		//find the point with least neighbors and start search with that point
	    int k = -1, k_min = N;
	    for(int i=0; i<points.size();i++)
		{
	        if(points[i]->neighbor < k_min)
	        {
	            k = i;
	            k_min = points[i]->neighbor;
	        }
	    }
	    cout<<"least neighbor:"<<k_min<<endl;
	
		cout<<"backtraking search without forward checking"<<endl;
		t1 = clock();
		if(backtrack_without_forwardchecking(points[k], points, neighbors) && num_conflicts(neighbors) == 0)
			for(int i=0; i<points.size(); i++)
				cout<<"point:"<<"("<<points[i]->x<<","<<points[i]->y<<") color:"<<points[i]->color<<";"<<endl;
		else
			cout<<"no solution avaiable"<<endl;
		t2 = clock();
		float diff1 ((float)t2 - (float)t1);
		cout.precision(10);
		cout<<"time cost:"<< diff1/CLOCKS_PER_SEC<<endl;
		
		clear_visited(points, true);
		
		cout<<"backtraking search with forward checking"<<endl;
		t3 = clock();
		if(backtrack_with_forwardchecking(points[k], points, neighbors) && num_conflicts(neighbors) == 0)
			for(int i=0; i<points.size(); i++)
				cout<<"point:"<<"("<<points[i]->x<<","<<points[i]->y<<") color:"<<points[i]->color<<";"<<endl;
		else
			cout<<"no solution avaiable"<<endl;
		t4 = clock();
		float diff2 ((float)t4 - (float)t3);
		cout.precision(10);
		cout<<"time cost:"<< diff2/CLOCKS_PER_SEC<<endl;
		
		clear_visited(points, true);
	
		cout<<"local search"<<endl;
		t5 = clock(); 
		if(local_search(points, neighbors, N) && num_conflicts(neighbors) == 0)
			for(int i=0; i<points.size(); i++)
				cout<<"point:"<<"("<<points[i]->x<<","<<points[i]->y<<") color:"<<points[i]->color<<";"<<endl;
		else
			cout<<"no solution avaiable"<<endl;
		t6 = clock();
		float diff3 = ((float)t6 - (float)t5);
		cout<<"time cost:"<< diff3/CLOCKS_PER_SEC<<endl;
		
		for(int i=0; i<points.size(); i++)
			delete points[i];
		cout<<endl;
	}
	return 0;
}
/*---------------------------------------------------------------end of main---------------------------------------------------------------*/
/*---------------------------------------------------------------end of main---------------------------------------------------------------*/

//backtrack search without forward checking
bool backtrack_without_forwardchecking(tuple* current, vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors)
{
	vector<tuple*> connected;
	//base case: color the first node	
	if(all_unvisited(points))
	{
		//put all the points connected to "current", i.e., next, to the vector "connected"
		for(int j=0; j<neighbors.size(); j++)
		{
			if(neighbors[j].first == current)
				connected.push_back(neighbors[j].second);
			else if(neighbors[j].second == current)
				connected.push_back(neighbors[j].first);		
		}
		for(int i = 1; i <= 4; i++)
		{
			cout<<"legal states:"<<"4"<<"--------";
			current->color = i;
			current->visited = true;
			cout<<"current:"<<current->x<<","<<current->y<<"->color:"<<current->color<<"---------";		
			cout<<"number of points connected:"<<connected.size()<<endl;
			//recurse
			if(backtrack_without_forwardchecking(connected[0], points, neighbors))
				return true;
		}
		return false;		
	}
	else
	{
		//put all the points connected to "current", i.e., next, to the vector "connected" and eliminate illegal states
		vector<int> current_legal_colors;
		for(int i=1; i<=4; i++)
			current_legal_colors.push_back(i);
		for(int i=0; i<neighbors.size(); i++)
		{
			if(neighbors[i].first == current)
			{
				connected.push_back(neighbors[i].second);
				vector<int>::iterator it = std::find(current_legal_colors.begin(), current_legal_colors.end(), neighbors[i].second->color);	//elminate the illegal states
				if(it != current_legal_colors.end())
					current_legal_colors.erase(it);					
			}
			else if(neighbors[i].second == current)
			{
				connected.push_back(neighbors[i].first);
				vector<int>::iterator it = std::find(current_legal_colors.begin(), current_legal_colors.end(), neighbors[i].first->color);	//elminate the illegal states
				if(it != current_legal_colors.end())
					current_legal_colors.erase(it);					
			}		
		}		
		//cout<<"legal states:"<<current_legal_colors.size()<<"--------";
		//fill the color for current point
		for(int i=0; i<current_legal_colors.size(); i++)
		{
			current->color = current_legal_colors[i];
			current->visited = true;
			cout<<"current:"<<current->x<<","<<current->y<<"->color:"<<current->color<<"---------";	
			cout<<"number of points connected:"<<connected.size()<<endl;
			//find the fewest states for connect[i], with index k and number of states num_k
			int k=-1, num_k = 5;
			for(int j=0; j<connected.size(); j++)
				if(!connected[j]->visited)	
					if(num_legal_colors(connected[j], neighbors) < num_k)
					{
						k = j;
						num_k = num_legal_colors(connected[j], neighbors);
					}
			//if all points are visited, return true and end up recursing.
			if(k == -1)
				return true;
			//if there is no legal states anymore for one of the unvisited next points, return false
			else if (num_k == 0)
				return false;
			//recurse
			if(backtrack_without_forwardchecking(connected[k], points, neighbors))
			{
				//check whether there is unreached points, if there are, recurse with these points.
				vector<tuple*> unvisited;
				if(current_visited(connected, unvisited))
					return true;
				else
				{
					for(int j=0; j<unvisited.size(); j++)
						if(!backtrack_without_forwardchecking(connected[j], points, neighbors))
							return false;
					return true;
				}
			}
		}
		return false;
	}
}

//check how many corners that current point links to
bool current_visited(vector<tuple*> connected, vector<tuple*> & unvisited)
{
	for(int i=0; i<connected.size(); i++)
		if(!(connected[i]->visited))
			unvisited.push_back(connected[i]);
	return (unvisited.size() == 0);	
}

//backtrack search with forward checking
bool backtrack_with_forwardchecking(tuple* current, vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors)
{
	vector<tuple*> connected;
	//base case: color the first node	
	if(all_unvisited(points))
	{
		//put all the points connected to "current", i.e., next, to the vector "connected"
		for(int j=0; j<neighbors.size(); j++)
		{
			if(neighbors[j].first == current)
				connected.push_back(neighbors[j].second);
			else if(neighbors[j].second == current)
				connected.push_back(neighbors[j].first);		
		}
		for(int i = 1; i <= 4; i++)
		{
			cout<<"legal states:"<<"4"<<"--------";
			current->color = i;
			current->visited = true;
			cout<<"current:"<<current->x<<","<<current->y<<"->color:"<<current->color<<"---------";		
			cout<<"number of points connected:"<<connected.size()<<endl;
			//check whether current assignment will results in future failure
			//if it will not result in future failure, recurse
			if(forwardchecking(points, neighbors))
				if(backtrack_with_forwardchecking(connected[0], points, neighbors))
					return true;
		}
		return false;		
	}
	else
	{
		//put all the points connected to "current", i.e., next, to the vector "connected" and eliminate illegal states
		vector<int> current_legal_colors;
		for(int i=1; i<=4; i++)
			current_legal_colors.push_back(i);
		for(int i=0; i<neighbors.size(); i++)
		{
			if(neighbors[i].first == current)
			{
				connected.push_back(neighbors[i].second);
				vector<int>::iterator it = std::find(current_legal_colors.begin(), current_legal_colors.end(), neighbors[i].second->color);	//elminate the illegal states
				if(it != current_legal_colors.end())
					current_legal_colors.erase(it);					
			}
			else if(neighbors[i].second == current)
			{
				connected.push_back(neighbors[i].first);
				vector<int>::iterator it = std::find(current_legal_colors.begin(), current_legal_colors.end(), neighbors[i].first->color);	//elminate the illegal states
				if(it != current_legal_colors.end())
					current_legal_colors.erase(it);					
			}		
		}		
		//fill the color for current point
		for(int i=0; i<current_legal_colors.size(); i++)
		{
			cout<<"legal states:"<<current_legal_colors.size()<<"--------";
			current->color = current_legal_colors[i];
			current->visited = true;
			cout<<"current:"<<current->x<<","<<current->y<<"->color:"<<current->color<<"---------";	
			cout<<"number of points connected:"<<connected.size()<<endl;
			//find the fewest states for connect[i], with index k and number of states num_k
			int k=-1, num_k = 5;
			for(int j=0; j<connected.size(); j++)
				if(!connected[j]->visited)
					if(num_legal_colors(connected[j], neighbors) < num_k)
					{
						k = j;
						num_k = num_legal_colors(connected[j], neighbors);
					}
			//if all points are visited, return true and end up recursing.
			if(k == -1)
				return true;
			//if there is no legal states anymore for one of the unvisited next points, return false
			else if (num_k == 0)
				return false;
			//check whether current assignment will results in future failure
			//if it will not result in future failure, recurse
			if(forwardchecking(points, neighbors))
				if(backtrack_with_forwardchecking(connected[k], points, neighbors))
				{
					//check whether there is unreached points, if there are, recurse with these points.
					vector<tuple*> unvisited;
					if(current_visited(connected, unvisited))
						return true;
					else
					{
						for(int j=0; j<unvisited.size(); j++)
							if(!backtrack_with_forwardchecking(connected[j], points, neighbors))
								return false;
						return true;
					}

				}
		}
		return false;
	}
}

//subfunction that does forward checking
//check whether there is legal states left for points uncolored
bool forwardchecking(vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors)
{
	for(int i=0; i<points.size(); i++)
	{
		//only check those points that has not been colored yet
		if(points[i]->color == 0)
		{
			//initialize legal_colors as above
			vector<int> legal_colors;
			for(int j=1; j<=4; j++)
				legal_colors.push_back(j);
			//check for each arc
			for(int j=0; j<neighbors.size(); j++)
			{
				if(neighbors[j].first == points[i])
				{
					vector<int>::iterator it = std::find(legal_colors.begin(), legal_colors.end(), neighbors[j].second->color);	//elminate the illegal states
					if(it != legal_colors.end())
						legal_colors.erase(it);					
				}
				else if(neighbors[j].second == points[i])
				{
					vector<int>::iterator it = std::find(legal_colors.begin(), legal_colors.end(), neighbors[j].first->color);	//elminate the illegal states
					if(it != legal_colors.end())
						legal_colors.erase(it);					
				}					
			}
			if(legal_colors.size() == 0)
				return false;
		}
	}
	return true;
}

//local search
//randomly choose any conflicted variable and choose the value that violates the fewest constrainsts
bool local_search(vector<tuple*> points, vector<pair<tuple*, tuple*> > neighbors, int N)
{
	//first assign intial solution. set all the color to 1
	for(int i=0; i<points.size(); i++)
	{
		points[i]->color = 1;
	}
	int min_conflicts = num_conflicts(neighbors);
	int last_min_conflicts = min_conflicts + 1;
	int l=0;
	while(num_conflicts(neighbors) > 0)
	{
		int temp = rand() % N;
		if(last_min_conflicts > min_conflicts)
		{
			cout<<"number of conflicts:"<<min_conflicts<<", search loops done:"<<l<<", index searched:"<<temp<<endl;
			clear_visited(points, false);	
		}
		//if number of conflicts keeps the same and all points are visited and looped, return false
		//i.e., there is no solution at all
		else if(last_min_conflicts == min_conflicts)			
			if(all_visited(points))
				return false;
		
		last_min_conflicts = min_conflicts;
		int k = 0;
		//if it is a conflicted point, we change its value
		//check all of the possible assignment and choose the one with least conflicts
		//when the number of conflicts is minimized, we chose that value to be the color of that point
		if(conflicted(points[temp], neighbors))
		{
			int color = points[temp]->color;
			for(int j=0; j<4; j++)
			{
				points[temp]->color = points[temp]->color + j;
				if(points[temp]->color > 4)
					points[temp]->color -= 4;
				if(num_conflicts(neighbors) < min_conflicts)
				{
					k = j;
					min_conflicts = num_conflicts(neighbors);
				}
			}
			points[temp]->color = color + k;
			if(points[temp]->color > 4)
				points[temp]->color -= 4;
		}
		min_conflicts = num_conflicts(neighbors);
		points[temp]->visited = true;			
		l++;
	}
	cout<<"number of conflicts:"<<num_conflicts(neighbors)<<", search loops done:"<<l<<endl;
	return true;
}

//this is the function we need to minimize
//it returns the number of conflicts that current color assignment has
int num_conflicts(vector<pair<tuple*, tuple*> > neighbors)
{
	int conflicts = 0;
	for(int i=0; i<neighbors.size(); i++)
		if(neighbors[i].first->color == neighbors[i].second->color)
		{
			conflicts++;
		}
	return conflicts;
}

//check whether current point is a conflicted point
bool conflicted(tuple* current, vector<pair<tuple*, tuple*> > neighbors)
{
	for(int i=0; i<neighbors.size(); i++)
	{
		if(neighbors[i].first == current)
		{
			if(neighbors[i].second->color == current->color)
				return true;
		}
		else if(neighbors[i].second == current)
		{
			if(neighbors[i].first->color == current->color)
				return true;
		}
	}
	return false;
}
