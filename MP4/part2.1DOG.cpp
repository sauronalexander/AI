//mp3 part1

#include <iostream>
#include <cmath>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <iomanip>

/* 5000 training digits, around 500 samples from each digit class. 
Each digit is of size 28x28, and the digits are concatenated together vertically.
' ' means a white (background) pixel, 
'+' means a gray pixel, and 
'#' means a black (foreground) pixel

*/

using namespace std;
int totalpixel = 785;

class node
{
	public:
	double background, foreground;
	node(double b, double f)
	{
		background = b;
		foreground = f;
	}
};
void Train(vector<vector<double> > & weight, vector<vector<char> > & traindata, vector<int> & trainlabel, int t);
void classify(vector<vector<char> > & testdata, vector<int> & result, vector<vector<double> > & weight);
void update(vector<vector<char> > & traindata, vector<double> & w_correct, vector<double> & w_wrong, double alpha, int index);
bool converge(vector<vector<double> > & weight, vector<vector<double> > & temp);


bool converge(vector<vector<double> > & weight, vector<vector<double> > & temp)
{
	for(int k=0; k<10; k++)
	{
		for(int i =0; i<totalpixel; i++)
		{
			if(abs(weight[k][i] - temp[k][i]) > 0.001)
			return false;
		}
	}
	return true;
}
void update(vector<vector<char> > & traindata, vector<double> & w_correct, vector<double> & w_wrong, double alpha, int index)
{
	for(int i=0; i<28; i++)
		for(int j=0; j<28; j++)
		{
			if(traindata[index*28+i][j] !=' ')
			{
				w_correct[i*28+j] += (double)1 * alpha;
				w_wrong[i*28+j] -= (double)1 * alpha;
			}
		}
	w_correct[784] += (double)1 * alpha;
	w_wrong[784] -= (double)1 * alpha;
}
//traindata- data from main, needs processing
void Train(vector<vector<double> > & weight, vector<vector<char> > & traindata, vector<int> & trainlabel, int constant)
{
	int t=0;
	vector<vector<double> >  copy;
	for(int i=0; i<10; i++)
	{
		vector<double> temp;
		for(int j=0; j<totalpixel; j++)
		{
			temp.push_back(0);
		}
		copy.push_back(temp);
	}

	do
	{	
		for(int i=0; i<10; i++)
		{
			for(int j=0; j<totalpixel; j++)
			{
				copy[i][j] = weight[i][j];
			}
		}

		for(int index=0; index<5000; index++)
		{
			double score[10]={0};
			int label = trainlabel[index];
			
	
			for(int k =0; k<10; k++)
			{
				for(int i=0; i<28; i++)
				{
					for(int j=0; j<28; j++)
					{
						if(traindata[index*28+i][j] == ' ')
							score[k] += 0;
						else										
							score[k] += ((double)1 * weight[k][i*28+j]);
					}
				}
				score[k] += weight[k][784];
			}
			int max_index = 0;
			double max = score[0];
			for(int i=0; i<10; i++)
			{
				//cout<<score[i]<<endl;
				if(score[i] > max)
				{
					max_index = i;
					max = score[i];
				}
			}
			double alpha = double(constant) / (double(constant) + double (t));
//			cout<<label<<"  "<<max_index<<endl;
			if(label != max_index) //mismatch
				update(traindata, weight[label], weight[max_index], alpha, index);
		}
		t++;
	}while(!converge(weight, copy));
	//cout <<"epoch = "<< t<<endl;
}

void classify(vector<vector<char> > & testdata, vector<int> & result, vector<vector<double> > & weight)
{
	for(int index=0; index<1000; index++)
	{
		double score[10]={0};

		for(int k =0; k<10; k++)
		{
			for(int i=0; i<28; i++)
			{
				for(int j=0; j<28; j++)
				{
					if(testdata[index*28+i][j] == ' ')
						score[k] += 0;
					else										
						score[k] += ((double)1 * weight[k][i*28+j]);
				}
			}
			score[k] += weight[k][784];
		}
		int max_index = 0;
		double max = score[0];
		for(int i=0; i<10; i++)
		{
			//cout<<score[i]<<endl;
			if(score[i] > max)
			{
				max_index = i;
				max = score[i];
			}
		}
		result.push_back(max_index);
	}
}

int main()
{

	ifstream myfile("trainingimages");
	vector<vector<char> >traindata;

	string line;
	vector<char> temp;
	while(getline(myfile, line))
	{
		for(int i=0; i<28;i++)
		{
			temp.push_back(line[i]);				
		}
		traindata.push_back(temp);
		line.clear();
		temp.clear();
	}
	
	ifstream file("traininglabels");
	vector<int> trainlabel;
	while(getline(file, line))
	{
		int temp1 = (int)line[0]-48;
		trainlabel.push_back(temp1);
	}

	vector<vector<double> > weight;
	for(int i=0; i<10; i++)
	{
		vector<double> temp;
		for(int j=0; j<totalpixel; j++)
			temp.push_back(0);
		weight.push_back(temp);
	}

	vector< vector<node*> > prob_table;
	vector<double>  prior;
	for(int constant=1; constant<1000; constant+=50)
	{
		Train(weight,  traindata,trainlabel, constant);
		
		vector<vector<char> > testdata;
		ifstream file1("testimages");
		while(getline(file1, line))
		{
			for(int i=0; i<28;i++)
			{
				temp.push_back(line[i]);				
			}
			testdata.push_back(temp);
			line.clear();
			temp.clear();
		}
		vector<int> result;
		vector<int> highest;
		vector<int> lowest;
		
	/*	for(int i=0; i<10; i++)
		{
			for(int j =0; j<28; j++)
			{
				for(int k=0; k<28; k++)
					cout<<weight[i][j*28+k]<<" ";
				cout<<endl;
			}
			cout<<endl;
		}*/

			classify(testdata, result,weight);
			

		/****************************************************************************************/
		ifstream file2("testlabels");
		vector<int> testlabel;
		while(getline(file2, line))
		{
			int temp1 = (int)line[0]-48;
			testlabel.push_back(temp1);
		}

		vector< vector<float> > confusion_matrix;
		vector<float>tempp;
		for(int i=0; i<10; i++)
		{
			for(int j=0; j<10; j++)
				tempp.push_back(0);

			confusion_matrix.push_back(tempp);
		}

		/* calculating classification rate*/
		vector<double> rate;
		vector<int> occurence;
		vector<int> classified;
		for(int i=0; i<10; i++)
		{
			rate.push_back(0);
			occurence.push_back(0);
			classified.push_back(0);
		}
	//	for(int i=0; i<1000; i++)
	//		cout<<result[i]<<"   "<<testlabel[i]<<endl;

		for(int i =0; i<1000; i++)
		{
			occurence[testlabel[i]]++;
			//result is the guessed value
			//testlabel is the actual value
			if(result[i] == testlabel[i])
				classified[testlabel[i]]++;	
			else
				confusion_matrix[testlabel[i]][result[i]]++;
		}

		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++)
				{
					confusion_matrix[i][j]/=(float)occurence[i];
					//cout << confusion_matrix[i][j]<<endl;
				}


		int occ=0;
		int cla=0;
		
		for(int i=0; i<10; i++)
		{
			rate[i] = (double)classified[i]/(double)occurence[i];
			//cout<<"class: "<<i<<endl;
			//cout<<"occured: "<<occurence[i]<<endl;
			//cout<<"correct: "<<classified[i]<<endl;
			//cout<<"rate: "<<rate[i]<<endl;
			//cout<<endl;
			occ += occurence[i];
			cla += classified[i];
		}
		
		//cout<<endl;
		cout<</*"overall rate: "<<*/100*(double)cla/(double)occ<<endl;		
		//cout<<endl;
		//cout<<endl;
	}
	

	return 0;
}
